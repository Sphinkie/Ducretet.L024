/*

  Several sketch examples come with the library, to open one, go to:
  File > Examples > U8g2 > full_buffer > GraphicsTest
  With this sketch example, the screen can be tested. In this eBook, the
  sketch code is modified in oreder to create more beginner friendly version of
  code.
*/

/* ****************************************************************************************************
 * The function drawStr() is used to display a constant string on the screen. 
 * The first two arguments represent the X and Y position of the cursor, where the text is displayed.
 * The third argument represents the text itself, a constant string value. 
 * The functions that set text layout before using drawStr() function should be used, 
 * otherwise the drawStr() function uses default settings for the font, size and overall layout of the text.
 * **************************************************************************************************** */
 
/* ****************************************************************************************************
  To display shapes, specific functions for each shape are used:
  The function called drawFrame(), has four arguments and returns no
  value. It is used to display a frame, an empty rectangle. The first two
  arguments represent the X and Y position of the top left corner of the frame.
  The third argument represents the width of the frame and the fourth
  argument represents the height of the frame.

  The function called drawRFrame() has five arguments and returns no
  value. It is used to display a frame with rounded corners. The first two
  arguments represent the X and Y position of the top left corner of the frame.
  The second two arguments represent the width and height of the frame and
  the fifth argument represents the corner radius.
  
  The function called drawBox() has four arguments and returns no value. It
  is used to display a filled rectangle. The first two arguments represent the X
  and Y position of the top left corner of the rectangle. The second two
  arguments represent the width and height of the rectangle,respectively.
  The function called drawRBox() has five arguments and returns no value.
  It is used to display a filled rectangle with rounded edges. The first two
  arguments represent the X and Y position of the top left corner of the
  rectangle. The second two arguments represent the width and height of the
  rectangle, respectively. The fifth argument represents the corner radius.
  The function called drawCircle() has three arguments and returns no
  value. It is used to display a circle. The first two arguments represent the X
  and Y positions of the circle center point. The third argument represents the
  circle radius.

  The function called drawDisc() has three arguments and returns no
  value. It is used to display a disc. The first two arguments represent X and
  Y position of the disc center point. The third argument represents the disc
  radius.
  
  The function called drawTriangle() has six arguments and returns no
  value. It is used to display a filled triangle. The first two arguments represent
  the X and Y position of the first corner point of the triangle. The second two
  arguments represent the X and Y positions of the second corner point of the
  triangle. The last two arguments represent the X and Y positions of the last
  corner point of the triangle.

  The function called drawLine() has four arguments and returns no value.
  It is used to display a line. The first two arguments represent the X and Y
  position of the starting point of the line. The second two arguments
  represent X and Y position of the end point of the line.
  
  The function called drawUTF8() has three arguments and returns а value.
  It is used to display a text, the string value which may contain a character
  encoded as a Unicode character. The first two arguments represent the X
  and Y position of the cursor and the third represents the text itself. The
  Unicode characters can be displayed in a couple of ways. The first is to
  copy and paste the existing character into the sketch, like in the following
  line of the code: u8g2.drawUTF8(50, 20, " " ☂ )
  The second is to create a char array, which has two values: the first value
  is a hexadecimal number of the Unicode character, and the second value
  is a null character (“\0”). This can be done by using the char array called
  COPYRIGHT_SYMBOL, like in the following lines of the code:
  const char COPYRIGHT_SYMBOL[] = {0xa9, '\0'}
  u8g2.drawUTF8(95, 20, COPYRIGHT_SYMBOL); //COPYRIGHT SYMBOL
  
  The third way of using the function is to use a hexadecimal number for the
  character itself, like in the following line of code:
  u8g2.drawUTF8(115, 15, "\xb0"); // DEGREE SYMBOL
  The function returns a value, an integer number which represents the width
  of the text (string).
  To display something on the screen, the screen data buffer has to be
  cleared first, then a new value is set (an image) for data buffer, then a new
  value of data buffer is send to the screen. This way, a new image is
  displayed on the screen. In order to see this change, delay() function has
  to be used to shift the next change of the data buffer, like in the following
  lines of code:
  u8g2.clearBuffer();
  u8g2_bitmap(); // setting the data buffer
  u8g2.sendBuffer();
  delay(time_delay);

*/

/*  At the beginning of the sketch two libraries are imported the U8g2lib and Wire. */

#include <U8g2lib.h>
#include <Wire.h>

#define time_delay 2000

/*  Next, object called u8g2 is created, with the following line of code:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
  The created object represents the screen itself and it is used to control the
  screen. The U8g2 library can be used for many other OLED screens, thus
  there are many constructors in the sketch examples from the library.
*/
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
const char COPYRIGHT_SYMBOL[] = {0xa9, '\0'};

/* ****************************************************************************************************
 * **************************************************************************************************** */
void u8g2_prepare() 
{
  /*  setFont sets the u8g2 font. */
  u8g2.setFont(u8g2_font_6x10_tf);
  /*  setFontRefHeightExtendedText is used for drawing characters on the screen. */
  u8g2.setFontRefHeightExtendedText();
/*  setDrawColor has one argument = an integer number which represents a color index for all drawing functions. 
 *  Font drawing procedures use this argument to set the foreground color. 
 *  The default value is 1. 
 *  If it is set to 0, then the space around the character is lit up, and the character is not. 
 *  Argument value 2 can also be used, but there is no difference from 0.  */
  u8g2.setDrawColor(1);
  /* setFontPosTop controls the character position in one line of the text. 
   *  The function has a couple of versions: 
   *  setFontPosBaseLine, setFontPosCenter, setFontPosBottom
   *  Their purpose is to change the position of the characters in the one line.*/
  u8g2.setFontPosTop();
  /* setFontDirection has one argument which is an integer number which represents direction of the text. 
   * 0 = 0°
   * 1 = 90°
   * 2 = 180° 
   * and 3 = 270° */
  u8g2.setFontDirection(0);
}


void u8g2_box_frame() 
{
  u8g2.drawStr(0, 0, "drawBox");
  u8g2.drawBox(5, 10, 20, 10);
  u8g2.drawStr(60, 0, "drawFrame");
  u8g2.drawFrame(65, 10, 20, 10);
}
/* ****************************************************************************************************
 * **************************************************************************************************** */
void u8g2_r_frame_box() 
{
  u8g2.drawStr(0, 0, "drawRFrame");
  u8g2.drawRFrame(5, 10, 40, 15, 3);
  u8g2.drawStr(70, 0, "drawRBox");
  u8g2.drawRBox(70, 10, 25, 15, 3);
}
void u8g2_disc_circle() 
{
  u8g2.drawStr(0, 0, "drawDisc");
  u8g2.drawDisc(10, 18, 9);
  u8g2.drawStr(60, 0, "drawCircle");
  u8g2.drawCircle(70, 18, 9);
}
void u8g2_string_orientation() 
{
  u8g2.setFontDirection(0);
  u8g2.drawStr(5, 15, "0");
  u8g2.setFontDirection(3);
  u8g2.drawStr(40, 25, "90");
  u8g2.setFontDirection(2);
  u8g2.drawStr(75, 15, "180");
  u8g2.setFontDirection(1);
  u8g2.drawStr(100, 10, "270");
}
void u8g2_line() 
{
  u8g2.drawStr(0, 0, "drawLine");
  u8g2.drawLine(7, 20, 77, 32);
}
void u8g2_triangle() 
{
  u8g2.drawStr(0, 0, "drawTriangle");
  u8g2.drawTriangle(14, 20, 45, 30, 10, 32);
}
void u8g2_unicode() 
{
  u8g2.drawStr(0, 0, "Unicode");
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.drawUTF8(10, 20, "☀");
  u8g2.drawUTF8(30, 20, "☁");
  u8g2.drawUTF8(50, 20, "☂");
  u8g2.drawUTF8(70, 20, "☔");
  u8g2.drawUTF8(95, 20, COPYRIGHT_SYMBOL); //COPYRIGHT SIMBOL
  u8g2.drawUTF8(115, 15, "\xb0"); // DEGREE SYMBOL
}

#define image_width 128
#define image_height 21
static const unsigned char image_bits[] U8X8_PROGMEM = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0x00, 0x00,
  0xfc, 0x1f, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfe, 0x1f, 0x00, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x18, 0x00, 0x00,
  0x0c, 0xc0, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x18, 0x00, 0x00, 0x0c, 0xc0, 0xf0, 0x1f, 0x06, 0x63, 0x80, 0xf1,
  0x1f, 0xfc, 0x33, 0xc0, 0x03, 0x18, 0x00, 0x00, 0x0c, 0xc0, 0xf8, 0x3f,
  0x06, 0x63, 0xc0, 0xf9, 0x3f, 0xfe, 0x33, 0xc0, 0x03, 0x18, 0x00, 0x00,
  0x0c, 0xc0, 0x18, 0x30, 0x06, 0x63, 0xc0, 0x18, 0x30, 0x06, 0x30, 0xc0,
  0xff, 0xff, 0xdf, 0xff, 0x0c, 0xc0, 0x18, 0x30, 0x06, 0x63, 0xe0, 0x18,
  0x30, 0x06, 0x30, 0xc0, 0xff, 0xff, 0xdf, 0xff, 0x0c, 0xc0, 0x98, 0x3f,
  0x06, 0x63, 0x60, 0x98, 0x3f, 0x06, 0x30, 0xc0, 0x03, 0x18, 0x0c, 0x00,
  0x0c, 0xc0, 0x98, 0x1f, 0x06, 0x63, 0x70, 0x98, 0x1f, 0x06, 0x30, 0xc0,
  0x03, 0x18, 0x06, 0x00, 0x0c, 0xc0, 0x18, 0x00, 0x06, 0x63, 0x38, 0x18,
  0x00, 0x06, 0x30, 0xc0, 0x03, 0x18, 0x03, 0x00, 0x0c, 0xe0, 0x18, 0x00,
  0x06, 0x63, 0x1c, 0x18, 0x00, 0x06, 0x30, 0xc0, 0x00, 0x80, 0x01, 0x00,
  0xfc, 0x7f, 0xf8, 0x07, 0x1e, 0xe3, 0x0f, 0xf8, 0x07, 0x06, 0xf0, 0xcf,
  0x00, 0xc0, 0x00, 0x00, 0xfc, 0x3f, 0xf0, 0x07, 0x1c, 0xe3, 0x07, 0xf0,
  0x07, 0x06, 0xe0, 0xcf, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x30, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xe0, 0x00, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0xfc, 0x1f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f
};

void u8g2_bitmap() 
{
  u8g2.drawXBMP(0, 5, image_width, image_height, image_bits);
}


void setup(void) 
{
  u8g2.begin();
  u8g2_prepare();
}

float i = 0.0;

void loop(void) 
{
  u8g2.clearBuffer();
  u8g2_prepare();
  u8g2_box_frame();
  u8g2.sendBuffer();
  delay(time_delay);
  u8g2.clearBuffer();
  u8g2_disc_circle();
  u8g2.sendBuffer();
  delay(time_delay);
  u8g2.clearBuffer();
  u8g2_r_frame_box();
  u8g2.sendBuffer();
  delay(time_delay);
  u8g2.clearBuffer();
  u8g2_prepare();
  u8g2_string_orientation();
  u8g2.sendBuffer();
  delay(time_delay);
  u8g2.clearBuffer();
  u8g2_line();
  u8g2.sendBuffer();
  delay(time_delay);
  // one tab
  u8g2.clearBuffer();
  u8g2_triangle();
  u8g2.sendBuffer();
  delay(time_delay);
  u8g2.clearBuffer();
  u8g2_prepare();
  u8g2_unicode();
  u8g2.sendBuffer();
  delay(time_delay);
  u8g2.clearBuffer();
  u8g2_bitmap();
  u8g2.sendBuffer();
  delay(time_delay);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 0);
  u8g2.print(i);
  i = i + 1.5;
  u8g2.sendBuffer();
  delay(time_delay);
}
