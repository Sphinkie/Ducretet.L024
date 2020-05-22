/* ***********************************************************
 *  MUSIC PLAYER
 *  
 *  Gestion de la carte Adafruit MP3 player, en utilisant la 
 *  librairie Adafruit_VS1053 et le bus SPI.
 *  
 *  Cette carte utilise les pins suivantes:
 *  - SPI (MISO MOSI SCLK) 
 *  - SPI SlaveSelect : MP3_CS MP3_DCS SD_CS
 *  - Ainsi que : DataRequest 
 ************************************************************* */
//#include <SPI.h>
//#include <SD.h>

#include "MusicPlayer.h"

/*
// Mode réel        
SdFat        sd;    
*/


/*
// Mode simulé
#include "Bouchon.h"
       Bouchon      SparkfunSD;
       Bouchon      SparkfunShield;
*/

// ******************************************************************************
// Constructor : aussi  Adafruit_VS1053_FilePlayer 
// ******************************************************************************
MusicPlayer::MusicPlayer(byte pinMP3_RESET, byte pinMP3_CS, byte pinMP3_DCS, byte pinMP3_DREQ, byte pinSD_CS) : Adafruit_VS1053_FilePlayer(pinMP3_RESET, pinMP3_CS, pinMP3_DCS, pinMP3_DREQ, pinSD_CS)
{
  // Initialisations
    Step=0;   // no step
    pinCard_CS=pinSD_CS;

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
// ******************************************************************************
void MusicPlayer::initialize()
{
  boolean ok;
  Serial.println(F("MP3+SD initialization."));

  //Initialize the MP3 Player Shield
  ok=Adafruit_VS1053_FilePlayer::begin();
  //check result, see readme for error codes.
  if(ok) 
  {
    Serial.println(F(" MP3 shield initialized."));
    Adafruit_VS1053_FilePlayer::sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  }
  else
  {
    Serial.print(F("Error when initializing MP3 shield. Couldn't find VS1053, do you have the right pins defined?"));
  }

  //Initialize the SD card.
  byte errcode=SD.begin(pinCard_CS);
  if (errcode!=0) 
  {
    Serial.println(F("SD failed, or not present: Program Stopped"));
    while (1);  // don't do anything more
  }
  Serial.println(F(" SD initialized."));
  // list files
  printDirectory();

  // Set volume for left, right channels. lower numbers == louder volume!
  Adafruit_VS1053_FilePlayer::setVolume(20,20);

  // Using MP3_DREQ interrupt pin, we can do background audio playing.
  ok=Adafruit_VS1053_FilePlayer::useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  if (!ok)
    Serial.println(F("MP3_DREQ pin is not an interrupt pin"));

}

// ******************************************************************************
// Cette fonction joue le fichier MP3 demandé (ID sans extension).
// Le fichier MP3 doit être dans le repertoire /Music.
// ******************************************************************************
void MusicPlayer::playTrack(String filename)
{
    boolean ok; 
    char    trackName[30];

    // Tell the MP3 Shield to play a track
    filename = String ("/Music/"+filename+".mp3");
    filename.toCharArray(trackName,30);
    
    ok = Adafruit_VS1053_FilePlayer::startPlayingFile(trackName);
    if (ok) 
    {
      Serial.println("Playing: "+filename);
      Step=1;   // first step
    }
    else
    { 
      Serial.print(F("Error when trying to play track "));
      Serial.println(filename);
    }
}


// ******************************************************************************
// Renvoie TRUE si la carte est en train de jouer. FALSE sinon.
// ******************************************************************************
bool MusicPlayer::isPlaying()
{
   return (Adafruit_VS1053_FilePlayer::playingMusic);
}


// ******************************************************************************
// Reglage du volume audio (0 à 255). Valeur haute = volume faible.
// input values are -1/2dB. e.g. 40 results in -20dB.  ( 0dB = max volume)
// ******************************************************************************
void MusicPlayer::setVolume(int volume)
{
    // push byte into both left and right assuming equal balance.
    Adafruit_VS1053_FilePlayer::setVolume(volume,volume);
}


// ******************************************************************************
// Stoppe la lecture du fichier mp3
// ******************************************************************************
void MusicPlayer::stopTrack()
{
   // Stop the current track
   Serial.println(F("  Stopping mp3"));
   Adafruit_VS1053_FilePlayer::stopPlaying();
   Step=0;   // step number
}

// ******************************************************************************
// Recommence la lecture du fichier mp3 à partir de 0
// ******************************************************************************
void MusicPlayer::restartTrack()
{
   // Restart the current track
   Serial.println(F("  Restarting mp3"));
   Adafruit_VS1053_FilePlayer::startPlayingFile(0);  // TODO
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

  if (!Adafruit_VS1053_FilePlayer::playingMusic) return;

  //we can get track info by using the following functions and arguments
  //the functions will extract the requested information, and put it in the array we pass in
  // trackTitle((char*)&title);
  // trackArtist((char*)&artist);
  // trackAlbum((char*)&album);

  //print out the arrays of track information
  // Serial.print(F(" title:  "));   Serial.write((byte*)&title, 30);  Serial.println();
  // Serial.print(F(" by:     "));   Serial.write((byte*)&artist,30);  Serial.println();
  // Serial.print(F(" album:  "));   Serial.write((byte*)&album, 30);  Serial.println();

  Serial.print(F(" title:  "));   Serial.println(title);
  Serial.print(F(" by:     "));   Serial.println(artist);
  Serial.print(F(" album:  "));   Serial.println(album);
}

// *******************************************************************************
// Récupère le titre (tag) du clip en cours. On tronque à 32
// *******************************************************************************
String MusicPlayer::getTitle()
{
  String retour;
  char* infobuffer;

  // S'il n'y a pas de clip en cours, on revoie un blanc.
  if (!Adafruit_VS1053_FilePlayer::playingMusic) return " ";
  // On lit le tag TITLE, et on le met dans le Buffer
  // AdafruitShield.trackTitle((char*)&Buffer);
  getTrackInfo(TRACK_TITLE, infobuffer);
  // S'il est vide, on renvoie un blanc.
  if (strlen(Buffer)==0) return " ";
  retour = String(Buffer);
  retour.remove(32);
  return (retour);
}

// *******************************************************************************
// Récupère le nom de l'artiste (tag) du clip en cours
// *******************************************************************************
String MusicPlayer::getArtist()
{
  String retour;
  char* infobuffer;

  // S'il n'y a pas de clip en cours, on revoie un blanc.
  if (!Adafruit_VS1053_FilePlayer::playingMusic) return " ";
  // On lit le tag ARTIST, et on le met dans le Buffer
  //AdafruitShield.trackArtist((char*)&Buffer);
  getTrackInfo(TRACK_ARTIST, infobuffer);
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
  char* infobuffer;
  
  // S'il n'y a pas de clip en cours, on revoie un blanc.
  if (!Adafruit_VS1053_FilePlayer::playingMusic) return " ";
  // On lit le tag ALBUM, et on le met dans le Buffer
  //AdafruitShield.trackAlbum((char*)&Buffer);
  //void SFEMP3Shield::trackArtist(char* infobuffer)
  getTrackInfo(TRACK_ALBUM, infobuffer);
  
  // S'il est vide, on renvoie un blanc.
  if (strlen(Buffer)==0) return " ";
  // Sinon, on le convertit en String, on tronque à 32 et on le renvoie
  retour = String(Buffer);
  retour.remove(32);
  return (retour);
}


// ******************************************************************************
// Cette fonction fait un reset sur le chipset de la carte MP3
// ******************************************************************************
void MusicPlayer::resetBoard()
{
    Adafruit_VS1053_FilePlayer::stopPlaying();
    Adafruit_VS1053_FilePlayer::reset();
    Serial.println(F(" Reseting VS1053 chipset"));
    Step=0;   // numero de step
}



// ******************************************************************************
// Liste le contenu de la carte SD sur le port série
// ******************************************************************************
void MusicPlayer::printDirectory()
{
  File dir = SD.open("/");
  int numTabs=0;
  // Display the files on the SdCard 
  if(!Adafruit_VS1053_FilePlayer::playingMusic) 
  {
     Serial.println(F("Files found (name date size):"));
     while(true) 
     {
         File entry =  dir.openNextFile();
         if (!entry) 
         {
            // no more files
            // Serial.println("**nomorefiles**");
            break;
         }
         for (uint8_t i=0; i<numTabs; i++) 
         {
            Serial.print('\t');
         }
         Serial.print(entry.name());
         if (entry.isDirectory()) 
         {
            Serial.println("/");
            //printDirectory(entry, numTabs+1);
         }
         else 
         {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
         }
         entry.close();
      }
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
// and the AdafruitShield library.
// These functions make sure there will be no data collisions on the SPI bus 
// caused by the MP3 decoder asking for more data at the wrong time. 
// You need to wrap any SPI code you add to your project with these two functions.
// ******************************************************************************
void MusicPlayer::pauseDataStream()
{
  Adafruit_VS1053_FilePlayer::pausePlaying(true);
}

// ******************************************************************************
// Enable interrupts.
// Note: you can not stop the data stream to the MP3 Shield for too long 
//       before it runs out of data so be careful and try not to do too much stuff in between the functions.
// ******************************************************************************
void MusicPlayer::resumeDataStream()
{
  Adafruit_VS1053_FilePlayer::pausePlaying(false);
}


/* *****************************************************************************************************************
 * Fetch ID3 Tag information
 *    offset : offset for the desired information desired.
 *    infobuffer (output): pointer char array of filename to be read.
 *
 * Read current filehandles offset of track ID3 tag information. Then strip all non readible (ascii) characters.
 * ***************************************************************************************************************** */
void MusicPlayer::getTrackInfo(uint8_t offset, char* infobuffer)
{
 strncpy (infobuffer, Buffer, 30);   // on retourne 30 caracteres à partir de offset : TODO
 // TODO : Ajouter un /0 à la fin
 // Remove the non-alpha characters
// infobuffer = strip_nonalpha_inplace(infobuffer);
 ID3tag* pointeur;
 // on le fait pointer sur le buffer de 128 chars contenant les tags
 pointeur = *Buffer;
 strncpy (infobuffer, pointeur->title, 30);   // on retourne 30 caracteres à partir de offset : TODO
}

/* *****************************************************************************************************************
 * Elle consiste en un espace de 128 octets placés à la fin du fichier. 
 * Les 3 premiers octets commencent par la chaîne « TAG », cela permet de trouver le début des informations par les lecteurs MP3. 
 * Le reste des octets est partagé entre les différents champs d'informations. 
 * Les chaînes de caractères doivent être codées en ISO/CEI 8859-1, seuls les caractères de l'alphabet latin peuvent donc être utilisés. 
 * 
 * Offset (en partant du début de la structure)  Taille (en octets)   Description
    0       3       Identifiant "TAG"
    3       30      Titre de la chanson
    33      30      Nom de l'interprète
    63      30      Nom de l'album
    93      4       Année de parution
    97      30      Commentaire sur la chanson
    127     1       Genre musical (code sur 1 octet)
 * \note this suspends currently playing streams and returns afterwards.
 * Restoring the file position to where it left off, before resuming.
 */
void MusicPlayer::readID3tags()
{
  unsigned long currentPos;
  unsigned long fileSize;
  strcpy (Buffer,"TAGRailroad boy  - - - - - - - --Joan Baez x x x x x x x x x xxIn Concert X X X X X X X X X X1964Commentaires..................1");

  //disable interupts
  // if(playingMusic) disableRefill();
  
  // save the current position in the file
  currentPos = currentTrack.position();
  fileSize = currentTrack.size();

  //skip to end
  // currentTrack.seekEnd((-128 + offset));
  currentTrack.seek(fileSize-128);

  //read 30 bytes of tag information at -128 + offset
  // currentTrack.read(infobuffer, 30);
  // infobuffer = strip_nonalpha_inplace(infobuffer);
  currentTrack.read(Buffer, 128);

  //seek back to saved file position
  currentTrack.seek(currentPos);

  //renable interupt
  // if(playingMusic) enableRefill();
  
}

/*
//chomp non printable characters out of string. 
char* strip_nonalpha_inplace(char *s) 
{
   for ( ; *s && !isalpha(*s); ++s)
     ; // skip leading non-alpha chars
   if(*s == '\0')
     return s; // there are no alpha characters
 
   char *tail = s + strlen(s);
   for ( ; !isalpha(*tail); --tail)
     ; // skip trailing non-alpha chars
   *++tail = '\0'; // truncate after the last alpha
   return s;
 }
 */
