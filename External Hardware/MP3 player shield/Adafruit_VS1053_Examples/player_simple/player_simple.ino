/************************************************************************ 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 *************************************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include "Adafruit_VS1053.h"
#include <SD.h>

// --------------------------------------------------------------------------
// define the pins used
// --------------------------------------------------------------------------
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"
#define CLK  52      // SPI Clock, shared with SD card ... UNO=13  MEGA=52
#define MISO 50      // Input data, from VS1053/SD card .. UNO=12  MEGA=50
#define MOSI 51      // Output data, to VS1053/SD card ... UNO=11  MEGA=51

#define SHIELD_RESET    -1     // VS1053 reset pin (output)
#define SHIELD_CS       7      // VS1053 chip select pin (output) 
#define SHIELD_DCS      6      // VS1053 Data/command select pin (output) 
#define CARDCS          4      // Card chip select pin
#define DREQ            3      // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);


// --------------------------------------------------------------------------
// SETUP
// --------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);
  Serial.println("Adafruit VS1053 Simple Test");

  // initialise the music player
  if (! musicPlayer.begin()) 
  { 
     // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);   // don't do anything more
  }
  Serial.println(F("VS1053 found"));
  
  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
 
  if (!SD.begin(CARDCS)) 
  {
    Serial.println(F("SD failed, or not present"));
    while (1);   // don't do anything more
  }
  Serial.println("SD OK!");

  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  /***** Two interrupt options! *******/ 

  // Timer int:
  // This option uses timer0, this means timer1 & t2 are not required
  // (so you can use 'em for Servos, etc) BUT millis() can lose time
  // since we're hitchhiking on top of the millis() tracker.
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT);

  // DREQ int:
  // This option uses a pin interrupt. No timers required! But DREQ
  // must be on an interrupt pin. For Uno, that's Digital #2 or #3
  // If DREQ is on an interrupt pin, we can do background audio playing.
  // See http://arduino.cc/en/Reference/attachInterrupt for other pins
  // *** This method is preferred.
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  
  // Play one file, don't return until complete
  Serial.println(F("Playing track 001"));
  musicPlayer.playFullFile("/track001.mp3");
  // Play another file in the background, REQUIRES interrupts!
  Serial.println(F("Playing track 002"));
  musicPlayer.startPlayingFile("/track002.mp3");
}

// --------------------------------------------------------------------------
// LOOP
// --------------------------------------------------------------------------
void loop() 
{
  // File is playing in the background
  if (musicPlayer.stopped()) 
  {
    Serial.println("Done playing music");
    while (1) 
    {
      delay(10);  // we're done! do nothing...
    }
  }
  if (Serial.available()) 
  {
    char c = Serial.read();
    // if we get an 's' on the serial console, stop!
    if (c == 's') 
  	{
      musicPlayer.stopPlaying();
    } 
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') 
    {
      if (! musicPlayer.paused()) 
      {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } 
      else 
      { 
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
  }
  delay(100);
}


// --------------------------------------------------------------------------
// File listing helper
// --------------------------------------------------------------------------
void printDirectory(File dir, int numTabs) 
{
   while(true) 
   {
     File entry =  dir.openNextFile();
     if (! entry) 
     {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) 
     {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) 
     {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } 
     else 
     {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}