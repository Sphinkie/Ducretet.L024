/* ************************************************************************************************
 *  
 *   DUCRETET L.024
 *
 * Software pour la radio vintage DUCRETET L.024
 *
 * Pilotage par Arduino-Uno d'un player MP3 et d'un afficheur
 *
 * Les musiques sont sur la carte SD du player MP3, et doivent etre préparées préalablement 
 * avec l'utilitaire mp3tag.exe (voir doc).
 *
 * Note:
 *   The F() macro tells the compiler to leave this particular array in PROGMEM (Flash). 
 *   (cela economise de la RAM programme)
 *   Then when it is time to access it, one byte of the data is copied to RAM at a time. 
 *   There’s a small performance overhead for this extra work. However, printing strings 
 *   over Serial or to a LCD is a really slow process, so a few extra clock cycles really won’t matter.
 *
 *************************************************************************************************** 
 * 0.1  19/02/2020  version initiale - Basée sur RADIAL-V. Read ID3 tags in MP3 file.
 * 0.2  27/05/2020  Playing a MP3 file
  *************************************************************************************************** 
*/

#include "Ducretet.h"
#include "RotaryButton.h"
#include "CapButton.h"
#include "SelfReturnButton.h"
#include "Catalog.h"
#include "MusicPlayer.h"
//#include "RemoteDisplay.h"
#include "Bouchon.h"

// *******************************************************************************
// Mapping du cablage
// *******************************************************************************

// ------------------ Pour MP3 Shield
// Midi_In              //      NOT USED    (MP3 shield) avec hardware interrupt
// GPIO                 //      GPIO        (MP3 shield)
//#define MP3_RESET     //      NOT USED    (MP3 shield) VS1053 reset pin (unused!)
#define MP3_DREQ   3    // D3   DataRequest (MP3 shield) avec hardware interrupt 1. VS1053 Data REQuest, ideally an Interrupt pin.
#define MP3_DCS    6    // D6   MP3 Data CS (MP3 shield) VS1053 Data/Command S  elect pin (output)
#define MP3_CS     7    // D7   MP3 CS      (MP3 shield) VS1053 Chip Select pin (output)
#define SD_CS      4    // D4   SD CS       (MP3 shield) SD-Card chip select pin

#define AGAIN      2    // D2   Digital In     avec hardware interrupt 0
#define NEXT       18   // D18  Digital In     avec hardware interrupt 5
#define PROMOTE    19   // D19  Digital In     avec hardware interrupt 4

// ------------------ Pour I2C
#define FM_SDIO    20   // D20 I2C Bus - Digital In/out avec hardware interrupt 3
#define FM_SCLK    21   // D21 I2C Bus - Digital In/out avec hardware interrupt 2

#define MODE_4     22   // D22  input   C-MODE-4    bouton Mode
#define MODE_3     24   // D24  input   C-MODE-3    bouton Mode
#define MODE_2     26   // D26  input   C-MODE-2    bouton Mode
#define MODE_1     28   // D28  input   C-MODE-1    bouton Mode
#define SPARE1     37   // D37  Spare1 Connector
#define SPARE_LED  43   // D43  input   SPARE LED (connector JP5)
#define LED_1      13   // D  output  LED
#define LED_2      47   // D47  output  LED
#define SPARE2     49   // D49  Spare2 Connector

// ------------------ Pour SPI
#define SPI_MISO   50    // D50  input  
#define SPI_MOSI   51    // D51  output 
#define SPI_SCLK   52    // D52  output 
#define SPI_SS     53    // D53  input  (configuré en output car Master)

#define TUNE_OUT   A8    // Analog output for bouton Tune (charge pin): Créneaux de 5v.
#define TUNE_IN    A9    // Analog input for bouton Tune: Read value



// *******************************************************************************
// variables globales
// *******************************************************************************
MusicPlayer        MP3Player(MP3_CS, MP3_DCS, MP3_DREQ, SD_CS);
Catalog            Catalogue;
Rotary             ModeButton(MODE_1,MODE_2,MODE_3,MODE_4); 
CapButton          TuneButton(TUNE_OUT,TUNE_IN);
//SelfReturnButton   PromoteButton(PROMOTE, &ISR_PromoteButton);
//SelfReturnButton   AgainButton(AGAIN,     &ISR_AgainButton);
//SelfReturnButton   NextButton(NEXT,       &ISR_NextButton);
Bouchon  RemoteTFT;  // RemoteDisplay      

volatile int       Action = _IDLE;             // variable de type volatile, utilisable par les ISR
String             MusicFile;                  // ID du clip MP3 en cours
String             NextMusicFile;              // ID du prochain clip MP3 à jouer


// *******************************************************************************
// The setup function runs once when you press reset or power the board
// *******************************************************************************
void setup() 
{
    // Dès le début, on met les SlaveSelect du bus SPI au niveau haut, pour qu'ils ne reçoivent pas de messages parasites.
    pinMode(53,      OUTPUT);    digitalWrite(53,      HIGH);
    pinMode(MP3_CS,  OUTPUT);    digitalWrite(MP3_CS,  HIGH);
    pinMode(MP3_DCS, OUTPUT);    digitalWrite(MP3_DCS, HIGH);
    pinMode(SD_CS,   OUTPUT);    digitalWrite(SD_CS,   HIGH);
  
    Serial.begin(115200);
    while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only
  
    Serial.println(F("================================="));
    Serial.println(F("==    DUCRETET     v0.2        =="));
    Serial.println(F("================================="));
    Serial.print  (F("CPU Frequency: ")); Serial.print(F_CPU/1000000); Serial.println(F(" MHz"));
    Serial.print  (F("Free RAM: "));      Serial.print(FreeRam(),DEC); Serial.println(F(" bytes"));

    pinMode(LED_1, OUTPUT); // Led SPI BUSY (Catalog accede à la carte SD)
    pinMode(LED_2, OUTPUT);
  

/*
    // ------------------------------------------------------------
    // Initalise le bus I2C
    // ------------------------------------------------------------
    RemoteTFT.initI2C();
*/
    // ------------------------------------------------------------
    // Initalise le Shield Sparkfun MP3 player
    // ------------------------------------------------------------
    MP3Player.initialize();
    digitalWrite(LED_1,HIGH);   // Eteint la Led témoin SPI BUSY

    // ------------------------------------------------------------
    // Initalise les autres objets
    // ------------------------------------------------------------
    // Catalogue.initialize();      
    TuneButton.begin();
    NextMusicFile = "NOISE" ;   // ID du prochain clip MP3 à jouer ("STARTER")
    Action = _IDLE;

/*  // -----------------------------------------------------------------------------
    // On attend que l'Arduino SLAVE soit prêt sur le bus I2C (status 0x01 = READY)
    // -----------------------------------------------------------------------------
    Serial.println(F("Waiting I2C Slave ready..."));
    while (SlaveArduinoStatus!=0)
    {
       SlaveArduinoStatus=RemoteTFT.requestStatus();
       delay(100);
    }
    Serial.println(F("================================="));
    RemoteTFT.setSlavePresent(USE_TWO_ARDUINO);
*/
}


// **************************************************************************************************************************************************************
// The loop function runs over and over again forever
// **************************************************************************************************************************************************************
void loop() 
{
    // --------------------------------------------------------------
    // Gestion du bouton TUNING
    // --------------------------------------------------------------
    int tuning = TuneButton.readValue();
  
    if (TuneButton.hasChanged())  
    {
        // On a détecté un changement de tuning. delta significatif + not still moving
        Serial.print(F(">>Tuning has changed: ")); Serial.println(tuning); 
        // Si le bouton TUNE a bougé, on est peut-être sur une autre année ou un autre genre
        // On détermine donc l'année/genre correspondant à ce réglage.
        Catalogue.setNewRequestedValues(tuning);
        // On relance la recherche du clip suivant
        initClipSearch();
    }

    // --------------------------------------------------------------
    // Gestion du bouton MODE (YEAR/GENRE/RANDOM)
    // --------------------------------------------------------------
    ModeButton.readPosition();
    if (ModeButton.hasChanged())  
    {
        // On a détecté un changement de Mode. On met à jour la valeur de l'année et du genre.
        Serial.print("Mode is now "); Serial.print(ModeButton.getValue()); Serial.println(F(" [1:year 2:genre 3:random]"));
        Catalogue.setNewRequestedValues(tuning); 
        // On relance la recherche du clip suivant
        initClipSearch();
    }

    // On actualise la recherche du clip suivant
    searchNextClip();

    // --------------------------------------------------------------
    // Gestion du clip MP3
    // --------------------------------------------------------------
    if (!MP3Player.isPlaying())      // S'il n'y a pas de morceau en cours, on en joue un.
    {
        Serial.println(F("No clip playing. Taking the next one."));
        Catalogue.takeClip();             // On prend ce qu'on a pu trouver. Le Next devient le Courant.
        MusicFile = Catalogue.getSelectedClipID();
        MP3Player.playTrack(MusicFile);   // Et on le joue (eventuellement, cela peut être Noise).
    }

    // --------------------------------------------------------------
    // Etapes que l'on fait lorsque l'on commence à jouer un MP3:
    // --------------------------------------------------------------
    int Step = MP3Player.getStep();
    switch (Step)
    {
      case 2: // On affiche le mode en cours.
            Serial.println(F("  >Send Requested Mode to TFT (from Tuning)"));
            RemoteTFT.clearBackground();
            displayRequestedMode();
            break;
      case 3: // On affiche les infos du clip issues du fichier MP3
            Serial.println(F("  >Send Title to TFT (from tag)"));
            // RemoteTFT.printLog(MusicFile);
            if (MusicFile == "NOISE") 
            {
               String message = F("Rech.");
               switch (ModeButton.getValue())
               {
                   case 1: message += F(" parmi les favoris"); break;
                   case 2: message += F(" par année");         break;
                   case 3: message += F(" par genre");         break;
                   case 4: message += F(" aléatoire");         break;
               }
               RemoteTFT.printTitle(message);
            }
            else
            {
               RemoteTFT.printTitle(MP3Player.getTitle());
               RemoteTFT.printArtist(MP3Player.getArtist());
               RemoteTFT.printAlbum(MP3Player.getAlbum());
            }
            break;
    case 4: // On affiche la suite des infos du clip issues du fichier Catalog
            Serial.println(F("  >Send Year+Genre to TFT (from Catalog)"));
            if (MusicFile == "NOISE") 
            {
               RemoteTFT.printYear(" ");
               RemoteTFT.printGenre(" ");
            }
            else
            {
                RemoteTFT.printYear(Catalogue.getSelectedClipYear());
                RemoteTFT.printGenre(Catalogue.getSelectedClipGenre());
            }
            break;
    case 5: // On determine le nombre d'étoiles et on l'affiche.
            Serial.println(F("  >Send Rating to TFT (from Catalog)"));
            if (MusicFile == "NOISE") 
              RemoteTFT.printStars("-");
            else
              RemoteTFT.printStars(Catalogue.getSelectedClipRating());
            break;
    case 12: // on surveille la RAM consommée
            Serial.print(F("Free RAM (bytes)= ")); Serial.println(FreeRam(), DEC);
            // On baisse l'intensité de l'affichage
            RemoteTFT.setBacklight(false);
            break;
   }

  // --------------------------------------------------------------
  // Gestion du bouton AGAIN/NEXT/PROMOTE (evenements déclenchés par ISR)
  // --------------------------------------------------------------
  switch (Action)
  {
       case _IDLE: break;
/*       
       case _NEXT: // On enlève une étoile et on stoppe le clip.
                        Action=_IDLE; 
                        NextButton.wasPushed();   
                        Serial.println("NEXT !");    
                        MP3Player.stopTrack();    
                        Catalogue.removeStar(); // On édite le catalog pendant que le clip est stoppé.
                        RemoteTFT.printStars(Catalogue.getSelectedClipRating());
                        break; 
       case _AGAIN: // On ajoute une étoile, et on reprend le clip du début
                        Action=_IDLE; 
                        AgainButton.wasPushed();  
                        Serial.println("AGAIN !");   
                        digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
                        MP3Player.pauseDataStream();
                        Catalogue.addStar();
                        MP3Player.resumeDataStream();
                        digitalWrite(LED_1,HIGH); // Eteint la Led témoin SPI BUSY
                        MP3Player.restartTrack();
                        RemoteTFT.printStars(Catalogue.getSelectedClipRating());
                        break;    
      case _PROMOTE: // On ajoute une étoile
                        Action=_IDLE; 
                        PromoteButton.wasPushed();
                        Serial.println("PROMOTE !"); 
                        digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
                        MP3Player.pauseDataStream();
                        Catalogue.addStar();    
                        MP3Player.resumeDataStream();
                        digitalWrite(LED_1,HIGH); // Eteint la Led témoin SPI BUSY
                        Serial.println(F(" display: Stars"));
                        RemoteTFT.printStars(Catalogue.getSelectedClipRating());
                        break; 
      */                       
  }
  // --------------------------------------------------------------
  // Temporisation de la boucle
  // --------------------------------------------------------------
  delay(60*1000);  // 1 sec
}


// *******************************************************************************
// Reinitialisation d'une recherche de clip dans le catalogue
// Ceci comporte un accès au fichier catalogue
// *******************************************************************************
void initClipSearch()
{
      digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
      MP3Player.pauseDataStream();
      if (ModeButton.getValue()==YEAR)   Catalogue.initSearchForRequestedYear();
      if (ModeButton.getValue()==GENRE)  Catalogue.initSearchForRequestedGenre();
/*      if (ModeButton.getValue()==RATING) Catalogue.initSearchForRequestedRating(); */
      MP3Player.resumeDataStream();
      digitalWrite(LED_1,HIGH); // Eteint la Led témoin SPI BUSY
}

// *******************************************************************************
// Renvoie le prochain clip a jouer (en fonction des boutons Mode et Tuning)
// *******************************************************************************
void searchNextClip()
{
    // On lit la position du bouton de reglage, pour le cas où il aurait changé au cours du clip précédent
    int tuning = TuneButton.readValue();
    // Serial.print(F("  searchNextClip for mode ")); Serial.println(ModeButton.getValue()); 

    digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
    MP3Player.pauseDataStream();
    switch (ModeButton.getValue())
    {
        case YEAR  : Catalogue.searchClipForRequestedYear();         break;
        case GENRE : Catalogue.searchClipForRequestedGenre();        break;
/*        case RATING: Catalogue.searchClipForRequestedRating();       break; */
        case RANDOM: Catalogue.selectRandomClip();                   break;
    }
    MP3Player.resumeDataStream();
    digitalWrite(LED_1,HIGH); // Eteint la Led témoin SPI BUSY
}

// *******************************************************************************
// Envoie un message à l'Arduino Slave, pour afficher le mode demandé
// *******************************************************************************
void displayRequestedMode()
{
            String ModeMessage;
            switch (ModeButton.getValue())
            {
                case 1: ModeMessage = F("Musique favorite "); 
                        ModeMessage += String(Catalogue.Plexi.Rating);
                        ModeMessage += F("*");
                        break;
                case 2: ModeMessage = F("    Années ");
                        ModeMessage += String(Catalogue.Plexi.RangeStart);
                        ModeMessage += "-";
                        ModeMessage += String(Catalogue.Plexi.RangeEnd);
                        break;
                case 3: ModeMessage = F("Genre: ");
                        ModeMessage += Catalogue.Plexi.Genre;            
                        break;
                case 4: ModeMessage = F("   Musique aleatoire");
                        break;
            }
            RemoteTFT.printLog(ModeMessage);
}

/*
// *******************************************************************************
// Interruption appellée si le bouton est tourné CW
// Note: le Serial ne fonctionne pas durant les ISR!
// *******************************************************************************
void ISR_NextButton() 
{
    Action = _NEXT;
    NextButton.setStatus(true);
}

// *******************************************************************************
// Interruption appellée si le bouton est tourné CCW
// Note: le Serial ne fonctionne pas durant les ISR!
// *******************************************************************************
void ISR_AgainButton() 
{
    Action = _AGAIN;
    AgainButton.setStatus(true);
}

// *******************************************************************************
// Interruption appellée si le bouton est poussé
// Note: le Serial ne fonctionne pas durant les ISR!
// *******************************************************************************
void ISR_PromoteButton() 
{
    Action = _PROMOTE;
    PromoteButton.setStatus(true);
}
*/
