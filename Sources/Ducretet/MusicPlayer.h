/* ***********************************************************
 *  Gestion de la carte MP3 player
 *  
 *  Utilise les Pins:
 *    50-51-52 pour le SPI
 *    53 du SPI est réservé
 *    Digital = 2-3-4 6-7-8-9
 *    
 * Note: Les tags Titre/Artist/Album dans le MP3 sont limités à 128.
 *       Au dela, le programme peut se planter (cf Buffer ci-dessous) 
 ************************************************************* */
#ifndef MUSICPLAYERCLASS_H_INCLUDED
#define MUSICPLAYERCLASS_H_INCLUDED

#include "Adafruit_VS1053.h"

/*
 * ID3 Tag location offsets
 * warning This may not be available on all source music files.
 */
// The offset from the begining of the ID3 tag for the location containing the track's Title of the mp3 file being read from the SdCard.
// The offset from the begining of the ID3 tag for the location containing the track's Artist of the mp3 file being read from the SdCard.
// The offset from the begining of the ID3 tag for the location containing the track's Album of the mp3 file being read from the SdCard.
#define TRACK_TITLE              3
#define TRACK_ARTIST            33
#define TRACK_ALBUM             63


class MusicPlayer : public Adafruit_VS1053_FilePlayer
{
    public:
        MusicPlayer(byte pinMP3_RESET, byte pinMP3_CS, byte pinMP3_DCS, byte pinMP3_DREQ, byte pinSD_CS);
        void   initialize();
        void   playTrack(String filename);
        void   setVolume(int volume);
        bool   isPlaying();
        void   stopTrack();
        void   displayMediaInfo();
        void   restartTrack();
        void   resetBoard();
        void   printDirectory();
        int    getStep();
        String getTitle();
        String getArtist();
        String getAlbum();
        void   pauseDataStream();
        void   resumeDataStream();

    private:
        void   getTrackInfo(uint8_t offset, char* infobuffer);
        void   readID3tags();

    // Private Member Variables:
        int  Step;
        byte pinCard_CS;
        char Buffer[128];   // Buffer to contain the extracted Tag from the current MP3 file

        typedef struct {
          char  tag[3];   // "TAG"
          char  title[30];
          char  artist[30];
          char  album[30];
          char  year[4];
          char  comment[30];
          char  genre;
          }ID3tag;
        
};

    // ---------------------------------------------------------------------------------
    // Pour l'affichage des messages d'erreur (selon la fonction appelante)
    // ---------------------------------------------------------------------------------
    #define  BEGIN 100
    #define  PLAY  200
    #define  SKIP  300

    // ---------------------------------------------------------------------------------
    #define  MIN_VOLUME 66    // Volume bas (-40 db) pour MP3.SetVolume 
    #define  MAX_VOLUME 0     // Volume fort  (0 db) pour MP3.SetVolume 
    #define  MAX_STEP  20     // Nombre d'étapes gérées par le mini-séquenceur

#endif // MUSICPLAYERCLASS_H_INCLUDED
