/* ************************************************************************************* 
 * This is an example for the Adafruit VS1053 Codec Breakout
 *
 * Designed specifically to work with the Adafruit VS1053 Codec Breakout 
 * ----> https://www.adafruit.com/products/1381
 *
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.  
 * BSD license, all text above must be included in any redistribution
 * *************************************************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include "Adafruit_VS1053.h"
#include <SD.h>

// ------------------------------------------------------------------------------
// define the pins used
// ------------------------------------------------------------------------------
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"
// ------------------------------------------------------------------------------
#define CLK           52      // SPI Clock, shared with SD card ... UNO=13  MEGA=52
#define MISO          50      // Input data, from VS1053/SD card .. UNO=12  MEGA=50
#define MOSI          51      // Output data, to VS1053/SD card ... UNO=11  MEGA=51
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS      7      // VS1053 chip select pin (output)
#define SHIELD_DCS     6      // VS1053 Data/command select pin (output)
#define CARDCS         4      // Card chip select pin
#define DREQ           3      // VS1053 Data request, ideally an Interrupt pin

// create shield object
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);


// ------------------------------------------------------------------------------
// Setup
// ------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);
  Serial.println("Adafruit VS1053 Library Test");

  // initialise the music player
  if (! musicPlayer.begin()) 
  { 
     // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
 
  if (!SD.begin(CARDCS)) 
  {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  /***** Two interrupt options! *******/ 

  // Timer interrupt :
  // This option uses timer0, this means timer1 & timer2 are not required
  // (so you can use 'em for Servos, etc) BUT millis() can lose time
  // since we're hitchhiking on top of the millis() tracker.
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT);
  
  // DREQ interrupt :
  // This option uses a pin interrupt. No timers required! But DREQ
  // must be on an interrupt pin. For Uno, that's Digital #2 or #3
  // If DREQ is on an interrupt pin, we can do background audio playing.
  // See http://arduino.cc/en/Reference/attachInterrupt for other pins
  // *** This method is preferred.
  
  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
    Serial.println(F("DREQ pin is not an interrupt pin"));
}

// ------------------------------------------------------------------------------
// Loop
// ------------------------------------------------------------------------------
void loop() 
{  
  // Alternately, we can just play an entire file at once
  // This doesn't happen in the background, instead, the entire
  // file is played and the program will continue when it's done!
  musicPlayer.playFullFile("track001.ogg");

  // Start playing a file, then we can do stuff while waiting for it to finish
  if (! musicPlayer.startPlayingFile("/track001.mp3")) 
  {
    Serial.println("Could not open file track001.mp3");
    while (1);
  }
  Serial.println(F("Started playing"));

  while (musicPlayer.playingMusic) 
  {
    // file is now playing in the 'background' so now's a good time
    // to do something else like handling LEDs or buttons :)
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Done playing music");
}


// ------------------------------------------------------------------------------
// File listing 
// ------------------------------------------------------------------------------
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