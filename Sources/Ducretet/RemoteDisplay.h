// *******************************************************************************
// REMOTE DISPLAY
//
// Cet objet envoie des commandes à un écran TFT via un bus I2C.
// Il y a un autre Arduino (slave) sur le bus I2C pour recevoir ces commandes
// *******************************************************************************
#ifndef REMOTEDISPLAY_H_INCLUDED
#define REMOTEDISPLAY_H_INCLUDED




class RemoteDisplay
{

  public:
    RemoteDisplay();
    void initialize();
   
    void clearScreen();
    void setBackgroundImage();
    
    void printTitle(String texte);
    void printArtist(String texte);
    void printAlbum(String texte);
    void printYear(int value);
    void printGenre(String texte);
    void printBeat(String texte);
    void printHeader(String texte);
    void printStars(String texte);
    
    void printPictoMute();
    void printPictoFM();
    void clearPicto();
    char requestStatus();
    
  private:
    void printFramedText(String texte);
    
    bool I2Cconnected;
    const byte MAX_LINE_LEN = 12;
    
    // ------------------------------------
    // Les interlocuteurs du bus I2C
    // ------------------------------------


};


#endif // REMOTEDISPLAY_H_INCLUDED
