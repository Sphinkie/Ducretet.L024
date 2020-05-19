/* ***********************************************************
 *  MUSIC PLAYER
 *  
 *  Gestion de la carte Sparkfun MP3 player, en utilisant la 
 *  librairie "SFE-MP3-Shield" de William Greiman et le bus SPI.
 *  
 ************************************************************* */

#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>
#include "MusicPlayer.h"

// ******************************************************************************
// Déclaration des objets (en dehors de la classe)
// ******************************************************************************
/*
// Mode réel        
SdFat        sd;    
SFEMP3Shield SparkfunShield;
*/

// Mode simulé
#include "Bouchon.h"
       Bouchon      SparkfunSD;
       Bouchon      SparkfunShield;

// ******************************************************************************
// Constructor
// ******************************************************************************
MusicPlayer::MusicPlayer(byte pinSD_CS)
{
  // Initialisations
    Step=0;   // no step
  /********************* Patch ***********************************/
  // On a souvent l'erreur:  Can't access SD card. Do not reformat. No card, wrong chip select pin, or SPI problem
  // La solution qui semble marcher est de mettre les pins SS à HIGH( inactif) dès le début. 
  // Sinon les lecteurs SD peuvent recevoir des parasites sur la ligne SPI, et se bloquer...
  pinMode(pinSD_CS, OUTPUT);  digitalWrite(pinSD_CS, HIGH);   
  pinMode(53, OUTPUT);        digitalWrite(53, HIGH);   // SPI_SS = 53
  /***************************************************************/
}


// ******************************************************************************
// Initialise la carte MP3 et la carte SD incluse.
// Cette carte utilise les pins suivantes:
// - SPI (MISO MOSI SCLK) 
// - SPI SlaveSelect :
//    6 = MP3 CS 
//    7 = MP3 data CS
//    9 = SD CS
// - 2+3+4+8 : DataRequest + Midi-In + GPIO + MP3-reset
// ******************************************************************************
void MusicPlayer::initialize()
{
  Serial.println(F("MP3+SD initialization."));

  //Initialize the SD card. 
  // Par defaut, SD_SEL vaut 9  (Slave Select)
  if(!SparkfunSD.begin(9, SPI_FULL_SPEED)) SparkfunSD.initErrorHalt();   // SD_SEL,SPI_FULL_SPEED
  Serial.println(F(" SD initialized."));
  
  if(!SparkfunSD.chdir("/")) SparkfunSD.errorHalt("sd.chdir");
  Serial.println(F(" directory changed to root"));

  //Initialize the MP3 Player Shield
  uint8_t result; //result code from some function as to be tested at later time.
  result = SparkfunShield.begin();
  
  //check result, see readme for error codes.
  if(result == 0) 
    Serial.println(F(" MP3 player initialized."));
  else
  {
    Serial.print(F("Error when trying to start MP3 player. Code "));
    Serial.println(result);
    Serial.println(getErrorMsg(BEGIN+result));
  }
  // Enleve une sorte d'écho acoustique désagréable
  SparkfunShield.setDifferentialOutput(1);
}

// ******************************************************************************
// Cette fonction joue le fichier MP3 demandé (ID sans extension).
// Le fichier MP3 doit être dans le repertoire /Music.
// ******************************************************************************
void MusicPlayer::playTrack(String filename)
{
    uint8_t result; 
    char    trackName[30];

    // Tell the MP3 Shield to play a track
    filename = String ("/Music/"+filename+".mp3");
    filename.toCharArray(trackName,30);
    
    result = SparkfunShield.playMP3(trackName);
    //check result, see readme for error codes.
    if(result != 0) {
      Serial.print(F("Error code: "));
      Serial.print(result);
      Serial.print(F(" when trying to play track "));
      Serial.println(filename);
      Serial.println(getErrorMsg(PLAY+result));
    } else {
      Serial.println("Playing: "+filename);
      Step=1;   // first step
    }
}


// ******************************************************************************
// Renvoie TRUE si la carte est en train de jouer. FALSE sinon.
// ******************************************************************************
bool MusicPlayer::isPlaying()
{
   return (SparkfunShield.isPlaying());
}


// ******************************************************************************
// Reglage du volume audio.
// input values are -1/2dB. e.g. 40 results in -20dB.  ( 0dB = max volume)
// ******************************************************************************
void MusicPlayer::setVolume(int volume)
{
    // push byte into both left and right assuming equal balance.
    SparkfunShield.setVolume(volume,volume);
}


// ******************************************************************************
// Stoppe la lecture du fichier mp3
// ******************************************************************************
void MusicPlayer::stopTrack()
{
   // Stop the current track
   Serial.println(F("  Stopping mp3"));
   SparkfunShield.stopTrack();
   Step=0;   // step number
}

// ******************************************************************************
// Recommence la lecture du fichier mp3 à partir de 0
// ******************************************************************************
void MusicPlayer::restartTrack()
{
   // Restart the current track
   Serial.println(F("  Restarting mp3"));
   SparkfunShield.skipTo(0);
   Step=0;   // step number
}


// *******************************************************************************
// Affiche les informations (tags) sur le clip en cours
// *******************************************************************************
void MusicPlayer::displayMediaInfo()
{
  // Note these buffer may be desired to exist globably. but do take much space if only needed temporarily, hence they are here.
  char title[30];   // buffer to contain the extract the Title from the current filehandles
  char album[30];   // buffer to contain the extract the album name from the current filehandles
  char artist[30];  // buffer to contain the extract the artist name from the current filehandles

  strcpy(title, "Current title");
  strcpy(artist,"Current artist");
  strcpy(album, "Current album");

  if (!SparkfunShield.isPlaying()) return;

  //we can get track info by using the following functions and arguments
  //the functions will extract the requested information, and put it in the array we pass in
  SparkfunShield.trackTitle((char*)&title);
  SparkfunShield.trackArtist((char*)&artist);
  SparkfunShield.trackAlbum((char*)&album);

  //print out the arrays of track information
  // Serial.print(F(" title:  "));   Serial.write((byte*)&title, 30);  Serial.println();
  // Serial.print(F(" by:     "));   Serial.write((byte*)&artist,30);  Serial.println();
  // Serial.print(F(" album:  "));   Serial.write((byte*)&album, 30);  Serial.println();

  Serial.print(F(" title:  "));   Serial.println(title);
  Serial.print(F(" by:     "));   Serial.println(artist);
  Serial.print(F(" album:  "));   Serial.println(album);
}

// *******************************************************************************
// Récupère le titre (tag) du clip en cours. On tronque à 52
// *******************************************************************************
String MusicPlayer::getTitle()
{
  String retour;

  // S'il n'y a pas de clip en cours, on revoie un blanc.
  if (!SparkfunShield.isPlaying()) return " ";
  // On lit le tag TITLE, et on le met dans le Buffer
  SparkfunShield.trackTitle((char*)&Buffer);
  // S'il est vide, on renvoie un blanc.
  if (strlen(Buffer)==0) return " ";
  retour = String(Buffer);
  retour.remove(52);
  return (retour);
}

// *******************************************************************************
// Récupère le nom de l'artiste (tag) du clip en cours
// *******************************************************************************
String MusicPlayer::getArtist()
{
  String retour;

  // S'il n'y a pas de clip en cours, on revoie un blanc.
  if (!SparkfunShield.isPlaying()) return " ";
  // On lit le tag ARTIST, et on le met dans le Buffer
  SparkfunShield.trackArtist((char*)&Buffer);
  // S'il est vide, on renvoie un blanc.
  if (strlen(Buffer)==0) return " ";
  retour = String(Buffer);
  retour.remove(32);
  return (retour);
}

// *******************************************************************************
// Récupère le nom de l'album (tag) du clip en cours
// *******************************************************************************
String MusicPlayer::getAlbum()
{
  String retour;
  
  // S'il n'y a pas de clip en cours, on revoie un blanc.
  if (!SparkfunShield.isPlaying()) return " ";
  // On lit le tag ALBUM, et on le met dans le Buffer
  SparkfunShield.trackAlbum((char*)&Buffer);
  // S'il est vide, on renvoie un blanc.
  if (strlen(Buffer)==0) return " ";
  // Sinon, on le convertit en String, on tronque à 32 et on le renvoie
  retour = String(Buffer);
  retour.remove(32);
  return (retour);
}


// ******************************************************************************
// Cette fonction fait un reset sur le chip de la carte MP3
// ******************************************************************************
void MusicPlayer::resetBoard()
{
    SparkfunShield.stopTrack();
    SparkfunShield.vs_init();
    Serial.println(F(" Reseting VS10xx chip"));
    Step=0;   // numero de step
}

/*
// ******************************************************************************
// Cette fonction cercle entre les 4 modes de "surround" pour oreillettes
// ******************************************************************************
void MusicPlayer::changeSurroundMode()
{
    uint8_t earspeaker = SparkfunShield.getEarSpeaker();
    if(earspeaker >= 3)
    {
      earspeaker = 0;
    } else 
    {
      earspeaker++;
    }
    SparkfunShield.setEarSpeaker(earspeaker); // commit new earspeaker
    Serial.print(F(" earspeaker set to "));
    Serial.println(earspeaker, DEC);
}
*/



// ******************************************************************************
// Passe de mode MONO a STEREO
// ******************************************************************************
void MusicPlayer::setStereo(bool ON)
{
    if (ON)  SparkfunShield.setMonoMode(1);
    else     SparkfunShield.setMonoMode(0);

    uint16_t monostate = SparkfunShield.getMonoMode();
    if (monostate==1) Serial.println(F(" mode stereo."));
    else Serial.println(F(" mode mono."));
}


// ******************************************************************************
// Affiche le status du player MP3 sur le port Série.
// Note: on pourrait ajouter sur l'écran LCD.
// Note: on pourrait déclencher par un bouton arrière
// ******************************************************************************
void MusicPlayer::printStatus()
{
    Serial.println(F("Current State of VS10xx is."));
    Serial.print(F("isPlaying() = "));
    Serial.println(SparkfunShield.isPlaying());

    Serial.print(F("getState() = "));
    switch (SparkfunShield.getState()) {
    case uninitialized:
      Serial.print(F("uninitialized"));
      break;
    case initialized:
      Serial.print(F("initialized"));
      break;
    case deactivated:
      Serial.print(F("deactivated"));
      break;
    case loading:
      Serial.print(F("loading"));
      break;
    case ready:
      Serial.print(F("ready"));
      break;
    case playback:
      Serial.print(F("playback"));
      break;
    case paused_playback:
      Serial.print(F("paused_playback"));
      break;
    case testing_memory:
      Serial.print(F("testing_memory"));
      break;
    case testing_sinewave:
      Serial.print(F("testing_sinewave"));
      break;
    }
    Serial.println();
}


// *********************************************************************
// Display error signification
// *********************************************************************
String MusicPlayer::getErrorMsg(int errCode)
{
  switch (errCode)
  {
    // Begin errors
    case BEGIN+0: return F("OK");
    case BEGIN+1: return F("Failure of SdFat to initialize physical contact with the SdCard.");
    case BEGIN+2: return F("Failure of SdFat to start the SdCard's volume.");
    case BEGIN+3: return F("Failure of SdFat to mount the root directory on the volume of the SdCard.");
    case BEGIN+4: return F("Other than default values were found in the SCI_MODE register.");
    case BEGIN+5: return F("SCI_CLOCKF did not read back and verify the configured value.");
    case BEGIN+6: return F("Warning: Patch was not loaded successfully, this may result in playTrack errors.");
    // Playing errors:
    case PLAY+0: return F("OK");
    case PLAY+1: return F("Already playing track.");
    case PLAY+2: return F("File not found.");
    case PLAY+3: return F("indicates that the VSdsp is in reset.");
    // Skip errors:
    case SKIP+0: return F("OK");
    case SKIP+1: return F("Not Playing track.");
    case SKIP+2: return F("Failed to skip to new file location.");
  }
}



// ******************************************************************************
// Liste le contenu de la carte SD sur le port série
// ******************************************************************************
void MusicPlayer::dir()
{
  // Display the files on the SdCard 
  // (prevent root.ls when playing, something locks the dump, but keeps playing).
  if(!SparkfunShield.isPlaying()) 
  {
     Serial.println(F("Files found (name date size):"));
     SparkfunSD.ls(LS_R | LS_DATE | LS_SIZE);
     
  } 
  else 
  {
     Serial.println(F("Busy Playing Files, try again later."));
  }
}


// ******************************************************************************
// Un petit séquenceur d'étapes (de 1 à MAX_STEP). 0 si inactif.
// Il part de 1 chaque fois qu'un nouveau clip commence à être joué (etat transitoire).
// ******************************************************************************
int MusicPlayer::getStep()
{
  int CurrentStep = Step;
  if (CurrentStep > 0) Step++;
  if (Step > MAX_STEP) Step=0;
  // Serial.print(F("  Step ")); Serial.print(CurrentStep); Serial.print(F("-->")); Serial.println(Step);
  return Step;
}


// ******************************************************************************
// Disable interrupts to avoid collisions on the SPI bus between this code 
// and the SparkfunShield library.
// These  functions make sure there will be no data collisions on the SPI bus 
// caused by the MP3 decoder asking for more data at the wrong time. 
// You need to wrap any SPI code you add to your project with these two functions.
// ******************************************************************************
void MusicPlayer::pauseDataStream()
{
  SparkfunShield.pauseDataStream();
}



// ******************************************************************************
// Enable interrupts.
// Note: you can not stop the data stream to the MP3 Shield for too long 
//       before it runs out of data so be careful and try not to do too much stuff in between the functions.
// ******************************************************************************
void MusicPlayer::resumeDataStream()
{
  SparkfunShield.resumeDataStream();
}
  
