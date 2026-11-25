/** *******************************************************************************
 * REMOTE DISPLAY
 *
 * Cet objet envoie des commandes à un écran TFT via un bus I2C.
 * ********************************************************************************/
#ifndef REMOTEDISPLAY_H_INCLUDED
#define REMOTEDISPLAY_H_INCLUDED

class RemoteDisplay 
{
  public:
    RemoteDisplay() {};
    void initialize();
   
    void clearText();
    void clearScreen();
    
    void printLogo();
    void printYear(int value);
    void printBeat(String texte);
    void printMode(String texte);
    void printGenre(String texte);
    void printArtist(String texte);
    void printTitle(String texte);
    void printAnimatedTitle(String texte);
    void startTitleAnimation();
    void animTitle();
    void printStars(int stars);
    void startStarAnimation(int stars);
    void animStars();
    
    void setHeader(String texte);
    
    void startScrolling();
    void stopScrolling();
    void scroll();

    void startTimer3();         // expérimentation des ISR
    void stopTimer3();          // expérimentation des ISR
    void printPictoMute() {};   // Radial-V
    void printPictoFM()   {};   // Radial-V
    char requestStatus()  {};   // Radial-V
    void printAlbum(String texte) {};  // Radial-V
    void setBackgroundImage()     {};  // Radial-V

    bool TitleScrolling=false;
    bool TitleAnim=false;
    byte StarAnim=0;      // 0 = stopped 
    
  private:
    void addFrame();
    void addHeader();
    void printFramedText(String to_print);
    void drawDualLineText(String texte, byte len);
    void drawStar(int pos, float a);
    void cutTitleString();
       
    // Gestion des mots animés
    String TitleCDR;
    char WordText[20];
    byte WordX, WordStep, WordWidth;
    // Gestion du Header
    char header[24];
    byte headerPos=0;
    // Gestion de l'animation des stars
    byte max_animated_star = 0;
    byte current_animated_star = 1;
    float current_star_size = 0.5;
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
