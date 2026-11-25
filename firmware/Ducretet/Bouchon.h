

#ifndef BOUCHON_H_INCLUDED
#define BOUCHON_H_INCLUDED

class Bouchon
{
    public:
        // ------------------------- 
        // Constructeurs
        // ------------------------- 
        Bouchon(byte dummy=0) {};
        Bouchon(byte p1, byte p2) {};
        Bouchon(byte p1, byte p2, byte p3, byte p4) {};
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
        void  printTitle(String texte)  {Serial.println("'"+texte+"'");};
        void  printArtist(String texte) {Serial.println("'"+texte+"'");};
        void  printAlbum(String texte)  {Serial.println("'"+texte+"'");};
        void  printYear(String texte)   {Serial.println("'"+texte+"'");};
        void  printBeat(String texte)   {Serial.println("'"+texte+"'");};
        void  printGenre(String texte)  {Serial.println("'"+texte+"'");};
        void  printLog(String texte)    {Serial.println("'"+texte+"'");};
        void  printStars(String texte)  {Serial.println("'"+texte+"'");};
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
        // ------------------------- 
        // Bouchon Rotary Button
        // ------------------------- 
        int    readPosition()           {return 1;}
        int    getValue()               {return 1;}
        int    getLastValue()           {return 1;}
        bool   hasChanged()             {return false;}
        // ------------------------- 
        // Bouchon Capacitor Button
        // ------------------------- 
        int    readValue(bool debug = false) {return 1;};
        void   dischargeCapacitor()          {};
        // ------------------------- 
        // Bouchon Catalog
        // ------------------------- 
        void   setNewRequestedValues(int t)   {Plexi.setTuning(200);};
        void   takeClip()                     {};
        void   removeStar()                   {};
        void   addStar()                      {};
        void   initSearchForRequestedYear()   {};
        void   initSearchForRequestedBeat()   {};
        void   initSearchForRequestedGenre()  {};
        void   initSearchForRequestedRating() {};
        void   selectRandomClip()             {};
        void   searchClipForRequestedYear()   {};
        void   searchClipForRequestedBeat()   {};
        void   searchClipForRequestedGenre()  {};
        void   searchClipForRequestedRating() {};
        String getSelectedClipID()            {return "DUMMY";};
        int    getSelectedClipYear()          {return (2020);};
        int    getSelectedClipBeat()          {return (120);};
        String getSelectedClipGenre()         {return "Chanson";};
        String getSelectedClipRating()        {return "5";};
        Plexiglass    Plexi;
        // ------------------------- 
        // Bouchon Rythmic
        // ------------------------- 
        void setFrequency(int frequency)      {};
        void startBeat(int beat)              {};
        void stopBeat()                       {};
    
                
    private:
        uint16_t  MonoMode=0;
        uint16_t  DifferentialOutput=0;
        uint8_t   EarSpeaker=0;
        int       Playing=300-2;
        int       Step=1;
};

#endif // BOUCHON_H_INCLUDED
