

#ifndef BOUCHONCLASS_H_INCLUDED
#define BOUCHONCLASS_H_INCLUDED

class Bouchon
{
    public:
        Bouchon(byte dummy=0) {};
        // ------------------------- 
        // Bouchon pour Sparfun MP3 player
        // ------------------------- 
        uint8_t  begin()                                        {return 0;};
        bool     begin(int dummy1, const uint8_t& dummy2)       {return true;};
        uint8_t  playMP3(char* dummy)                           {Playing=0; return 0;};
        bool     isPlaying()                                    {Playing++; return (Playing!=300);};   // 300 = 1 minute
        void     setVolume(int dummy1, int dummy2)              {};
        void     vs_init()                                      {};
        void     trackTitle(char*  dummy)                       {};
        void     trackArtist(char* dummy)                       {};
        void     trackAlbum(char*  dummy)                       {};
        uint16_t getDifferentialOutput()                        {return DifferentialOutput;};
        void     setDifferentialOutput(uint16_t dummy)          {DifferentialOutput=dummy;};
        void     setMonoMode(uint16_t dummy)                    {MonoMode=dummy;};
        uint16_t getMonoMode()                                  {return MonoMode;};
        void     skipTo(int dummy)                              {};
        uint8_t  getEarSpeaker()                                {return EarSpeaker;};
        void     setEarSpeaker(int mode=0)                      {EarSpeaker=mode;};
        // ------------------------- 
        // Bouchon pour SDfat
        // ------------------------- 
        int      getState()               {return 0;};       // renvoyer le code qui correspond à ready
        void     initErrorHalt()          {Serial.println("Bouchon::initErrorHalt");};
        bool     chdir(char* dummy)       {return true;};
        void     errorHalt(char* dummy)   {};
        void     ls(int dummy)            {};
        // ------------------------- 
        // Bouchon pour RemoteTFT
        // ------------------------- 
        // void initI2C();
        // void setBackgroundImage();
        // void printSplitedTitle(String texte);
        // void clearPicto();
        // char requestStatus();
        void  clearBackground()         {};
        void  printTitle(String texte)  {};
        void  printArtist(String texte) {};
        void  printAlbum(String texte)  {};
        void  printYear(String texte)   {};
        void  printGenre(String texte)  {};
        void  printLog(String texte)    {};
        void  printStars(String texte)  {};
        void  clearAllText()            {};
        void  setBacklight(bool on)     {};
        // ------------------------- 
        // Bouchon Media Player
        // ------------------------- 
        void   initialize()               {};
        void   pauseDataStream()          {};
        void   resumeDataStream()         {};
        void   setDiffmode()              {};
        void   playTrack(String filename) {};
        void   stopTrack()                {Playing=0;};
        void   restartTrack()             {Playing=1;};
        int    getStep()                  {return Step;};
        String getTitle()                 {return "Title";};
        String getArtist()                {return "Artist";};
        String getAlbum()                 {return "Album";};

        
        private:
        uint16_t  MonoMode=0;
        uint16_t  DifferentialOutput=0;
        uint8_t   EarSpeaker=0;
        int       Playing=300-2;
        int       Step=1;
};

#endif // BOUCHONCLASS_H_INCLUDED
