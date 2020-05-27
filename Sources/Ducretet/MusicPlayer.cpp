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
#include "MusicPlayer.h"

/*
// Mode simulé
#include "Bouchon.h"
       Bouchon      SparkfunSD;
       Bouchon      SparkfunShield;
*/

// ******************************************************************************
// Constructor : aussi  Adafruit_VS1053_FilePlayer
// On précise les Pins du bus SPI, qui sont particulières sur les Arduino Mega.
// ******************************************************************************
MusicPlayer::MusicPlayer(byte pinMP3_RESET, byte pinMP3_CS, byte pinMP3_DCS, byte pinMP3_DREQ, byte pinSD_CS) : Adafruit_VS1053_FilePlayer(SPI_MOSI, SPI_MISO, SPI_SCLK, pinMP3_RESET, pinMP3_CS, pinMP3_DCS, pinMP3_DREQ, pinSD_CS)
{
  // Initialisations
    Step=0;   // no step
    pinCard_CS=pinSD_CS;

  /********************* Patch ***********************************/
  // On a souvent l'erreur:  Can't access SD card. Do not reformat. No card, wrong chip select pin, or SPI problem
  // La solution qui semble marcher est de mettre les pins SS à HIGH( inactif) dès le début. 
  // Sinon les lecteurs SD peuvent recevoir des parasites sur la ligne SPI, et se bloquer...
/*  pinMode(pinSD_CS, OUTPUT);  digitalWrite(pinSD_CS, HIGH);   
  pinMode(53, OUTPUT);        digitalWrite(53, HIGH);   // SPI_SS = 53
  */
  /***************************************************************/
}


// ******************************************************************************
// Initialise la carte MP3 et la carte SD incluse.
// ******************************************************************************
void MusicPlayer::initialize()
{
  boolean initOk;
  Serial.println(F("MP3+SD initialization."));

  //Initialize the MP3 Player Shield
  initOk = Adafruit_VS1053_FilePlayer::begin();
  //check result, see readme for error codes.
  if (initOk) 
  {
    Serial.println(F(" MP3 shield initialized."));
    Serial.println(F(" 500ms test beep..."));
    Adafruit_VS1053_FilePlayer::sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  }
  else
  {
    Serial.print(F("Error when initializing MP3 shield. Couldn't find VS1053, do you have the right pins defined?"));
  }

  //Initialize the SD card.
  initOk = SD.begin(pinCard_CS);
  if (!initOk) 
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
  initOk=Adafruit_VS1053_FilePlayer::useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  if (!initOk)
    Serial.println(F("MP3_DREQ pin is not an interrupt pin"));

}

// ******************************************************************************
// Cette fonction joue le fichier MP3 demandé (ID sans extension).
// Le fichier MP3 doit être dans le repertoire /Music.
// ******************************************************************************
void MusicPlayer::playTrack(String filename)
{
    boolean initOk;
    char    trackName[30]; // Le chemin complet ne doit pas depasser 30 char

    // Tell the MP3 Shield to play a track
    filename = String ("/Music/"+filename+".mp3");
    filename.toCharArray(trackName,30);
    Serial.println("PlayTrack: "+filename);

    // initOk = Adafruit_VS1053_FilePlayer::startPlayingFile("/track001.mp3");
    initOk = Adafruit_VS1053_FilePlayer::startPlayingFile(trackName);
    /*initOk = Adafruit_VS1053_FilePlayer::currentTrack = SD.open(trackName);
    Adafruit_VS1053_FilePlayer::currentTrack.seek(mp3_ID3Jumper(currentTrack));*/
    
    if (initOk) 
    {
      Serial.println(" Playing "+filename);
      // On lit les tags ID3 du fichier MP3.
      Serial.println(F("Read ID3 tags"));
      this->readID3tags();
      Step=1;   // first step
    }
    else
    { 
      Serial.print(F("Error when trying to play track "));
      Serial.println(filename);
    }
    String T = getTitle();    Serial.println(T);
    String A = getArtist();   Serial.println(A);
    String B = getAlbum();    Serial.println(B);
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
// Récupère le tag TITLE du clip en cours. Taille=30.
// Il faut avoir appelé auparavant: readID3tags()
// *******************************************************************************
String MusicPlayer::getTitle()
{
  char infobuffer[32];
  const byte OFFSET = 3;
  Serial.println("getTitle");

  for (int i=0; i<30;i++) infobuffer[i]=Buffer[i+OFFSET];

  infobuffer[30]=0;
  //Serial.print('"');Serial.print(infobuffer);Serial.println('"');
  // S'il est vide, on renvoie un blanc.
  if (strlen(infobuffer)==0) return " ";
  String retour = String(infobuffer);
  retour.trim();
  return (retour);
}

// *******************************************************************************
// Récupère le tag ARTIST du clip en cours. Taille = 30.
// Il faut avoir appelé auparavant: readID3tags()
// *******************************************************************************
String MusicPlayer::getArtist()
{
  char infobuffer[32];
  const byte OFFSET = 33;

  for (int i=0; i<30;i++) infobuffer[i]=Buffer[i+OFFSET];
  infobuffer[30]=0;

  // S'il est vide, on renvoie un blanc.
  if (strlen(infobuffer)==0) return " ";
  String retour = String(infobuffer);
  retour.trim();
  return (retour);
}

// *******************************************************************************
// Récupère le tag ALBUM du clip en cours. Taille=30.
// Il faut avoir appelé auparavant: readID3tags()
// *******************************************************************************
String MusicPlayer::getAlbum()
{
  char infobuffer[32];
  const byte OFFSET = 63;

  for (int i=0; i<30;i++) infobuffer[i]=Buffer[i+OFFSET];
  infobuffer[30]=0;

  // S'il est vide, on renvoie un blanc.
  if (strlen(infobuffer)==0) return " ";
  String retour = String(infobuffer);
  retour.trim();
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
     Serial.println(F("Files found (name size):"));
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
            Serial.print("\t");
         }
         Serial.print(entry.name());
         if (entry.isDirectory()) 
         {
            // Affiche le contenu des sous-répertoires
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
 *  Lit la zone ID3 du fichier MP3 et la stocke dans un Buffer.
 * ****************************************************************************************************************** 
 * La zone destags ID3 est un espace de 128 octets placés à la fin du fichier. 
 * Les 3 premiers octets commencent par la chaîne « TAG », cela permet de trouver le début des informations par les lecteurs MP3. 
 * Le reste des octets est partagé entre les différents champs d'informations. 
 * Les chaînes de caractères doivent être codées en ISO/CEI 8859-1 (alphabet latin). 
 * En partant du début de la structure:
 * Offset   Taille (octets)   Description
      0        3              Identifiant "TAG"
      3       30              Titre de la chanson
     33       30              Nom de l'interprète
     63       30              Nom de l'album
     93        4              Année de parution
     97       30              Commentaire sur la chanson
    127        1              Genre musical (code sur 1 octet)
 * ****************************************************************************************************************** 
 * Note: this suspends currently playing streams and returns afterwards.
 * Restoring the file position to where it left off, before resuming.
 * ****************************************************************************************************************** */
void MusicPlayer::readID3tags()
{
  unsigned long currentPos;
  unsigned long fileSize;
  // strcpy (Buffer,"TAGRailroad boy  - - - - - - - Joan Baez x x x x x x x x x In Concert X X X X X X X X X 1964Commentaires..............1");
  //                "TAGVariable white noise          David de lorenzo              R⸮alisation & Software        2020                              2"


  //disable interupts
  // if(playingMusic) disableRefill();
  
  // save the current position in the file
  currentPos = Adafruit_VS1053_FilePlayer::currentTrack.position();
  fileSize = Adafruit_VS1053_FilePlayer::currentTrack.size();
  Serial.print("fileSize ");  Serial.println(fileSize);  // Noise.mp3 = (118 634 bytes)
  Serial.print("currentPos ");  Serial.println(currentPos);
  // skip to 128 bytes before the end
  currentTrack.seek(fileSize-128);

  // Read 128 bytes of tag information
  currentTrack.read(Buffer, 128);
  // Display ID3 tag values:
  {
     Serial.println("ID3 tag:"); 
     for (int i=0; i<127;i++){  Serial.print(Buffer[i]);}
     Serial.print('[');Serial.print(int(Buffer[127]));Serial.print(']'); // le dernier octet est un code de Genre
     Serial.println();
  }
  // On supprime les éventuels caratères non-latins
  *Buffer = strip_nonalpha_inplace(*Buffer);

  //seek back to saved file position
  currentTrack.seek(currentPos);

  //renable interupt
  // if(playingMusic) enableRefill();
  
}

/* ********************************************************
 * Chomp non printable characters out of the string. 
 * (from VLSI library)
 * ******************************************************** */
char* MusicPlayer::strip_nonalpha_inplace(char *s) 
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
 
