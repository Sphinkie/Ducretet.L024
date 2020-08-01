/* ****************************************************************************************************
 * Programme pour expérimenter les différentes possibilités graphiques de l'afficheur OLED
 * **************************************************************************************************** */

#include <U8g2lib.h>
#include <Wire.h>
/* #include "bitmap.h" */

#define time_delay 2000

/*
  Creation de l'objet u8g2 avec les paramètres correspondants à un écran avec chipset SH1106.
*/
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
// U8G2_R0           = No rotation, landscape
// Use U8X8_PIN_NONE = there is no switch connected to the pin
// _F_               = Full buffer mode



/* ****************************************************************************************************
 * Initialisations
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
 * Affiche du titre aligné à gauche. A faire avant de commencer le scrolling.
 * **************************************************************************************************** */
void displayTitle(char* text)
{
  int pos = 2;
  u8g2.setFont(u8g2_font_profont22_tf);

  // on definit la substring à afficher
  char sub_text[16];
  memcpy(sub_text, &text[0], 15 );
  sub_text[15] = '\0';

  u8g2.clearBuffer();
  //u8g2.setCursor(6, 4);
  //u8g2.print(u8g2.getUTF8Width(sub_text));
  //u8g2.setCursor(90, 4);
  //u8g2.print(u8g2.getUTF8Width(text));
  u8g2.drawUTF8(pos, 32, sub_text);
  u8g2.sendBuffer();
}

/* ****************************************************************************************************
 * Affiche du titre aligné à gauche. A faire avant de commencer le scrolling à police propotionnelle.
 * **************************************************************************************************** */
void displayTitleProp(char* text)
{
  int pos = 2;
  u8g2.setFont(u8g2_font_helvR18_tf);
  // on definit la substring à afficher
  char sub_text[16];
  memcpy(sub_text, &text[0], 15 );
  sub_text[15] = '\0';
  u8g2.clearBuffer();
  u8g2.drawUTF8(pos, 32, sub_text);
  u8g2.sendBuffer();
}


/* ****************************************************************************************************
   Affichage du titre centré
 * **************************************************************************************************** */
void displayTitleCentered(char* text)
{
  Serial.println("displayTitleCentered");
  u8g2.setFont(u8g2_font_profont22_tf);

  // on definit la substring à afficher (devrait etre à width)
  char sub_text[16];
  memcpy(sub_text, &text[0], 15 );
  sub_text[15] = '\0';
  u8g2_uint_t width = u8g2.getUTF8Width(text);        // calculate the pixel width of the text
  u8g2_uint_t sub_width = u8g2.getUTF8Width(sub_text);             // taille de substring restant à afficher

  // on part de l'hypothèse que le texte est moins large que l'écran (width < 128)
  pos = (128 - width) / 2;
  u8g2.clearBuffer();
  u8g2.setCursor(6, 4);
  u8g2.print(width);
  u8g2.setCursor(90, 4);
  u8g2.print(sub_width);
  u8g2.drawUTF8(pos, 32, sub_text);
  u8g2.sendBuffer();
}


/* ****************************************************************************************************
   Scrolling pour les polices à taille fixe (ici 12 pixels de large)
 * ****************************************************************************************************
   On utilise une substring, car si le texte dépasse 256 pixels, il revient écraser l'affichage.
   Taille de la substing = 15 char = 15*12 pixels = 180 pixels.
          ecran        mémoire       ecrasement ecran
      [...128...]  ....128....   [..................
      The house o  f the Risin   g Sun
 * **************************************************************************************************** */
void displayScrollingTitle(char* text)
{
  Serial.println("displayScrollingTitle");
  // On détermine la taille
  u8g2.setFont(u8g2_font_profont22_tf);
  const byte char_width = 12;             // pour cette police, les caractères ont une largeur de 12 pixels

  /* en UTF8 les caractères accentués sont sur 2 octets et commencent par 0xFFC3. Ex: "é" = 0xC3A9   */

  int pos = 0;
  byte text_len = strlen(text);
  byte first_visible_char = 0;

  // On definit la substring qui sera affichée
  char sub_text[16];
  memcpy(sub_text, &text[first_visible_char], 15 );
  sub_text[15] = '\0';

  /* 
  //dumps 
  Serial.print("text    : ");
  for (byte i = 0; i < 16; i++){Serial.print(text[i], HEX);Serial.print(" ");} Serial.println();
  Serial.print("sub_text: ");
  for (byte i = 0; i < 16; i++){Serial.print(sub_text[i], HEX);Serial.print(" ");} Serial.println();
  */

  // on fait venir le texte par la droite
  do {
    if (pos >= 0)
      first_visible_char = 0;
      
    // Si un caractère vient de sortir de l'écran (pos est négatif)
    else if (pos == -char_width)
    {
      // Serial.print("Going out: ");Serial.println(sub_text[0], HEX);
      // si le caractère était un prefixe UTF8
      if (sub_text[0] == 0xFFC3)
      {
        // Serial.println("UTF8 prefix: << exiting 2 bytes.");
        // on avance la substring de 2 bytes
        first_visible_char += 2;
      }
      // Autre caractère:
      else
      {
        // on avance la substring de 1 byte
        first_visible_char += 1;
      }
      memcpy(sub_text, &text[first_visible_char], 15 );
      sub_text[15] = '\0';
      for (byte i = 0; i < 15; i++){Serial.print(sub_text[i], HEX);Serial.print(" ");} Serial.println();
      // on repositionne de la substring
      pos = 0;
    }
    u8g2.clearBuffer();
/*  u8g2.setCursor(6, 4);
    u8g2.print(u8g2.getUTF8Width(sub_text));
    u8g2.setCursor(90, 4);
    u8g2.print(u8g2.getUTF8Width(text));*/
    u8g2.drawUTF8(pos, 32, sub_text);
    u8g2.sendBuffer();
    delay(10);        // petite tempo
    pos--;            // on décale la position d'un pixel vers la gauche
  }
  // on arrete le scroll quand il ne reste aucun caractère visible
  while (first_visible_char < text_len);
}


/* ****************************************************************************************************
 * Scrolling pour les polices à taille proportionelle
 * ****************************************************************************************************
 * Résultat saccadé quoique l'on fasse. getWidth ne renvoie pas toujours la bonne valeur...
 * Peut-etre en raison de la transparence de la police?
 * ****************************************************************************************************
   On utilise une substring, car si le texte dépasse 256 pixels, il revient écraser l'affichage.
   Taille de la substring 15 char => on peut utiliser des polices ayant entre 8 et 17 pixels de large.
   Taille de la substring 14 char => on peut utiliser des polices ayant entre 9 et 18 pixels de large.
          ecran        mémoire       ecrasement ecran
      [...128...]  ....128....   [..................
      The house o  f the Risin   g Sun
 * **************************************************************************************************** */
void displayScrollingTitleProp(char* text)
{
  Serial.println("displayScrollingTitleProp");
  u8g2_uint_t width;        // pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined
  u8g2_uint_t sub_width;
  byte        char_width;   // largeur d'un caractère en pixels (non constant)

  u8g2.setFont(u8g2_font_helvR18_tf);     // Lucida: pas très jolie u8g2_font_luRS19_tf
  width = u8g2.getUTF8Width(text);        // calculate the pixel width of the text

  int pos = 0;
  byte text_len = strlen(text);

  // On definit la substring qui sera affichée
  char sub_text[16];
  byte first_visible_char = 0;
  memcpy(sub_text, &text[first_visible_char], 15 );
  sub_text[15] = '\0';
  sub_width = u8g2.getUTF8Width(sub_text);        // largeur du texte affichable
  // les fonction de la librairie u8g2 donnent la largeur en pixels d'une chaine, mais pas celle d'un caractère.
  // il nous faut donc tester une chaine d'une seule lettre (de 1 ou 2 octets, selon s'il est accentué ou pas.)
  char first_letter[3];
  strncpy(first_letter, sub_text, 2);   
  if (first_letter[0]!=0xFFC3) first_letter[1]='\0';  // si ce n'est pas un caractere accentué, on n'a pas besoin du 2eme octet;
  first_letter[2]='\0';
  char_width = u8g2.getUTF8Width(first_letter)+1;       // largeur du premier caractère
  /* dumps */
  Serial.print("text: ");
  for (byte i = 0; i < 15; i++) {    Serial.print(text[i], HEX);    Serial.print(" ");  } Serial.println();
  Serial.print("sub_text: ");
  for (byte i = 0; i < 15; i++) {    Serial.print(sub_text[i], HEX);    Serial.print(" ");  } Serial.println();
  Serial.print("first_letter: ");
  for (byte i = 0; i < 3; i++)  {    Serial.print(first_letter[i], HEX);    Serial.print(" ");  } Serial.println();

  // on fait venir le texte par la droite
  do {
    if (pos >= 0)
      first_visible_char = 0;
      
    // Si un caractère vient de sortir de l'écran (pos est négatif)
    // la problématque est de connaitre la largeur en pixels du caractère qui vient de sortir
    else if (pos == -char_width)
    
    {
      Serial.print("Going out: ");Serial.println(sub_text[0], HEX);
      // si le caractère était un prefixe UTF8
      if (sub_text[0] == 0xFFC3)
      {
        Serial.println("UTF8 prefix: << exiting 2 bytes.");
        // on avance la substring de 2 bytes
        first_visible_char += 2;
      }
      // Autre caractère:
      else
      {
        // on avance la substring de 1 byte
        first_visible_char += 1;
      }
      memcpy(sub_text, &text[first_visible_char], 15 );
      sub_text[15] = '\0';
      for (byte i = 0; i < 15; i++){Serial.print(sub_text[i], HEX);Serial.print(" ");} Serial.println();
      // on examine la première lettre
      strncpy(first_letter, sub_text, 2);   
      if (first_letter[0]!=0xFFC3) first_letter[1]='\0';  // si ce n'est pas un caractère accentué, on n'a pas besoin du 2eme octet;
      first_letter[2]='\0';

      char_width = u8g2.getUTF8Width(first_letter)   +1;  // patch de la largeur
      Serial.print("first_letter: '"); 
      Serial.print(first_letter); 
      Serial.print("' : "); 
      for (byte i = 0; i < 3; i++)  {Serial.print(first_letter[i], HEX);Serial.print(" ");} Serial.println();
      Serial.print("width= ");Serial.println(char_width);

      // on repositionne de la substring
      pos = 0;
    }

    u8g2.clearBuffer();
    u8g2.drawUTF8(pos, 32, sub_text);
    u8g2.sendBuffer();
    delay(10);
    pos -= 1;
  }
  // on arrete le scroll quand il ne reste aucun caractère visible
  while (first_visible_char < text_len);
}


/* ****************************************************************************************************
 *  Fait grossir une étoile (taille 1 à taille 4)
 * **************************************************************************************************** */
void animStar(int pos)
{
  drawStar(pos, 1.0);
  u8g2.sendBuffer();
  delay(40);
  drawStar(pos, 2.0);
  u8g2.sendBuffer();
  delay(40);
  drawStar(pos, 3.0);
  u8g2.sendBuffer();
  delay(40);
  drawStar(pos, 4.0);
  u8g2.sendBuffer();
  delay(120);
}

/* ****************************************************************************************************
 *  dessine une étoile, de la taille donnée
 * **************************************************************************************************** */
void drawStar(int pos, float a)
{
  int x0 = pos * 36 - 8;
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


/* ****************************************************************************************************
 *
 * **************************************************************************************************** */
void displayGenre()
{
  // Famille des fontes Lucida: 08 - 10 - 12 - 13 - 14 - 16 - 18 - 19 - 24
  u8g2.setFont(u8g2_font_luIS14_tf);
  u8g2.drawStr(4, 30, "Lucida IS 14");
  u8g2.drawRFrame(2, 20, 124, 34, 7);
}

/* ****************************************************************************************************
 *  Texte de 2 lignes dans un cadre
 * **************************************************************************************************** */
void displayDualLine(uint8_t* font, char* line1, char* line2)
{
  u8g2.clearBuffer();
  displayMenu();
  u8g2.setFont(font);
  u8g2.drawStr(6, 26, line1);
  u8g2.drawUTF8(6, 42, line2);
  u8g2.drawRFrame(2, 18, 124, 44, 7);
  u8g2.sendBuffer();
 }

/* ****************************************************************************************************
 * Affiche un texte en gros
 * **************************************************************************************************** */
void displayNumbers(uint8_t* font)
{
  const char* text = "1964";

  // font numérique Old Standard = 18 - 21 - 26 - 29 - 35 - 41
  u8g2.setFont(font);   // u8g2_font_ncenB24_tn   u8g2_font_osb29_tn
  u8g2.setFontMode(0);                // enable transparent mode, which is faster
  u8g2.drawStr(20, 20, text);         // draw the text

}

/* ****************************************************************************************************
    Menubox en haut. Texte sur fond blanc.
 * **************************************************************************************************** */
void displayMenu()
{
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setFont(u8g2_font_DigitalDisco_tf); 
  // u8g2_font_DigitalDisco_tf (17 char on a line) - très lisible
  // u8g2_font_halftone_tf     (18 char on a line) - très discret (grisé sur fond blanc)
  // u8g2_font_profont12_tf    (21 char on a line) - trop fin
  u8g2.setDrawColor(0);
  u8g2.drawStr(2, 0, "u8g2_font_DigitalDisco_tf");
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
 *   dessine une courbe polaire, de parametre 'b'
 *   r² = a²cos2Th + sqr(b4-a4sin²22Th)  (pour les calculs intermédiaires: F1 + F2)
 *   Th = Théta (varie entre 0 et pi/2, par 96 pas)
 *  on fait varier b entre 0 et 2*a
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
 * Dessine une courbe de Jarre. voir p280.
 *    x = cos3t + sin4t
 *    y = sint + cos2t + 2
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
 *  Dessine une barre animée de largeur = sint + sin2t. Pas très fluide.
 * **************************************************************************************************** */
void drawBar()
{
  for (float t = 0.0; t < 6.2832; t += 0.0327)
  {
    u8g2.clearBuffer();
    float x = abs(sin(t) + sin(2 * t));
    // u8g2.drawBox(64-20*x, 30, 40 * x, 6); // lent
    u8g2.drawHLine(64 - 20 * x, 30, 40 * x);
    u8g2.sendBuffer();
  }
}


/* ****************************************************************************************************
 *  Dessine des points qui s'echappent vers les bords
 * **************************************************************************************************** */
void drawFlyingPixels()
{
  // A animer avec deux ISR pour être en rythme et avoir plusieurs points en même temps.
  // ISR1 = beat: debut d'une séquence: creation de points
  // ISR2 = régulière : avancement des points et refresh display
  // debut de l'annimation: un rond qui grossit
    u8g2.clearBuffer();
    u8g2.drawDisc(64, 30,3);
    u8g2.sendBuffer();
    delay(40);
    u8g2.clearBuffer();
    u8g2.drawCircle(64,30,4);
    u8g2.sendBuffer();
    delay(40);
    u8g2.clearBuffer();
    u8g2.drawCircle(64,30,6);
    u8g2.sendBuffer();
    delay(40);
    u8g2.clearBuffer();
    u8g2.drawEllipse(64,30,9,7);
    u8g2.sendBuffer();
    delay(40);
  
  for (int t = 10; t < 64; t += 1)
  {
    u8g2.clearBuffer();
    u8g2.drawPixel(64 + t, 30);
    u8g2.drawPixel(64 - t, 30);
    u8g2.sendBuffer();
    if (t < 40) t += 1; // un peu de dynamisme au début
  }
}


/* ****************************************************************************************************
 *  Animation courbe
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
 *  SETUP
 * **************************************************************************************************** */
void setup(void)
{
    Serial.println("** start **");
  // begin() function will reset, configure, clear and disable power save mode of the display
  u8g2.begin();
  u8g2_prepare();
  Serial.begin(115200);
}


/* ****************************************************************************************************
    LOOP
 * **************************************************************************************************** */
void loop(void)
{
   
  // Effacement
  u8g2_prepare();

  char title[64];

  /*
    // Affiche une image
    u8g2.clearBuffer();
    u8g2_bitmap();
    u8g2.sendBuffer();
    delay(3*time_delay);
  */
  
    // Affiche des points fuyants vers les bords
    for (int i=10; i<50; i+=10)
    {
      drawFlyingPixels();
      delay(i);
    }
/*
      // trace un pixel animé le long d'une courbe
      drawJarre();
      // Affiche une courbe animée
      animCurve();
      // Affiche une barre animée
      animBar();
*/
   
      // anime 3 étoiles
      u8g2.clearBuffer();
      u8g2.drawRFrame(2, 18, 124, 44, 7);
      animStar(1);
      animStar(2);
      animStar(3);
      delay(time_delay);

      // Affiche l'année
      u8g2.clearBuffer();
      displayMenu();
      displayNumbers(u8g2_font_ncenB24_tn);
      u8g2.sendBuffer();
      delay(3*time_delay);

      // Affiche l'année
      u8g2.clearBuffer();
      displayMenu();
      displayNumbers(u8g2_font_osb29_tn);
      u8g2.sendBuffer();
      delay(3*time_delay);

      // Police 12 sur 2 lignes
      displayDualLine(u8g2_font_luRS10_tf,"Lucida","RS 10");
      delay(3*time_delay);
      displayDualLine(u8g2_font_DigitalDisco_tf,"Digital","Disco 10");  // police DaFont 10
      delay(3*time_delay);
      displayDualLine(u8g2_font_profont16_tf,"Pro Font","16");          // police monotype
      delay(3*time_delay);
      displayDualLine(u8g2_font_helvR12_tf,"Helvetica","R 12");         // police proportionelle
      delay(3*time_delay);
      displayDualLine(u8g2_font_helvR12_tf,"Helvetica","R 12");         // police proportionelle
      delay(3*time_delay);
      displayDualLine(u8g2_font_helvR12_tf,"Helvetica","R 12");         // police proportionelle
      delay(3*time_delay);
      displayDualLine(u8g2_font_helvR12_tf,"Helvetica","R 12");         // police proportionelle
      delay(3*time_delay);
      
      // u8g2_font_ncenR12_tf                "Adobe X11": joli. Mais avec serif
      // u8g2_font_cupcakemetoyourleader_tr  Style SF. (pas de tf)

      // Police 14 sur 1 ligne
      u8g2.clearBuffer();
      displayMenu();
      displayGenre();
      u8g2.sendBuffer();
      delay(3*time_delay);

      // Titre (centré)
      strcpy(title,"Profont22");
      displayTitleCentered(title);
      delay(time_delay);
  

  // Titres déroulants
  
  strcpy(title,"Déroulant à police monotype");
  displayTitle(title);
  delay(time_delay);
  displayScrollingTitle(title);
  delay(time_delay);

  strcpy(title,"Déroulant à police proportionelle");
  displayTitleProp(title);
  delay(time_delay);
  displayScrollingTitleProp(title);
  delay(time_delay);
  

}
