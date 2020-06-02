#include <SPI.h>
#include "epd2in9b.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

/* *************************************************************************************
 * put your setup code here, to run once:  
 * ************************************************************************************* */
void setup() 
{
  Serial.begin(115200);
  Serial.println("ePaperDisplayTexts");
  Epd epd;

  if (epd.Init() != 0) 
  {
    Serial.println("e-Paper init failed");
    return;
  }

  /* This clears the SRAM of the e-paper display */
  Serial.println("ClearFrame");
  epd.ClearFrame();

  /*
   * Due to RAM not enough in Arduino UNO, a frame buffer (full screen) is not allowed.
   * In this case, a smaller image buffer is allocated and you have to 
   * update a partial display several times.
   * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
   */
  Serial.println("create image buffer");
  unsigned char image[1024];      // small 1kB image buffer
  Paint paint(image, 16, 296);    // (pointer, width, height). Width should be multiple of 8.
  paint.SetRotate(ROTATE_90);

  /* Write with size 16 font : BLACK on WHITE*/
  Serial.println("Display Artist");
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "by Amy Whinehouse", &Font16, COLORED);
  // transmit partial data to the black part of SRAM of the e-paper display
  epd.SetPartialWindowBlack(paint.GetImage(), 32, 0, paint.GetWidth(), paint.GetHeight()); // (buffer_black, x, y, w, l) 

  /* Write with size 12 font: WHITE on RED*/
  Serial.println("Display Album");
  paint.Clear(COLORED);
  paint.DrawStringAt(4, 2, "Back to Black", &Font12, UNCOLORED);
  epd.SetPartialWindowBlack(paint.GetImage(), 12, 0, paint.GetWidth(), paint.GetHeight());

  /* Write with size 20 font: WHITE on RED*/
  Serial.println("Display Title");
  paint.SetWidth(18);
  paint.Clear(COLORED);
  paint.DrawStringAt(2, 2, "You Know I'm No Good", &Font20, UNCOLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 92, 0, paint.GetWidth(), paint.GetHeight());


  Serial.println("Display YEAR");
  paint.SetWidth(16);
  paint.SetHeight(64);
  paint.Clear(COLORED);
  paint.DrawStringAt(1, 2, "2006", &Font20, UNCOLORED);  // Marges bas/gauche
  epd.SetPartialWindowBlack(paint.GetImage(), 64, 180, paint.GetWidth(), paint.GetHeight()); // positions: from bottom / from left border.

  delay(5000);

  /* This displays the data from the SRAM in e-Paper module */
  Serial.println("Display Frame");
  epd.DisplayFrame();

  /* Deep sleep */
  Serial.println("Deep Sleep");
  epd.Sleep();
}

/* *************************************************************************************
 * put your main code here, to run repeatedly:
 * ************************************************************************************* */
void loop() 
{

}
