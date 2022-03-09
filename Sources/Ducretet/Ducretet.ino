/* ************************************************************************************************
 *  
 *   DUCRETET L.024
 *
 * Software pour la radio vintage DUCRETET L.024
 * Target : Arduino MEGA or MEGA2560
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
 * ************************************************************************************************** 
 * 0.5  11/06/2020  Gestion de LED selon le BEAT
 * ************************************************************************************************** */
#include "Ducretet.h"
#include "RotaryButton.h"
#include "CapButton.h"
#include "SelfReturnButton.h"
#include "Catalog.h"
#include "MusicPlayer.h"
#include "Rythmic.h"
#include "RemoteDisplay.h"
#include "Bouchon.h"

// *******************************************************************************
// variables globales
// *******************************************************************************

MusicPlayer        MP3Player(MP3_CS, MP3_DCS, MP3_DREQ, SD_CS);
Catalog            Catalogue;
Rotary             ModeButton(MODE_1,MODE_2,MODE_3,MODE_4); 
CapButton          TuneButton(TUNE_OUT,TUNE_IN);
Rythmic            Beat_ISR;
RemoteDisplay      RemoteTFT;

//SelfReturnButton   PromoteButton(PROMOTE, &ISR_PromoteButton);
//SelfReturnButton   AgainButton(AGAIN,     &ISR_AgainButton);
//SelfReturnButton   NextButton(NEXT,       &ISR_NextButton);

volatile int       Action = _IDLE;          // variable volatile (stockée en RAM et pas dans un registre), utilisable par les ISR
String             MusicFile;               // ID du clip MP3 en cours
String             NextMusicFile;           // ID du prochain clip MP3 à jouer



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
    Serial.println(F("==    DUCRETET     v0.5-a      =="));
    Serial.println(F("================================="));
    Serial.print  (F("CPU Frequency: ")); Serial.print(F_CPU/1000000); Serial.println(F(" MHz"));
    Serial.print  (F("Free RAM: "));      Serial.print(FreeRam(),DEC); Serial.println(F(" bytes"));

    pinMode(LED_1, OUTPUT);    // Led SPI BUSY (Catalog accede à la carte SD)
    pinMode(LED_2, OUTPUT);
    digitalWrite(LED_1,LOW);   // Allume la Led témoin SPI BUSY
  
    // ------------------------------------------------------------
    // Initalise le bus I2C et l'ecran OLED
    // ------------------------------------------------------------
    RemoteTFT.initialize();
    RemoteTFT.clearScreen();

    // ------------------------------------------------------------
    // Initalise le Shield Sparkfun MP3 player
    // ------------------------------------------------------------
    MP3Player.initialize();
            
    // ------------------------------------------------------------
    // Initalise les autres objets
    // ------------------------------------------------------------
    Catalogue.initialize();      
    TuneButton.initialize();
    Action = _IDLE;

    // ------------------------------------------------------------
    // On commence par jouer Noise
    // ------------------------------------------------------------
    MusicFile="NOISE";
    MP3Player.playTrack(MusicFile);   

    digitalWrite(LED_1,HIGH);   // Eteint la Led témoin SPI BUSY
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
        // On a détecté un changement de tuning: delta significatif + not still moving
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
        Serial.print("Mode is now "); Serial.print(ModeButton.getValue()); Serial.println(F(" [1:GENRE 2:YEAR 3:BEAT 4:RANDOM]"));
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
        Beat_ISR.stopBeat();
        Catalogue.takeClip();                       // On prend ce qu'on a pu trouver. Le Next devient le Courant.
        MusicFile = Catalogue.getSelectedClipID();
        MP3Player.playTrack(MusicFile);             // Et on le joue (éventuellement, cela peut être Noise).
    }

    // --------------------------------------------------------------
    // Etapes que l'on fait lorsque l'on commence à jouer un MP3:
    // --------------------------------------------------------------
    const int DELAY = 40;   // ms
    const int F = 10;       // Permet de regler le delai entre les affichages (F=10 => 3sec)
    switch (MP3Player.getStep())
    {
     case 1*F: // On affiche le mode en cours.
            RemoteTFT.clearScreen();
            displayRequestedMode();
            break;
     case 2*F: // On affiche la recherche en cours (le cas échéant)
            if (MusicFile == "NOISE") 
            {
               switch (ModeButton.getValue())
               {
                   case FAV: RemoteTFT.printTitle(F("Rech. parmi les favoris")); break;
                   case YEAR: RemoteTFT.printTitle(F("Rech. par année"));        break;
                   case BEAT: RemoteTFT.printTitle(F("Rech. par tempo"));        break;
                   case GENRE: RemoteTFT.printArtist(F("Rech. par genre"));       break;
                   case RANDOM: RemoteTFT.printTitle(F("Rech. aléatoire"));      break;
               }
            }
            break;
     case 3*F:   // Affiche l'ANNEE (Catalog)
            RemoteTFT.printYear(Catalogue.getSelectedClipYear());
            break;
     case 4*F:
            // Affiche le GENRE (Catalog)
            RemoteTFT.printGenre(Catalogue.getSelectedClipGenre());
            break;
     case 5*F:
            // Affiche l'ARTISTE (tag MP3)
            RemoteTFT.printArtist(MP3Player.getArtist());
            break;
     case 6*F:
            // Affiche le RATING (Catalog)
            if (MusicFile == "NOISE") RemoteTFT.printStars(0);
            else RemoteTFT.printStars(Catalogue.getSelectedClipRating());
            break;
      case 7*F:
            // Affiche le BEAT (Catalog)
            if (MusicFile != "NOISE") 
            {
                int beat = Catalogue.getSelectedClipBeat();
                RemoteTFT.printBeat(String(beat));
            }            
            break;
     case 9*F:
            // On affiche le TITRE
            RemoteTFT.printTitle(MP3Player.getTitle());
            break;
     case 10*F: // On fait scroller le TITRE
            RemoteTFT.startScrolling();
            break;
     case 11*F:
            // Fait clignoter le BEAT (Catalog)
            if (MusicFile != "NOISE")
            {
                int beat = Catalogue.getSelectedClipBeat();
//                Beat_ISR.startBeat(beat);
            }            
            break;
     case 19*F:
            RemoteTFT.stopScrolling();            
            break;
     case 20*F: // On affiche le debut du TITRE en fixe
            RemoteTFT.printTitle(MP3Player.getTitle());
            break;
    case 50*F: // on surveille la RAM consommée
            Serial.print(F("Free RAM (bytes)= ")); Serial.println(FreeRam(), DEC);
            break;
    case 21*F: // STOP (DEBUG)
            RemoteTFT.printStars(5);
            /* MP3Player.stopTrack(); */
            break;

    case 25*F: 
            RemoteTFT.startStarAnimation(5);
            break;
    case 25*F+1:
    case 25*F+2:
    case 25*F+3:
    case 25*F+4:
    case 25*F+5:
    case 25*F+6:
    case 25*F+7:
    case 25*F+8:
    case 25*F+9:
    case 25*F+10:
    case 25*F+11:
    case 25*F+12:
    case 25*F+13:
    case 25*F+14:
            // stars animation
            RemoteTFT.animStars();
            break;
            
   }

/*       
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
  }
*/                       

  // --------------------------------------------------------------
  // Temporisation de la boucle
  // --------------------------------------------------------------
  if (RemoteTFT.titleScrolling)
      RemoteTFT.scroll();
  else
     delay(DELAY);
}


// *******************************************************************************
// Reinitialisation d'une recherche de clip dans le catalogue
// Ceci comporte un accès au fichier catalogue
// *******************************************************************************
void initClipSearch()
{
      Serial.println(F(" >initClipSearch"));
      digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
      MP3Player.pauseDataStream();
      switch (ModeButton.getValue())
      {
          case YEAR  : Catalogue.initSearchForRequestedYear();     break;
          case BEAT  : Catalogue.initSearchForRequestedBeat();     break;
          case GENRE : Catalogue.initSearchForRequestedGenre();    break;
          case RATING: Catalogue.initSearchForRequestedRating();   break;
      }
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
    //Serial.print(F(" >searchNextClip for mode ")); Serial.print(ModeButton.getValue());

    digitalWrite(LED_1,LOW); // Allume la Led témoin SPI BUSY
    MP3Player.pauseDataStream();
    switch (ModeButton.getValue())
    {
        case YEAR  : Catalogue.searchClipForRequestedYear();       break;
        case BEAT  : Catalogue.searchClipForRequestedBeat();       break;
        case GENRE : Catalogue.searchClipForRequestedGenre();      break;
        case RATING: Catalogue.searchClipForRequestedRating();     break;
        case RANDOM: Catalogue.selectRandomClip();                 break; 
    }
    MP3Player.resumeDataStream();
    digitalWrite(LED_1,HIGH); // Eteint la Led témoin SPI BUSY
}

// *******************************************************************************
// Affiche le mode demandé
// Sur OLED : 17 char max.
// *******************************************************************************
void displayRequestedMode()
{
    Serial.println(F("  Display Requested Mode"));
    String ModeMessage;
    switch (ModeButton.getValue())
    {
       case RATING:   // 19 chars
                ModeMessage = F("Musique favorite "); 
                ModeMessage += String(Catalogue.Plexi.Rating);
                ModeMessage += F("*");
                break;
         case YEAR:   // 16 chars
                ModeMessage = F("Années ");
                ModeMessage += String(Catalogue.Plexi.RangeStart);
                ModeMessage += "-";
                ModeMessage += String(Catalogue.Plexi.RangeEnd);
                break;
         case BEAT:     // 10 chars
                ModeMessage = "Tempo: " + Catalogue.Plexi.Beat;            
                break;
         case GENRE:    
                ModeMessage = "> " + Catalogue.Plexi.Genre + " <";
                break;
         case RANDOM:   // 17 chars
                ModeMessage = "Musique aleatoire";
                break;
    }
    RemoteTFT.setHeader(ModeMessage);
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
