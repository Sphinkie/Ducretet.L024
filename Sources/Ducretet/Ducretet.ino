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
 * 0.1  19/02/2020  vesion initiale - Basée sur RADIAL-V
  *************************************************************************************************** 
*/

/*
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>
*/
#include "Ducretet.h"
/*
#include "MusicPlayer.h"
#include "RotaryButton.h"
#include "CapButton.h"
#include "SelfReturnButton.h"
#include "Catalog.h"
#include "RemoteDisplay.h"
*/

// *******************************************************************************
// Mapping du cablage
// *******************************************************************************
// MP3 data request     // D2 DataRequest   (MP3 shield) avec hardware interrupt 0
#define AGAIN      3    // D3   Digital In avec hardware interrupt 1
// Midi-In              // D3   NOT USED    (MP3 shield) avec hardware interrupt 1
// GPIO                 // D4   GPIO        (MP3 shield)
// MP3-reset            // D8   MP3 reset   (MP3 shield)
#define MP3_CS     6    // D6   MP3 CS      (MP3 shield)
#define MP3_DCS    7    // D7   MP3 data CS (MP3 shield)
#define SD_CS      9    // D9   SD CS       (MP3 shield)

#define NEXT       18   // D18  Digital In     avec hardware interrupt 5
#define PROMOTE    19   // D19  Digital In     avec hardware interrupt 4

// ------------------ Pour I2C
#define FM_SDIO    20 // D20 I2C Bus - Digital In/out avec hardware interrupt 3
#define FM_SCLK    21 // D21 I2C Bus - Digital In/out avec hardware interrupt 2
#define FM_GPIO2   23 // D23 input  : FM shield pulse received then Seek/Tune completed. (pin GPI02 du shield FM Si7403)

#define MODE_4     25   // D25  input   C-MODE-5    bouton Mode
#define MODE_3     27   // D27  input   C-MODE-4    bouton Mode
#define MODE_2     29   // D29  input   C-MODE-3    bouton Mode
#define MODE_1     31   // D31  input   C-MODE-2    bouton Mode
#define SPARE1     37   // D37  Spare1 Connector
#define SPARE_LED  43   // D43  input   SPARE LED (connector JP5)
#define LED_1      45   // D45  output  LED
#define LED_2      47   // D47  output  LED
#define SPARE2     49   // D49  Spare2 Connector
// SPI_MISO             // D50  input  
// SPI_MOSI             // D51  output 
// SPI_SCLK             // D52  output 
// SPI_SS               // D53  input  (configuré en output car Master)

#define TUNE_OUT   A12    // Analog output for bouton Tune (charge pin): Créneaux de 5v.
#define TUNE_IN    A14    // Analog input for bouton Tune: Read value



// *******************************************************************************
// variables globales
// *******************************************************************************
MusicPlayer        mp3shield(SD_CS);
Catalog            Catalogue;
Rotary             ModeButton(MODE_1,MODE_2,MODE_3,MODE_4); 
CapButton          TuneButton(TUNE_OUT,TUNE_IN);
/*SelfReturnButton   PromoteButton(PROMOTE, &ISR_PromoteButton);   */
SelfReturnButton   AgainButton(AGAIN,     &ISR_AgainButton);     
SelfReturnButton   NextButton(NEXT,       &ISR_NextButton);    
RemoteDisplay      RemoteTFT;

volatile int       Action = _IDLE;             // variable de type volatile, utilisable par les ISR
String             MusicFile;                  // ID du clip MP3 en cours
String             NextMusicFile;              // ID du prochain clip MP3 à jouer
/*char               SlaveArduinoStatus=0;       // Status de l'Arduino Slave I2C (1=ready)                */

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
    Serial.println(F("==    DUCRETET     v0.1        =="));
    Serial.println(F("================================="));
    Serial.print  (F("CPU Frequency: ")); Serial.print(F_CPU/1000000); Serial.println(F(" MHz"));
    Serial.print  (F("Free RAM: "));      Serial.print(FreeRam(),DEC); Serial.println(F(" bytes"));
    // FreeRam is provided by SdFatUtil.h

    pinMode(LED_1, OUTPUT); // Led SPI BUSY (Catalog accede à la carte SD)
    pinMode(LED_2, OUTPUT);
    pinMode(K1,    OUTPUT); // Commande relay K1
    pinMode(K2,    OUTPUT); // Commande relay K2
  

/*
    // ------------------------------------------------------------
    // Initalise le bus I2C
    // ------------------------------------------------------------
    RemoteTFT.initI2C();
*/
    // ------------------------------------------------------------
    // Initalise le Shield Sparkfun MP3 player
    // ------------------------------------------------------------
    mp3shield.initialize();
    // Listing des fichiers de la carte SD 
    // mp3shield.dir();
    mp3shield.setDiffmode();    // Enleve un echo acoustique désagréable
    digitalWrite(LED_1,HIGH);   // Eteint la Led témoin SPI BUSY

    // ------------------------------------------------------------
    // Initalise les autres objets
    // ------------------------------------------------------------
    Catalogue.initialize();      
    TuneButton.begin();
    NextMusicFile = "NOISE" ;   // ID du prochain clip MP3 à jouer ("STARTER")
    Action = _IDLE;

/*    // -----------------------------------------------------------------------------
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
        // Si le bouton TUNE a bougé, on est peut-être sur une autre année, ou un autre genre, ou autre rating.
        // On redétermine donc l'année/genre/rating correspondant à ce réglage.
        Catalogue.setNewRequestedValues(tuning);
        // On relance la recherche du clip suivant
        initClipSearch();
    }

    // --------------------------------------------------------------
    // Gestion du bouton MODE (RATING/YEAR/GENRE/RANDOM)
    // --------------------------------------------------------------
    ModeButton.readPosition();
    if (ModeButton.hasChanged())  
    {
        // On a détecté un changement de Mode. On met à jour la valeur de l'année et du genre.
        Serial.print("Mode is now "); Serial.print(ModeButton.getValue()); Serial.println(F(" [1:fav 2:year 3:genre 4:random]"));
        Catalogue.setNewRequestedValues(tuning); 
        // On relance la recherche du clip suivant
        initClipSearch();
    }

    // On actualise la recherche du clip suivant
    searchNextClip();

    // --------------------------------------------------------------
    // Gestion du clip MP3
    // --------------------------------------------------------------
    if (!mp3shield.isPlaying())      // S'il n'y a pas de morceau en cours, on en joue un.
    {
        Serial.println(F("No clip playing. Taking the next one."));
        Catalogue.takeClip();             // On prend ce qu'on a pu trouver. Le Next devient le Courant.
        MusicFile = Catalogue.getSelectedClipID();
        mp3shield.playTrack(MusicFile);   // Et on le joue (eventuellement, cela peut être Noise).
    }

    // --------------------------------------------------------------
    // Etapes que l'on fait lorsque l'on commence à jouer un MP3:
    // --------------------------------------------------------------
    int Step = mp3shield.getStep();
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
               RemoteTFT.printTitle(mp3shield.getTitle());
               RemoteTFT.printArtist(mp3shield.getArtist());
               RemoteTFT.printAlbum(mp3shield.getAlbum());
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
       case _NEXT: // On enlève une étoile et on stoppe le clip.
                        Action=_IDLE; 
                        NextButton.wasPushed();   
                        Serial.println("NEXT !");    
                        mp3shield.stopTrack();    
                        Catalogue.removeStar(); // On édite le catalog pendant que le clip est stoppé.
                        RemoteTFT.printStars(Catalogue.getSelectedClipRating());
                        break; 
       case _AGAIN: // On ajoute une étoile, et on reprend le clip du début
                        Action=_IDLE; 
                        AgainButton.wasPushed();  
                        Serial.println("AGAIN !");   
                        digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
                        mp3shield.pauseDataStream();
                        Catalogue.addStar();
                        mp3shield.resumeDataStream();
                        digitalWrite(LED_1,HIGH); // Eteint la Led témoin SPI BUSY
                        mp3shield.restartTrack();
                        RemoteTFT.printStars(Catalogue.getSelectedClipRating());
                        break;
       case _PROMOTE: // On ajoute une étoile
                        Action=_IDLE; 
                        PromoteButton.wasPushed();
                        Serial.println("PROMOTE !"); 
                        digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
                        mp3shield.pauseDataStream();
                        Catalogue.addStar();    
                        mp3shield.resumeDataStream();
                        digitalWrite(LED_1,HIGH); // Eteint la Led témoin SPI BUSY
                        Serial.println(F(" display: Stars"));
                        RemoteTFT.printStars(Catalogue.getSelectedClipRating());
                        break; 
  }
  // --------------------------------------------------------------
  // Temporisation de la boucle
  // --------------------------------------------------------------
  delay(1000);
}


// *******************************************************************************
// Reinitialisation d'une recherche de clip dans le catalogue
// Ceci comporte un accès au fichier catalogue
// *******************************************************************************
void initClipSearch()
{
      digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
      mp3shield.pauseDataStream();
      if (ModeButton.getValue()==YEAR)   Catalogue.initSearchForRequestedYear();
      if (ModeButton.getValue()==GENRE)  Catalogue.initSearchForRequestedGenre();
      if (ModeButton.getValue()==RATING) Catalogue.initSearchForRequestedRating();
      mp3shield.resumeDataStream();
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
    mp3shield.pauseDataStream();
    switch (ModeButton.getValue())
    {
        case YEAR  : Catalogue.searchClipForRequestedYear();         break;
        case GENRE : Catalogue.searchClipForRequestedGenre();        break;
        case RATING: Catalogue.searchClipForRequestedRating();       break;
        case RANDOM: Catalogue.selectRandomClip();                   break;
    }
    mp3shield.resumeDataStream();
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
