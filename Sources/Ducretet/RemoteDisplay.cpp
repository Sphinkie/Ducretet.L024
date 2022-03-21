/* *******************************************************************************
 * REMOTE DISPLAY
 * Ecran 128x64
 * doc de référence : https://github.com/olikraus/u8g2/wiki/u8g2reference
 * ******************************************************************************* */
#include "Arduino.h"
#include "RemoteDisplay.h"
#include <U8g2lib.h>
#include <Wire.h>


/* *******************************************************************************
 * Creation de l'objet u8g2 avec les paramètres correspondants à un écran avec chipset SH1106.
 *   U8G2_R0           = No rotation, landscape
 *   Use U8X8_PIN_NONE = there is no switch button connected to the Display pins
 *   _F_               = Full buffer mode
 * ******************************************************************************* */
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2 (U8G2_R0, U8X8_PIN_NONE);


/** *******************************************************************************
 * Initialisation de l'afficheur.
 * ********************************************************************************/
void RemoteDisplay::initialize()
{
  Serial.println("OLED initialize");
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();   // Ascent will be the largest ascent of "A", "1" or "(" of the current font. Descent will be the descent of "g" or "(" of the current font.
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();         // Change the reference position for the glyph and string draw functions.
  u8g2.setFontDirection(0);     // horizontal
}

/** *******************************************************************************
 * Efface l'écran.
 * ********************************************************************************/
void RemoteDisplay::clearScreen()
{
   u8g2.clearBuffer();
   u8g2.sendBuffer();
}

/** *******************************************************************************
 * Efface l'écran, mais laisse le Header visible.
 **********************************************************************************/
void RemoteDisplay::clearText()
{
   u8g2.clearBuffer();
   this->addHeader();
   u8g2.sendBuffer();
}

/** *******************************************************************************
 * Affiche le SplashScreeen.
 * Ecran = 128x64.
 * Epaisseur des traits = 3 pixels.
 * Position 0,0 = Upper-Left.
 * ********************************************************************************/
 void RemoteDisplay::printLogo()
{
  Serial.println("printLogo");
  // u8g2.clearBuffer();
  // Cercle: x0,y0,R
  u8g2.drawCircle(64, 32, 25);
  u8g2.drawCircle(64, 32, 24);
  u8g2.drawCircle(64, 32, 23);
  // Pied: x0, y0, L
  #define PIVOT 44    
  u8g2.drawVLine(64,   PIVOT, 20);
  u8g2.drawVLine(64-1, PIVOT, 20);
  u8g2.drawVLine(64+1, PIVOT, 20);
  // Raccord: x0,y0,R
  #define RADIUS 5    // Rayon du raccord
  u8g2.drawCircle(64, PIVOT-RADIUS, RADIUS,   U8G2_DRAW_LOWER_RIGHT | U8G2_DRAW_LOWER_LEFT);
  u8g2.drawCircle(64, PIVOT-RADIUS, RADIUS+1, U8G2_DRAW_LOWER_RIGHT | U8G2_DRAW_LOWER_LEFT);
  u8g2.drawCircle(64, PIVOT-RADIUS, RADIUS+2, U8G2_DRAW_LOWER_RIGHT | U8G2_DRAW_LOWER_LEFT);
  // Branche gauche: x0, y0, x1, y1
  u8g2.drawLine(64-RADIUS,   PIVOT-RADIUS, 64-3, 0);
  u8g2.drawLine(64-RADIUS-1, PIVOT-RADIUS, 64-4, 0);
  u8g2.drawLine(64-RADIUS-2, PIVOT-RADIUS, 64-5, 0);
  // Branche droite
  u8g2.drawLine(64+RADIUS,   PIVOT-RADIUS, 64+3, 0);
  u8g2.drawLine(64+RADIUS+1, PIVOT-RADIUS, 64+4, 0);
  u8g2.drawLine(64+RADIUS+2, PIVOT-RADIUS, 64+5, 0);

  u8g2.sendBuffer(); 
}

/** *******************************************************************************
 * Prepare l'animation du titre.
 * @post startTitleAnimation() doit être appelé après.
 * @param titleString : Le titre à animer
 * ********************************************************************************/
void RemoteDisplay::printAnimatedTitle(String titleString)
{
   if (titleString=="NOISE") 
      this->TitleCDR = "";
   else
      this->TitleCDR = titleString;
}

/** *******************************************************************************
 * On prépare un mot à animer.
 * En raison de l'UTF-8, certaines lettres prennent 2 bytes...
 * D'où un CharArray de [16] pour stocker une String de [10].
 * @pre printAnimatedTitle() doit être appelé avant.
 * @pre animTitle() doit être appelé après (plusieurs fois)
 * ********************************************************************************/
void RemoteDisplay::startTitleAnimation()
{
   u8g2_uint_t width;               // Largeur du texte (en pixels)

   // S'il n'y a rien a afficher, on sort
   if (this->TitleCDR.length()==0) return;
   // On extrait le mot à animer
   this->cutTitleString();
   // On determine la position du texte animé
   u8g2.setFont(u8g2_font_profont22_tf);
   width = u8g2.getUTF8Width(this->WordText);
   this->WordX = (128 - width) / 2;
   this->WordWidth = width;
   // On active l'animation
   this->WordStep = 0;
   this->TitleAnim = true;
}

/** *******************************************************************************
 *  Coupe le titre CDR en deux: 
 *    WordText = le mot à animer
 *    TitleCDR = le texte restant
 * ********************************************************************************/
void RemoteDisplay::cutTitleString()
{
   const byte MAX_LINE_LEN = 10;    // Nb max de charactères par ligne en u8g2_font_profont22_tf = 10.6

   byte longeurTexte = this->TitleCDR.length();   // Nombre de caractères restants
   // Si le texte (restant) est court, il tient sur une ligne.
   if (longeurTexte < MAX_LINE_LEN)
   {
      this->TitleCDR.toCharArray(this->WordText, MAX_LINE_LEN);
      this->TitleCDR = "";    // il ne reste rien à afficher
   }
   else
   {
      // Si le texte est long, on prend le premier (groupe de) mot(s) de moins de 10 chars.
      byte cissure = longeurTexte;
      // On cherche le dernier espace inférent à la longueur de la ligne
      while (cissure>MAX_LINE_LEN) cissure = TitleCDR.lastIndexOf(' ',cissure-1);
      // On a trouvé le premier mot
      this->TitleCDR.toCharArray(this->WordText, cissure+1);
      // On l'enlève de la string restant à afficher.
      this->TitleCDR.remove(0, cissure+1);
   }  
}

/** *******************************************************************************
 * Animation d'un mot du titre.
 * @pre startTitleAnimation() doit être appelé avant.
 * ********************************************************************************/
void RemoteDisplay::animTitle()
{
  switch (this->WordStep)
  {
    case 0:
            // Step 0: on trace une ligne fine.
            u8g2.clearBuffer();
            u8g2.drawHLine(this->WordX, 40, this->WordWidth);
            this->addHeader();
            this->WordStep++;
            break;
    case 1:
            // On épaissit la ligne.
            u8g2.drawHLine(this->WordX, 40-1, this->WordWidth);
            u8g2.drawHLine(this->WordX, 40+1, this->WordWidth);
            this->WordStep++;
            break;
    case 2:
            // On épaissit la ligne.
            u8g2.drawHLine(this->WordX, 40-2, this->WordWidth);
            u8g2.drawHLine(this->WordX, 40+2, this->WordWidth);
            this->WordStep++;
            break;
    case 3:
            // Step final: on écrit le mot.
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_profont22_tf);
            u8g2.drawUTF8(this->WordX, 40-10, this->WordText);
            this->WordStep++;
            this->addHeader();
            break;
    case 4:
            // On stoppe l'animation
            this->TitleAnim = false;
            this->WordStep = 0;
            break;
  }  
  u8g2.sendBuffer(); 
}

/** *******************************************************************************
 * Affiche le beat dans le cadre.
 * @param texte : Le texte à afficher. Ex: "116 bpm".
 * ********************************************************************************/
void RemoteDisplay::printBeat(String texte)
{
  this->printFramedText(texte + " bpm");
}

/** *******************************************************************************
 * Affiche le Genre de musique dans le cadre.
 * @param texte : Le texte à afficher
 * ********************************************************************************/
void RemoteDisplay::printGenre(String texte)
{
  this->printFramedText(texte);
}

/** *******************************************************************************
 * Affiche le titre sur une ou deux lignes (sans cadre).
 * A noter que dans les tags MP3, la taille du Titre est limitée à 30 chars.
 * \n\b u8g2_font_profont17_tf   : 14 char par ligne
 * @param texte : Le texte à afficher
 * ********************************************************************************/
void RemoteDisplay::printTitle(String texte)
{
   if (texte=="NOISE") return;
   byte longeurTexte = texte.length();  // Nombre de caractères
   
   // Si le texte est court, on ne touche pas à l'affichage.
   if (longeurTexte >= 14)
   {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_profont17_tf);
      // u8g2.setFontMode(1);
      this->drawDualLineText(texte, 14);
      this->addHeader();
      u8g2.sendBuffer();
   }
}

/** *******************************************************************************
 * Affiche le mode de recherche dans un rectangle blanc.
 * \n\b u8g2_font_koleeko_tf   : width 12 / Height 14 : petite police avec un look SF / Art-Deco
 * @param texte : Le texte à afficher
 * ********************************************************************************/
void RemoteDisplay::printMode(String texte)
{
   u8g2.setFont(u8g2_font_koleeko_tf);
   u8g2.setFontMode(1);   // 0:pour police _mx (faster) / 1:pour police _tx
   u8g2.clearBuffer();
   u8g2.drawRBox(0, 10, 128, 44, 7);   // x0, y0, w, h, r
   u8g2.setDrawColor(0);
   this->drawDualLineText(texte, 16);
   u8g2.setDrawColor(1);
   u8g2.sendBuffer();
}

/** ******************************************************************************
 * Affiche l'Artiste sur une ou deux lignes
 * \n\b u8g2_font_helvR12_tf : police Helvetica fine et lisible
 * \n\b u8g2_font_helvR10_tf : police Helvetica fine et lisible (16 char par ligne)
 * @param texte : Le texte à afficher
 * ********************************************************************************/
void RemoteDisplay::printArtist(String texte)
{
   u8g2.setFont(u8g2_font_helvR10_tf);
   u8g2.setFontMode(1);                // 0:pour police _mx (faster) / 1:pour police _tx
   u8g2.clearBuffer();
   this->drawDualLineText(texte, 16);
   this->addFrame();
   this->addHeader();
   u8g2.sendBuffer();
}

/** *******************************************************************************
 * Affiche un texte sur une ou deux lignes.
 * La police doit être positionnée avant l'appel, pour mesurer la largeur du texte.
 * @param texte : Texte à afficher
 * @param len   : Nb max de charactères dans le rectangle (par ligne)
 * ********************************************************************************/
void RemoteDisplay::drawDualLineText(String texte, byte len)
{
   char line1[20];
   char line2[20];
   byte longeurTexte = texte.length();

   // Si le texte est court, il tient sur une ligne.
   if (longeurTexte < len)
   {
        texte.toCharArray(line1, len);
        strcpy(line2, " ");
   }
   // Sinon, le texte tient sur deux lignes: on cherche un espace où couper.
   else
   {
      byte cissure = longeurTexte;
      while (cissure>len) cissure = texte.lastIndexOf(' ',cissure-1);
      texte.toCharArray(line1, cissure+1);   // [0..cissure]
      texte.substring(cissure+1).toCharArray(line2, len);
   }
   // On centre les textes
   byte pos1 = (128 - u8g2.getUTF8Width(line1)) / 2;
   byte pos2 = (128 - u8g2.getUTF8Width(line2)) / 2;
   u8g2.drawStr(pos1, 26, line1);
   u8g2.drawStr(pos2, 42, line2);
   Serial.print("printDualLines: "); Serial.print(line1); Serial.print("|");  Serial.println(line2);
}

/** ******************************************************************************
 * Affiche l'année (sauf si 0)
 * \n\b u8g2_font_ncenB24_tn      : Possible aussi, mais un peu moins joli.
 * \n\b u8g2_font_osb26_tn        : "Old Standard Bold". Existe en 18 - 21 - 26 - 29
 * @param value : Année à afficher
 * ********************************************************************************/
void RemoteDisplay::printYear(int value)
{
   Serial.println("printYear: " + String(value));
  
   if (value!=0)
   {
      char texte[5];   
      itoa (value, texte, 10);          // base 10
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_osb26_tn);
      u8g2.setFontMode(1);           // 0:pour police _mx (faster) / 1:pour police _tx
      u8g2.drawStr(22, 28, texte);   // Draw the text
      this->addFrame();              // Ajoute le cadre
      this->addHeader();             // Ajoute le bandeau
      u8g2.sendBuffer();
   }
}

/** *******************************************************************************
 * Affiche le texte, dans un cadre, centré sur 1 ligne, et avec le bandeau.
 * Tronqué à 16 chars.
 * \n\b u8g2_font_helvR12_tf         : police Helvetica fine et lisible
 * \n\b u8g2_font_profont22_tf       : assez gros
 * @param to_print : Texte à afficher
 * ********************************************************************************/
void RemoteDisplay::printFramedText(String to_print)
{
   const byte MAX_LINE_LEN = 16;    // Nb max de charactères dans le rectangle (par ligne)
   char texte[32];
   byte textePos;
   
   Serial.println("printFramedText: " + to_print);
   to_print.toCharArray(texte, MAX_LINE_LEN);
   u8g2.setFont(u8g2_font_profont22_tf); 
   u8g2.setFontMode(1);                   // 0:pour police _mx (faster) / 1:pour police _tx
   textePos = (128 - u8g2.getUTF8Width(texte)) / 2;
   u8g2.clearBuffer();
   u8g2.drawStr(textePos, 32, texte);     // Draw the text
   this->addFrame();      // Ajoute le cadre
   this->addHeader();     // Ajoute le bandeau
   u8g2.sendBuffer();
}

/** *******************************************************************************
 * Trace un rectangle aux coins arrondis.
 * ********************************************************************************/
void RemoteDisplay::addFrame()
{
   u8g2.drawRFrame(0, 20, 128, 44, 7);       // xo, yo, lg, h, r
}

/** *******************************************************************************
 * Mémorise le texte à afficher dans le header. Tronque à 17. 
 * @remark Préparer avec la même police que pour l'affichage.
 * ********************************************************************************/
void RemoteDisplay::setHeader(String texte)
{
  texte.toCharArray(this->header, 17);
  u8g2.setFont(u8g2_font_DigitalDisco_tf); 
  this->headerPos = (128 - u8g2.getUTF8Width(this->header)) / 2;
}

/** ******************************************************************************
 * Dessine un header avec le texte prévu avec setHeader(). 
 * Noir sur fond blanc.
 * *******************************************************************************
 *  \n\b u8g2_font_DigitalDisco_tf (17 char on a line) - très lisible (daFont) Lettres rondes.
 *  \n\b u8g2_font_halftone_tf     (18 char on a line) - très discret (grisé sur fond blanc)
 *  \n\b u8g2_font_profont12_tf    (21 char on a line) - trop fin
 *  \n\b u8g2_font_Pixellari_tf     police assez petite
 * ******************************************************************************* */
void RemoteDisplay::addHeader()
{
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setFont(u8g2_font_DigitalDisco_tf); 
  u8g2.setDrawColor(0);
  u8g2.drawStr(this->headerPos, 0, this->header);
  u8g2.setDrawColor(1);
}

/** *******************************************************************************
 * Affiche les étoiles du Rating dans le cadre.
 * ********************************************************************************/
void RemoteDisplay::printStars(int stars)
{
   Serial.println("printStars: " + String(stars));
   u8g2.clearBuffer();
   for (int i = 0; i<stars; i++)
     this->drawStar(i+1, 4.0);
   this->addFrame();      // Ajoute le cadre
   this->addHeader();     // Ajoute le bandeau
   u8g2.sendBuffer();
}

/** *******************************************************************************
 * Initialise l'animations des étoiles de Rating.
 * @param stars : Le nombre d'étoiles à afficher
 * ********************************************************************************/
void RemoteDisplay::startStarAnimation(int stars)
{
   Serial.println("startStarAnimation: " + String(stars));
   current_star_size = 1.0;
   current_animated_star = 1;
   max_animated_star = stars;
   u8g2.clearBuffer();
   this->addFrame();      // Ajoute le cadre
   this->addHeader();     // Ajoute le bandeau
   this->StarAnim=1;
}

/** *******************************************************************************
 * Anime les étoiles du Rating, en les faisant grossir une-à-une.
 * Appeler 20 fois (4 étapes pour chaque étoile).
 * ********************************************************************************/
void RemoteDisplay::animStars()
{
   // Tant qu'on n'a pas atteint la taille maximum, on augmente la taille.
   if (current_star_size<4) 
      current_star_size += 1.0;
   else
   {
      // Sinon, l'étoile a fini de grossir, on passe à la star suivante.
     if (current_animated_star < max_animated_star) current_animated_star += 1;
     current_star_size = 1.0;
   }
   // On trace le sprite
   this->drawStar(current_animated_star, current_star_size);
   u8g2.sendBuffer();
   // Si on a atteint le step 20, on a fini.
   if (++this->StarAnim > 20)
      this->StarAnim=0;
}

/** ****************************************************************************************************
 *  Dessine une étoile à 5 branches.
 *  @param pos : Position de l'étoile (de 1 à 5)
 *  @param a : Taille de l'étoile (longueur en pixels des branches). Taille min=1. Taille max=4.
 * *****************************************************************************************************/
void RemoteDisplay::drawStar(int pos, float a)
{
  int x0 = pos * 24 - 8;
  int y0 = 40;
  float sa  = a * 0.9510;  // sin(2pi/5)
  float ca  = a * 0.3090;  // cos(2pi/5)
  float s2a = a * 0.5878;  // sin(2x2pi/5)
  float c2a = a * 0.8090;  // cos(2x2pi/5)
  // Triangle du haut
  u8g2.drawTriangle(x0 - a, y0,    x0, y0 - 3 * a,         x0 + a, y0);
  // triangles lateraux
  u8g2.drawTriangle(x0, y0 - 3 * ca, x0 + 3 * sa, y0 - 3 * ca,   x0, y0 + a);
  u8g2.drawTriangle(x0, y0 - 3 * ca, x0 - 3 * sa, y0 - 3 * ca,   x0, y0 + a);
  // triangles pieds
  u8g2.drawTriangle(x0 + a, y0,    x0 + 3 * s2a, y0 + 3 * c2a, x0, y0 + a);
  u8g2.drawTriangle(x0 - a, y0,    x0 - 3 * s2a, y0 + 3 * c2a, x0, y0 + a);
}



/** *******************************************************************************
 * Démarre le timer TIMER3
 * *******************************************************************************
 * @details
 * Note sur timer3:
 * \n      compteur 16bits => match register = 65.536 max
 * \n      prescaler max = 1024
 * \n    donc sa frequence minimale = 16M/1024/65536 = 0.238 Hz = 4.19 sec
 * \n Prescaler allowed values: 1,8,64,256,1024
 * *******************************************************************************
 *    cpu frequency       = 16.000.000 (16 MHz)
 * \n diviseur prescaler  = 256
 * \n fréquence souhaitée = 25 fois par seconde
 * *******************************************************************************
 *      Match   25 Hz = 16.000.000/256/25     = 2.500
 * \n   Match   30 Hz = 16.000.000/256/25     = 2.000
 * *******************************************************************************
 * Le timer incremente son compteur à 16M/prescaler = 16M/256 = 62500 Hz.
 * Le timer génère une IT lorsque le compteur atteint la valeur du match register.
 * On calcule donc sa valeur: MatchRegister + 1 = 16M / 256 / DesiredInterruptFrequency
 * (si on veut être précis, le +1 prend en compte la RAZ du compteur).
 * ********************************************************************************/
void  RemoteDisplay::startTimer3()
{
    const float scaled_freq = 16000000 /256 /25;
    uint16_t    match       = uint16_t(scaled_freq);

    noInterrupts();            // désactiver toutes les interruptions
    TCCR3A = 0;                // set entire TIMER3 TCCR3A register to 0
    TCCR3B = 0;                // set entire TIMER3 TCCR3B register to 0
    TCNT3  = 0;                // initialize TIMER3 counter value to 0
    OCR3A  = match;             // set CompareRegister 
    TCCR3B |= bit(WGM12);      // turn ON the CTC mode
    TCCR3B |= bit(CS12);       // set CS12 bit for 256 prescaler
    TIMSK3 |= bit(OCIE1A);     // enable timer "Compare" interrupt
    interrupts();              // activer toutes les interruptions
}

/** *******************************************************************************
 * Stoppe le timer TIMER3
 * ********************************************************************************/
void  RemoteDisplay::stopTimer3()
{
    cli();                        // stop interrupts
    TIMSK3 &= ~bit(OCIE1A);       // disable timer "Compare" interrupt 
    sei();                        // allow interrupts
}


/** *******************************************************************************
 * TIMER3 interrupt routine. 
 * @attention Ne supporte pas u8g2.sendBuffer().
 * @remark Déclarer les variables nécessaires en 'volatile'.
 * ******************************************************************************* */
ISR(TIMER3_COMPA_vect)
{
        digitalWrite(23,LOW);
        digitalWrite(25,LOW);
}
