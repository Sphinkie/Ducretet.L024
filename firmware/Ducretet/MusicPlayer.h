/* ***********************************************************
 *  Gestion de la carte MP3 player
 *  
 *  Utilise aussi le SPI
 ************************************************************* */
#ifndef MUSICPLAYERCLASS_H_INCLUDED
#define MUSICPLAYERCLASS_H_INCLUDED

#include "Adafruit_VS1053.h"

#define  MAX_STEP  6000    // Nombre d'étapes gérées par le mini-séquenceur

class MusicPlayer : public Adafruit_VS1053_FilePlayer
{
    public:
        MusicPlayer(byte pinMP3_CS, byte pinMP3_DCS, byte pinMP3_DREQ, byte pinSD_CS);
        void   initialize();
        void   playTrack(String filename);
        void   setVolume(int volume);
        bool   isPlaying();
        void   stopTrack();
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
        void   readID3tags(File mp3file);
        char*  strip_nonalpha_inplace(char *s);

    // Private Member Variables:
        int    Step;
        byte   pinCard_CS;
        char   filename[30];        // Chemin complet du clip (ne doit pas depasser 30 char)
        char   Buffer[128] = {0};   // Buffer to contain the extracted Tag from the current MP3 file. Inizialized with zeros.
};


#endif // MUSICPLAYERCLASS_H_INCLUDED
