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

// #define DEBUG

// *********************************************************************************
// Constructor : aussi  Adafruit_VS1053_FilePlayer
// On précise pas les Pins du bus SPI, car on passe par le connecteur standard ICSP
// Appel standard = Adafruit_VS1053_FilePlayer(-1, 7, 6, 3, 4)
// *********************************************************************************
MusicPlayer::MusicPlayer(byte pinMP3_CS, byte pinMP3_DCS, byte pinMP3_DREQ, byte pinSD_CS) : Adafruit_VS1053_FilePlayer(-1, pinMP3_CS, pinMP3_DCS, pinMP3_DREQ, pinSD_CS)
{
    pinCard_CS = pinSD_CS;  // On le mémorise pour plus tard
    Step       = 0;         // no step

  /* Avec la carte Sparkfun, on a souvent l'erreur "Can't access SD card. Do not reformat. No card, wrong chip select pin, or SPI problem."
   * La solution qui semble marcher est de mettre les pins SS à HIGH (inactif) dès le début. 
   * Sinon les lecteurs SD peuvent recevoir des parasites sur la ligne SPI, et se bloquer...
   */
  pinMode(pinSD_CS, OUTPUT);  digitalWrite(pinSD_CS, HIGH);   
  pinMode(53, OUTPUT);        digitalWrite(53, HIGH);          // SPI_SS
}


// ******************************************************************************
// Initialise la carte MP3 et la carte SD incluse.
// ******************************************************************************
void MusicPlayer::initialize()
{
  boolean initOk;
  Serial.println(F("MP3+SD initialization."));

  // Initialize the MP3 Player Shield
  initOk = Adafruit_VS1053_FilePlayer::begin();
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
  // printDirectory();

  // Set volume for left, right channels. lower numbers == louder volume!
  Adafruit_VS1053_FilePlayer::setVolume(20,20);

  // Using MP3_DREQ interrupt pin, we can do background audio playing.
  initOk=Adafruit_VS1053_FilePlayer::useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  if (!initOk)
    Serial.println(F("MP3_DREQ pin is not an interrupt pin"));
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
// Cette fonction joue le fichier MP3 demandé (ID sans extension).
// Le fichier MP3 doit être dans le repertoire /Music.
// ******************************************************************************
void MusicPlayer::playTrack(String trackName)
{
    boolean initOk;

    // On convertit de String en char[30], avec chemin complet et extension.
    trackName = String ("/Music/"+trackName+".mp3");
    trackName.toCharArray(this->filename,30);

    // on lit les Tags avant de jouer le fichier
    File mp3file = SD.open(this->filename);
    this->readID3tags(mp3file);
    mp3file.close();

    // Tell the MP3 Shield to play a track
    initOk = Adafruit_VS1053_FilePlayer::startPlayingFile(this->filename);
    
    if (initOk) 
    {
      Serial.println(" Playing "+trackName);
      Step=1;   // first step
    }
    else
    { 
      Serial.println("Error when trying to play track "+trackName);
      Step=0;
    }
}


// ******************************************************************************
// Stoppe la lecture du fichier mp3
// ******************************************************************************
void MusicPlayer::stopTrack()
{
   // Stop the current track
   Serial.println(F("  Stopping mp3"));
   Adafruit_VS1053_FilePlayer::stopPlaying();
   memset(Buffer, '\0', 128);    // On vide le buffer des tags ID3
   Step=0;                       // step number
}

// ******************************************************************************
// Recommence la lecture du fichier mp3 à partir de 0
// ******************************************************************************
void MusicPlayer::restartTrack()
{
   // Restart the current track
   Serial.println(F("  Restarting mp3"));
   Adafruit_VS1053_FilePlayer::stopPlaying();
   Adafruit_VS1053_FilePlayer::startPlayingFile(this->filename);
   Step=0;   // step number
}


// ******************************************************************************
// Renvoie TRUE si la carte est en train de jouer. FALSE sinon.
// ******************************************************************************
bool MusicPlayer::isPlaying()
{
   return (Adafruit_VS1053_FilePlayer::playingMusic);
}


// *******************************************************************************
// Récupère le tag TITLE du clip en cours. Taille=30.
// Il faut avoir appelé auparavant: readID3tags()
// *******************************************************************************
String MusicPlayer::getTitle()
{
  char infobuffer[32];
  const byte OFFSET = 3;

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
// Un petit séquenceur d'étapes (de 1 à MAX_STEP). 0 si inactif.
// Il part de 1 chaque fois qu'un nouveau clip commence à être joué (etat transitoire).
// ******************************************************************************
int MusicPlayer::getStep()
{
   int CurrentStep = Step;
   if (CurrentStep > 0) Step++;
   if (Step > MAX_STEP) Step=0;
#ifdef DEBUG   
   if (CurrentStep!=0) {Serial.print(F("Step ")); Serial.print(CurrentStep); Serial.print(F("-->")); Serial.println(Step);}
#endif
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
  noInterrupts();
  // Adafruit_VS1053_FilePlayer::feedBufferLock=false;
}

// ******************************************************************************
// Enable interrupts.
// Note: you can not stop the data stream to the MP3 Shield for too long 
//       before it runs out of data so be careful and try not to do too much stuff in between the functions.
// ******************************************************************************
void MusicPlayer::resumeDataStream()
{
  interrupts();
  // Adafruit_VS1053_FilePlayer::feedBufferLock=true;
}


/* *****************************************************************************************************************
 *  Lit la zone ID3v1 du fichier MP3 et la stocke dans un Buffer.
 *  l'alimentation du stream audio est inhibé pendant cette phase.
 * ****************************************************************************************************************** 
 * https://id3.org/id3v1
 * https://id3.org/d3v2.3.0
 * ****************************************************************************************************************** 
 * La zone des tags ID3v1 est un espace de 128 octets placés à la fin du fichier. 
 * Les 3 premiers octets commencent par la chaîne « TAG ». 
 * Le reste des octets constitue des champs d'informations de taille fixe. 
 * Les chaînes de caractères doivent être codées en ISO/CEI 8859-1 (alphabet latin). 
 * Offset   Taille (octets)   Description
      0        3              Identifiant "TAG"
      3       30              Titre de la chanson
     33       30              Nom de l'interprète
     63       30              Nom de l'album
     93        4              Année de parution
     97       30              Commentaire sur la chanson
    127        1              Genre musical (code sur 1 octet)
 * ****************************************************************************************************************** */
void MusicPlayer::readID3tags(File mp3file)
{
  unsigned long currentPos;
  unsigned long fileSize;
  // "TAGRailroad Boy                  Joan Baez                     In Concert                    1961                             [12]"
  // "TAGVariable white noise          David de lorenzo              R⸮alisation & Software        2020                              [39]"
  Serial.println(F("Read ID3v1 tags"));

  // Disable interrupts
  if (Adafruit_VS1053_FilePlayer::playingMusic) noInterrupts();
  
  // Save the current position in the file
  currentPos = mp3file.position();
  fileSize   = mp3file.size();
  mp3file.seek(fileSize-128);    // Go to 128 bytes before the end
  mp3file.read(Buffer, 128);     // Read 128 bytes of tag information
#ifdef DEBUG
  {
     // Display ID3 tag values:
     Serial.println("ID3 tag:"); 
     for (int i=0; i<127;i++){  Serial.print(Buffer[i]);}
     Serial.print('[');Serial.print(int(Buffer[127]));Serial.print(']'); // le dernier octet est un code de Genre
     Serial.println();
     for (int i=0; i<128;i++){  Serial.print(int(Buffer[i]),HEX); ;Serial.print(' '); }
     Serial.println();
  }
#endif

  // Restore previous saved file position
  mp3file.seek(currentPos);
  // Re-enable interrupts
  if (Adafruit_VS1053_FilePlayer::playingMusic) interrupts();
  // Si on a des tags ID3v1 présents dans le MP3
  if (strncmp(Buffer,"TAG",3))
      // Si différent, on remet le Buffer à 0x00
      memset(Buffer, '\0', 128);
  else
      // Si identique, on supprime les éventuels caractères non-latins
      *Buffer = strip_nonalpha_inplace(*Buffer);

  
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
            Serial.println(F("  **skipping subdir content**"));
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
  dir.close();
}
