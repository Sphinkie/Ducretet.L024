/* *******************************************************************************
 * REMOTE DISPLAY
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


/* *******************************************************************************
 * Initialisation de l'afficheur
 * ******************************************************************************* */
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


/* *******************************************************************************
 * Efface l'écran
 * ******************************************************************************* */
void RemoteDisplay::clearScreen()
{
   u8g2.clearBuffer();
   u8g2.sendBuffer();
}


/* *******************************************************************************
 * Affiche le Titre du morceau
 * *******************************************************************************
 * 128 pixels = 10.6 chars (avec police u8g2_font_profont22_tf)
 * ******************************************************************************* */
void RemoteDisplay::printTitle(String titleString)
{
  Serial.println("printTitle");
  u8g2_uint_t width;                    // Largeur du texte (en pixels)
//  u8g2_uint_t pos;                      // Position du texte (en pixels)

  if (titleString=="NOISE") return;
  
  titleLen = titleString.length();
  titleString.toCharArray(titleText, titleLen);
  titleString.toCharArray(titleSubText, 16);
  // En raison de l'UTF-8, certaines lettres prennent 2 bytes...
  // D'où un CharArray de [16] pour stocker une String de [10].

  u8g2.setFont(u8g2_font_profont22_tf);
  if (titleLen>10)
  {
    // On place le titre à gauche, puis on le fera scroller.
    titlePos = 0;
    firstVisibleChar =0;
    scrollableTitle=true;
  }
  else
  {
    // On centre le titre
    width = u8g2.getUTF8Width(titleText);
    titlePos = (128 - width) / 2;
    scrollableTitle=false;
  }
  // On affiche le texte
  u8g2.clearBuffer();
  u8g2.drawUTF8(titlePos, 32, titleSubText);
  u8g2.sendBuffer();

  
}


/* *******************************************************************************
 * Affiche l'artiste sur une ou deux lignes
 * ******************************************************************************* */
void RemoteDisplay::printArtist(String texte)
{
   char line1[20];
   char line2[20];
   byte longeurTitre = texte.length();
   const byte MAX_LINE_LEN = 16;    // Nb max de charactères dans le rectangle (par ligne)

   if (longeurTitre < MAX_LINE_LEN)
   {
        texte.toCharArray(line1, MAX_LINE_LEN);
        strcpy(line2, " ");
   }
   // Sinon, le texte tient sur deux lignes: il faut trouver où tronquer.
   else
   {
      byte cissure=longeurTitre;
      while (cissure>MAX_LINE_LEN) cissure = texte.lastIndexOf(' ',cissure-1);
      texte.toCharArray(line1, cissure+1);   // [0..cissure]
      texte.substring(cissure+1).toCharArray(line2, MAX_LINE_LEN);
   }
   // On centre les textes
   u8g2.setFont(u8g2_font_helvR12_tf);
   byte pos1 = (128 - u8g2.getUTF8Width(line1)) / 2;
   byte pos2 = (128 - u8g2.getUTF8Width(line2)) / 2;

   u8g2.setFontMode(1);                // 0:pour police _mx (faster) / 1:pour police _tx
   u8g2.clearBuffer();
   u8g2.drawStr(pos1, 26, line1);
   u8g2.drawStr(pos2, 42, line2);
   //u8g2.drawRFrame(2, 18, 124, 44, 7);
   this->addHeader();
   u8g2.sendBuffer();

   Serial.println("printFramedText: "+texte);
   Serial.println(line1);
   Serial.println(line2);

}


/* *******************************************************************************
 * Affiche l'année (sauf si 0)
 * u8g2_font_ncenB24_tn      : Possible aussi, mais un peu moins joli.
 * u8g2_font_osb26_tn        : "Old Standard" Bold. Existe en 18 - 21 - 26 - 29
 * ******************************************************************************* */
void RemoteDisplay::printYear(int value)
{
   Serial.println("printYear: "+String(value));
  
   if (value!=0)
   {
      char texte[5];   
      itoa (value, texte, 10);          // base 10
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB24_tn);
      u8g2.setFontMode(1);              // 0:pour police _mx (faster) / 1:pour police _tx
      u8g2.drawStr(22, 24, texte);      // draw the text
      u8g2.drawRFrame(2, 18, 124, 44, 7);
      this->addHeader();
      u8g2.sendBuffer();
   }
}

/* *******************************************************************************
 * 
 * ******************************************************************************* */
void RemoteDisplay::printBeat(String texte)
{}

/* *******************************************************************************
 * Affiche le Genre de musique. Centré sur 1 ligne. 
 * u8g2_font_DigitalDisco_tf
 * ******************************************************************************* */
void RemoteDisplay::printGenre(String GenreString)
{
   const byte MAX_LINE_LEN = 16;    // Nb max de charactères dans le rectangle (par ligne)
   char texte[32];
   byte textePos;
   
   Serial.println("printGenre: "+String(GenreString));
   GenreString.toCharArray(texte, MAX_LINE_LEN);
   u8g2.setFont(u8g2_font_Pixellari_tf); 
   textePos = (128 - u8g2.getUTF8Width(texte)) / 2;
   u8g2.clearBuffer();
   u8g2.drawStr(textePos, 32, texte);
   u8g2.drawRFrame(2, 18, 124, 44, 7);      
   this->addHeader();
   u8g2.sendBuffer();
}

/* *******************************************************************************
 * Mémorise le texte à afficher dans le header. Tronque à 17. 
 * ******************************************************************************* */
void RemoteDisplay::setHeader(String texte)
{
  texte.toCharArray(this->header,17);
  u8g2.setFont(u8g2_font_Pixellari_tf); 
  headerPos = (128 - u8g2.getUTF8Width(this->header)) / 2;
}

/* *******************************************************************************
 * Dessine un header avec le texte prévu. Noir sur fond blanc.
 * *******************************************************************************
 *  u8g2_font_DigitalDisco_tf (17 char on a line) - très lisible (daFont)
 *  u8g2_font_halftone_tf     (18 char on a line) - très discret (grisé sur fond blanc)
 *  u8g2_font_profont12_tf    (21 char on a line) - trop fin
 * ******************************************************************************* */
void RemoteDisplay::addHeader()
{
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setFont(u8g2_font_DigitalDisco_tf); 
  u8g2.setDrawColor(0);
  u8g2.drawStr(this->headerPos, 0, this->header);
  u8g2.setDrawColor(1);
}

void RemoteDisplay::printStars(String texte)
{}


/* *******************************************************************************
 * Eface l'écran, mais laisse le Header visible.
 * ******************************************************************************* */
void RemoteDisplay::clearText()
{
   u8g2.clearBuffer();
   this->addHeader();
   u8g2.sendBuffer();
}


/* *******************************************************************************
 * 
 * ******************************************************************************* */
void  RemoteDisplay::startScrolling()
{
  Serial.println("startScrolling");
  if (scrollableTitle)  titleScrolling = true;
}
void  RemoteDisplay::stopScrolling()
{
  titleScrolling = false;
}
void  RemoteDisplay::scroll()
{
  if (titleScrolling)
  {
    // Si le debut du texte est visible
    if (titlePos >= 0)
    {
      firstVisibleChar = 0;
    }  
    // Si un caractère vient de sortir de l'écran (ie: pos est négatif)
    else if (titlePos == -char_width)
    {
      // si le caractère était un prefixe UTF8
      if (titleSubText[0] == 0xFFC3)
      {
        // on avance la substring de 2 bytes
        firstVisibleChar += 2;
      }
      // Autre caractère:
      else
      {
        // on avance la substring de 1 byte
        firstVisibleChar += 1;
      }
      memcpy(titleSubText, &titleText[firstVisibleChar], 15 );
      titleSubText[15] = '\0';
      // on repositionne la substring
      titlePos = 0;
    }
    u8g2.clearBuffer();
    u8g2.drawUTF8(titlePos, 32, titleSubText);
    u8g2.sendBuffer();
    titlePos--;            // on décale la position d'un pixel vers la gauche
  }
}





/* *******************************************************************************
 * Démarre le timer TIMER3
 * *******************************************************************************
 * Note sur timer3:
 *     compteur 16bits => match register = 65.536 max
 *     prescaler max = 1024
 *     donc sa frequence minimale = 16M/1024/65536 = 0.238 Hz = 4.19 sec
 * Prescaler allowed values: 1,8,64,256,1024
 * *******************************************************************************
 * cpu frequency       = 16.000.000 (16 MHz)
 * diviseur prescaler  = 256
 * fréquence souhaitée = 25 fois par seconde
 * *******************************************************************************
 *   Match   25 Hz = 16.000.000/256/25     = 2.500
 *   Match   30 Hz = 16.000.000/256/25     = 2.000
 * *******************************************************************************
 * le timer incremente son compteur à 16M/prescaler = 16M/256 = 62500 Hz
 * le timer génère une IT lorsque le compteur atteint la valeur du match register
 * on calcule donc sa valeur: MatchRegister + 1 = 16M / 256 / DesiredInterruptFrequency
 * (si on veut être précis, le +1 prend en compte la RAZ du compteur).
 * ******************************************************************************* */
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

void  RemoteDisplay::stopTimer3()
{
    cli();                        // stop interrupts
    TIMSK3 &= ~bit(OCIE1A);       // disable timer "Compare" interrupt 
    sei();                        // allow interrupts
}


/* *******************************************************************************
 * TIMER3 interrupt routine. Attention: ne supporte pas u8g2.sendBuffer().
 * Déclarer les variables nécessaires en 'volatile'.
 * ******************************************************************************* */
ISR(TIMER3_COMPA_vect)
{
        digitalWrite(23,LOW);
        digitalWrite(25,LOW);
}
