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

class MusicPlayer
{
    public:
        MusicPlayer(byte pinSD_CS);
        void   initialize();
        void   playTrack(String filename);
        void   setVolume(int volume);
        bool   isPlaying();
        void   stopTrack();
        void   displayMediaInfo();
        void   restartTrack();
        void   resetBoard();
        // void   changeSurroundMode();
        // void   setDiffmode();
        void   setStereo(bool ON);
        void   printStatus();
        void   dir();
        int    getStep();
        String getTitle();
        String getArtist();
        String getAlbum();
        void   pauseDataStream();
        void   resumeDataStream();

    private:
        String getErrorMsg(int errCode);

    // variables:
        int  Step;
        char Buffer[128];   // buffer to contain the extracted Tag from the current MP3 filehandle
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
