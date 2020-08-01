// *******************************************************************************
// REMOTE DISPLAY
//
// Cet objet envoie des commandes à un écran TFT via un bus I2C.
// *******************************************************************************
#ifndef REMOTEDISPLAY_H_INCLUDED
#define REMOTEDISPLAY_H_INCLUDED

class RemoteDisplay 
{
  public:
    RemoteDisplay() {};
    void initialize();
   
    void clearText();
    void clearScreen();
    
    void printYear(int value);
    void printBeat(String texte);
    void printTitle(String texte);
    void printGenre(String texte);
    void printStars(String texte);
    void printArtist(String texte);
    
    void setHeader(String texte);
    void addHeader();
    
    void startScrolling();
    void stopScrolling();
    void scroll();

    void startTimer3();        // expérimentation des ISR
    void stopTimer3();         // expérimentation des ISR
    void printPictoMute() {};  // Radial-V
    void printPictoFM()   {};  // Radial-V
    char requestStatus()  {};  // Radial-V
    void printAlbum(String texte) {};  // Radial-V
    void setBackgroundImage()     {};  // Radial-V

    bool titleScrolling=false;
    
  private:
    char header[24];
    byte headerPos=0;
    // Gestion du titre déroulant
    bool scrollableTitle=false;
    byte titlePos = 0;                    // son vrai type est : u8g2_uint_t
    byte titleLen = 0;
    byte firstVisibleChar = 0;
    char titleText[64];
    char titleSubText[16];
    const byte char_width = 12;           // pour la police ProFont-22, les caractères ont une largeur de 12 pixels

   
};

#endif // REMOTEDISPLAY_H_INCLUDED
