/*
*/

#include <U8g2lib.h>
#include <Wire.h>
/* #include "bitmap.h" */

#define time_delay 2000

/*  Next, object called u8g2 is created, with the following line of code:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
  The created object represents the screen itself and it is used to control the
  screen. The U8g2 library can be used for many other OLED screens, thus
  there are many constructors in the sketch examples from the library.
*/
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
// U8G2_R0   No rotation, landscape
// Use U8X8_PIN_NONE if there is no switch connected to the pin

/* ****************************************************************************************************
 * **************************************************************************************************** */
void u8g2_prepare()
{
  /*  setFont sets the u8g2 font. */
  u8g2.setFont(u8g2_font_6x10_tf);
  /*  setFontRefHeightExtendedText is used for drawing characters on the screen. */
  u8g2.setFontRefHeightExtendedText();
  /*  setDrawColor has one argument = an integer number which represents a color index for all drawing functions.
      Font drawing procedures use this argument to set the foreground color.
      The default value is 1.
      If it is set to 0, then the space around the character is lit up, and the character is not.
      Argument value 2 can also be used, but there is no difference from 0.  */
  u8g2.setDrawColor(1);
  /* setFontPosTop controls the character position in one line of the text.
      The function has a couple of versions:
      setFontPosBaseLine, setFontPosCenter, setFontPosBottom
      Their purpose is to change the position of the characters in the one line.*/
  u8g2.setFontPosTop();
  /* setFontDirection has one argument which is an integer number which represents direction of the text.
     0 = 0°
     1 = 90°
     2 = 180°
     and 3 = 270° */
  u8g2.setFontDirection(0);
}


/* ****************************************************************************************************

 * **************************************************************************************************** */
void displayGenre()
{
  // Famille des fontes Lucida: 08 - 10 - 12 - 13 - 14 - 16 - 18 - 19 - 24
  u8g2.setFont(u8g2_font_luIS14_tf);
  u8g2.drawStr(10, 30, "Rock n'Roll");   // A centrer
  u8g2.drawRFrame(2, 20, 124, 34, 7);
}

/* ****************************************************************************************************

 * **************************************************************************************************** */
void displayTitle()
{
  u8g2_uint_t offset;       // current offset for the scrolling text
  u8g2_uint_t width;        // pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined

//  const char* text = "The house of the Rising Sun";  // largeur 268 (recouvrement à partir de "ng Sun")
//  const char* text = "Bom bom bom";  // largeur 132  (dépasse légèrement des 128: "m")
  const char* text = "Sons and Daughters";  // largeur 216  (dépasse des 128: "aughters")
  u8g2.setFont(u8g2_font_profont22_mr);

  width = u8g2.getUTF8Width(text);    // calculate the pixel width of the text

  u8g2.setCursor(10, 10);
  u8g2.setFont(u8g2_font_9x18B_mn);
  u8g2.print(width);

char subbuff[5];
memcpy( subbuff, &text[0], 4 );
subbuff[4] = '\0';

  u8g2.setFont(u8g2_font_profont22_mr);
  u8g2.drawUTF8(2, 30, text);   // A dérouler

//while( x < u8g2.getDisplayWidth() );    // draw again until the complete display is filled
    
  offset-=1;              // scroll by one pixel
  if ( (u8g2_uint_t)offset < (u8g2_uint_t)-width )  
    offset = 0;             // start over again
  delay(10);
  
  u8g2.sendBuffer();

}


/* ****************************************************************************************************
    Anime une étoile
 * **************************************************************************************************** */
void animStar(int pos)
{
  drawStar(pos, 2.0);
  u8g2.sendBuffer();
  delay(40);
  drawStar(pos, 3.0);
  u8g2.sendBuffer();
  delay(40);
  drawStar(pos, 4.0);
  u8g2.sendBuffer();
  delay(40);
  drawStar(pos, 5.0);
  u8g2.sendBuffer();
  delay(120);
  //  drawStar(pos,6.0);
  //  u8g2.sendBuffer();
}

/* ****************************************************************************************************
    dessine une étoile
 * **************************************************************************************************** */
void drawStar(int pos, float a)
{
  int x0 = pos * 36 - 8;
  int y0 = 35;
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


/* ****************************************************************************************************
    Texte de 2 lignes dans un cadre
 * **************************************************************************************************** */
void displayAlbum()
{
  // Famille des fontes Lucida: 08 - 10 - 12 - 13 - 14 - 16 - 18 - 19 - 24
  u8g2.setFont(u8g2_font_luRS10_tf);
  u8g2.drawStr(6, 26, "Tout ce qui brille");
  u8g2.drawUTF8(6, 42, "(éèàùêç)");  // 30 char max
  u8g2.drawRFrame(2, 18, 124, 44, 7);
}

/* ****************************************************************************************************
    Texte de 2 lignes dans un cadre
 * **************************************************************************************************** */
void displayArtist()
{
  // Famille des fontes DaFont: 10
  u8g2.setFont(u8g2_font_DigitalDisco_tf);
  u8g2.drawStr(6, 26, " The Papas & ");
  u8g2.drawStr(6, 42, "   The Mamas");  // 30 char max
  u8g2.drawRFrame(2, 18, 124, 44, 7);
}

/* ****************************************************************************************************

 * **************************************************************************************************** */
void displayYear()
{
  const char* text = "1964";

  // font numérique Old Standard = 18 - 21 - 26 - 29 - 35 - 41
  u8g2.setFont(u8g2_font_osb29_tn);   // u8g2_font_ncenB24_tn
  u8g2.setFontMode(0);                // enable transparent mode, which is faster
  u8g2.drawStr(20, 20, text);         // draw the text

}

/* ****************************************************************************************************
    Menubox en haut
 * **************************************************************************************************** */
void displayMenu()
{
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setFont(u8g2_font_halftone_tf);
  u8g2.setDrawColor(0);
  u8g2.drawStr(2, 0, "  Genre: rock");
  u8g2.setDrawColor(1);
}


/* ****************************************************************************************************
    dessine une bitmap
 * **************************************************************************************************** */
/*
  void u8g2_bitmap()
  {
  u8g2.drawXBMP(0, 0, image_width, image_height, image_bits);
  }
*/

/* ****************************************************************************************************
    dessine une courbe polaire
    r² = a²cos2Th + sqr(b4-a4sin²22Th)  (pour les calculs intermédiaires: F1 + F2)
    Th = Théta (varie entre 0 et pi/2, par 96 pas)
   on fait varier b entre 0 et 2*a
 * **************************************************************************************************** */
void drawCurve(int b)
{
  byte a = 20;    // les deux foyers sont à +20 et -20 sur l'axe x.
  int  a2 = 400;
  float F1 = 0.0;
  float F2 = 0.0;

  for (float Th = 0.0; Th < 1.5708; Th += 0.0164)
  {
    F1 = a2 * cos(2 * Th);
    F2 = sqrt(pow(b, 4) - sq(a2 * sin(2 * Th)));
    // 1er point:
    float r1 = sqrt(F1 + F2);
    int x = r1 * cos(Th);
    int y = r1 * sin(Th);
    u8g2.drawPixel(64 + x, 32 + y);
    u8g2.drawPixel(64 + x, 32 - y);
    u8g2.drawPixel(64 - x, 32 + y);
    u8g2.drawPixel(64 - x, 32 - y);
    // eventuel 2eme point
    if (F1 > F2)
    {
      float r2 = sqrt(F1 - F2);
      x = r2 * cos(Th);
      y = r2 * sin(Th);
      u8g2.drawPixel(64 + x, 32 + y);
      u8g2.drawPixel(64 + x, 32 - y);
      u8g2.drawPixel(64 - x, 32 + y);
      u8g2.drawPixel(64 - x, 32 - y);
    }
  }
}


/* ****************************************************************************************************
    Dessine une courbe de Jarre. voir p280.
     x = cos3t + sin4t
     y = sint + cos2t + 2
 * **************************************************************************************************** */
void drawJarre(int pixel_len = 4, int pixel_speed = 0)
{

  for (float t = 0.0; t < 6.2832; t += 0.0327 * pixel_len)
  {
    u8g2.clearBuffer();
    for (int l = 0; l < pixel_len + 1; l++)
    {
      float x = cos(3 * t) + sin(4 * t);
      float y = sin(t) + cos (2 * t) + 2;
      u8g2.drawPixel(64 + 20 * x, 32 + 8 * y);
      u8g2.sendBuffer();
      t += 0.0327;
    }
    delay(pixel_speed);
  }
}


/* ****************************************************************************************************
 * Dessine une barre animée de largeur = sint + sin2t
 * **************************************************************************************************** */
void drawBar()
{
  for (float t = 0.0; t < 6.2832; t += 0.0327)
  {
    u8g2.clearBuffer();
    float x = abs(sin(t) + sin(2 * t));
    // u8g2.drawBox(64-20*x, 30, 40 * x, 6); // lent
    u8g2.drawHLine(64-20*x, 30, 40*x);
    u8g2.sendBuffer();
  }
}


/* ****************************************************************************************************
 * Dessine des points qui s'echappent vers les bords
 * **************************************************************************************************** */
void drawFlyingPixels()
{
  // A animer avec deux ISR pour être en rythme et avoir plusieurs points en même temps.
  // ISR1 = beat: debut d'une séquence: creation de points
  // ISR2 = régulière : avancement des points et refresh display
  for (int t=1; t<64; t+=1)
  {
      u8g2.clearBuffer();
      u8g2.drawPixel(64 + t, 30);
      u8g2.drawPixel(64 - t, 30);
      u8g2.sendBuffer();
      if (t<10) t+=1; // un peu de dynamisme au début
  }
}


/* ****************************************************************************************************
    Animation courbe
 * **************************************************************************************************** */
void animCurve()
{
  for (int b = 4; b < 72; b++)
  {
    u8g2.clearBuffer();
    drawCurve(b);
    u8g2.sendBuffer();
    //delay(10);
    if (b > 24) b += b * 0.06;
  }
}


/* ****************************************************************************************************
    SETUP
 * **************************************************************************************************** */
void setup(void)
{
  // begin() function will reset, configure, clear and disable power save mode of the display
  u8g2.begin();
  u8g2_prepare();
}


/* ****************************************************************************************************
    LOOP
 * **************************************************************************************************** */
void loop(void)
{
  // Effacement
  u8g2_prepare();

  /*
    // Affiche une image
    u8g2.clearBuffer();
    u8g2_bitmap();
    u8g2.sendBuffer();
    delay(3*time_delay);
  */
/*
  // Affiche des points fuyants vers les bords
  for (int i=10; i<50; i+=10)
  {
    drawFlyingPixels();
    delay(i);
  }
  
    // trace un pixel animé le long d'une courbe
    drawJarre();
    // Affiche une courbe animée
    animCurve();
    // Affiche une barre animée
    animBar();

    // anime 3 étoiles
    u8g2.clearBuffer();
    u8g2.drawRFrame(2, 20, 124, 32, 7);
    animStar(1);
    animStar(2);
    animStar(3);
    delay(time_delay);

    // Affiche l'année
    u8g2.clearBuffer();
    displayMenu();
    displayYear();
    u8g2.sendBuffer();
    delay(time_delay);

    // Efface l'année
    u8g2.clearBuffer();
    displayMenu();
    u8g2.sendBuffer();
    delay(time_delay);

    // Album
    u8g2.clearBuffer();
    displayMenu();
    displayAlbum();
    u8g2.sendBuffer();
    delay(time_delay);

    // Artist
    u8g2.clearBuffer();
    displayMenu();
    displayArtist();
    u8g2.sendBuffer();
    delay(time_delay);
*/  
  // Titre (déroulant)
  u8g2.clearBuffer();
  //displayMenu();
  displayTitle();
  delay(5*time_delay);

    // Genre (1 ligne / centré)
    displayMenu();
    displayGenre();
    u8g2.sendBuffer();
    delay(time_delay);

}
