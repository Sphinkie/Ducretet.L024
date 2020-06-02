#include <SPI.h>
#include "epd2in9b.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

/* *************************************************************************************
 * put your setup code here, to run once:  
 * ************************************************************************************* */
void setup() 
{
  Serial.begin(115200);
  Serial.println("e-Paper demo starting");
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
  Paint paint(image, 128, 18);    // (pointer, width, height). Width should be multiple of 8.

  /* Write "e-Paper Demo" with size 12 font : BLACK on WHITE*/
  Serial.println("Display 'e-Paper Demo'");
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "e-Paper Demo", &Font12, COLORED);
  // transmit partial data to the black part of SRAM of the e-paper display
  epd.SetPartialWindowBlack(paint.GetImage(), 24, 32, paint.GetWidth(), paint.GetHeight()); // (buffer_black, x, y, w, l) 

  /* Write "Hello world" with size 16 font: WHITE on RED*/
  Serial.println("Display 'Hello World'");
  paint.Clear(COLORED);
  paint.DrawStringAt(2, 2, "Hello world", &Font16, UNCOLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

  /* Draw a rectangle with diagonals : BLACK on WHITE */
  Serial.println("Display a rectangle");
  paint.SetWidth(64);
  paint.SetHeight(64);

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.SetPartialWindowBlack(paint.GetImage(), 8, 120, paint.GetWidth(), paint.GetHeight());

  /* Draw a circle: BLACK on WHITE */
  Serial.println("Display a circle");
  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.SetPartialWindowBlack(paint.GetImage(), 64, 120, paint.GetWidth(), paint.GetHeight());

  /* Draw a filled rectangle: RED on WHITE */
  Serial.println("Display a RED filled rectangle");
  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 8, 200, paint.GetWidth(), paint.GetHeight());

  /* Draw a filled circle: RED on WHITE */
  Serial.println("Display a RED filled circle");
  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.SetPartialWindowRed(paint.GetImage(), 64, 200, paint.GetWidth(), paint.GetHeight());

  /* This displays the data from the SRAM in e-Paper module */
  Serial.println("Display Frame");
  epd.DisplayFrame();

  /* wait 3 seconds */ 
  Serial.println("Wait 3 sec");
  delay(3000);
  Serial.println("Display Waveshare Electronics logo");
  /* This displays a bitmap with Waveshare BLACK and RED logo */
  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

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
