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
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);



/* *******************************************************************************
 * Constructor
 * ******************************************************************************* */
RemoteDisplay::RemoteDisplay()
{}

/* *******************************************************************************
 * Initialisation de l'afficheur
 * ******************************************************************************* */
void RemoteDisplay::initialize()
{
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();   // Ascent will be the largest ascent of "A", "1" or "(" of the current font. Descent will be the descent of "g" or "(" of the current font.
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();       // Change the reference position for the glyph and string draw functions.
  u8g2.setFontDirection(0);   // horizontal

}


/* *******************************************************************************
 * Efface l'écran
 * ******************************************************************************* */
void RemoteDisplay::clearScreen()
{
   u8g2.clearBuffer();
       u8g2.setCursor(90, 4);
    u8g2.print("start");
   u8g2.sendBuffer();
}

void RemoteDisplay::setBackgroundImage()
{}

/* *******************************************************************************
 * Affiche le Titre du morceau
 * ******************************************************************************* */
void RemoteDisplay::printTitle(String texte)
{
  
}


/* *******************************************************************************
 * Affiche l'artiste sur une ou deux lignes
 * ******************************************************************************* */
void RemoteDisplay::printArtist(String texte)
{
   // Si le texte tient en une ligne: on ajoute "par"
   if (texte.length() < MAX_LINE_LEN) texte = "par " + texte;
   this->printFramedText(texte);
}

/* *******************************************************************************
 * Affiche le nom de l'album sur une ou deux lignes
 * ******************************************************************************* */
void RemoteDisplay::printAlbum(String texte)
{
   this->printFramedText(texte);
}
      
/* *******************************************************************************
 * Affiche un texte en deux lignes dans un rectangle
 * ******************************************************************************* */
void RemoteDisplay::printFramedText(String texte)
{
   char line1[16];
   char line2[16];
   byte longeurTitre = texte.length();

   if (longeurTitre < MAX_LINE_LEN)
   {
        texte.toCharArray(line1, MAX_LINE_LEN);
        strcpy(line2, " ");
   }
   // Sinon, le texte tient sur deux lignes: il faut trouver où tronquer.
   else
   {
      byte cissure=longeurTitre;
      while (cissure>MAX_LINE_LEN)  cissure = texte.lastIndexOf(' ',cissure-1);
      texte.toCharArray(line1, cissure);   // [0..cissure]
      texte.substring(cissure+1).toCharArray(line2, MAX_LINE_LEN);
   }
   u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_DigitalDisco_tf);      // Famille des fontes DaFont: 10
   u8g2.setFontMode(0);                // 0:pour police _mx (faster) / 1:pour police _tx
   u8g2.drawStr(6, 26, line1);
   u8g2.drawStr(6, 42, line2);
   u8g2.drawRFrame(2, 18, 124, 44, 7);      
   u8g2.sendBuffer();

   Serial.println("printFramedText: "+texte);
   Serial.println(line1);
   Serial.println(line2);

}



/* *******************************************************************************
 * Affiche l'année (sauf si 0)
 * ******************************************************************************* */
void RemoteDisplay::printYear(int value)
{
   Serial.println("printYear: "+String(value));
  
   if (value!=0)
   {
      char texte[5];   
      itoa (value, texte, 10);            // base 10
      // "Old Standard" Bold. Existe en 18 - 21 - 26 - 29 - 35 - 41
      u8g2.setFont(u8g2_font_osb29_tn);   // u8g2_font_ncenB24_tn
      u8g2.setFontMode(0);                // 0:pour police _mx (faster) / 1:pour police _tx
      u8g2.clearBuffer();
      u8g2.drawStr(20, 20, texte);        // draw the text
      u8g2.sendBuffer();
   }
}

void RemoteDisplay::printBeat(String texte)
{}

void RemoteDisplay::printGenre(String texte)
{}

void RemoteDisplay::printHeader(String texte)
{}

void RemoteDisplay::printStars(String texte)
{}

void RemoteDisplay::printPictoMute()
{}

void RemoteDisplay::printPictoFM()
{}

void RemoteDisplay::clearPicto()
{}

char RemoteDisplay::requestStatus()
{}

    
