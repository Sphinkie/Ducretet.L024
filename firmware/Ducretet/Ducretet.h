/* ************************************************************************************************
 *  
 *   DUCRETET L.024
 *
 *  Target : Arduino MEGA or MEGA2560
 ************************************************************************************************ */

#ifndef DUCRETET_H_INCLUDED
#define DUCRETET_H_INCLUDED

   // -----------------------------------------------------
   // Declaration des fonctions Interrupt Forward 
   // -----------------------------------------------------
   void ISR_NextButton();
   void ISR_AgainButton();
//   void ISR_PromoteButton();

   // -----------------------------------------------------
   // Boutons Poussoirs
   // -----------------------------------------------------
   #define _IDLE    0
   #define _NEXT    1
   #define _AGAIN   2
//   #define _PROMOTE 3

   // -----------------------------------------------------
   // Valeurs retournées par ROTARY BUTTON
   // -----------------------------------------------------
   #define GENRE   1
   #define YEAR    2
   #define BEAT    3
   #define RANDOM  4
   #define RATING  5  // pour Radial-V uniquement
   #define FAV     6  // pour Radial-V uniquement

   
   // -----------------------------------------------------
   // Mapping du cablage
   // -----------------------------------------------------

   // ------------------ Pour MP3 Shield
   // Midi_In              //      NOT USED    (MP3 shield) avec hardware interrupt
   // GPIO                 //      GPIO        (MP3 shield)
   // MP3_RESET            //      NOT USED    (MP3 shield) VS1053 reset pin (unused!)
   #define MP3_DREQ   3    // D3   DataRequest (MP3 shield) avec hardware interrupt 1. VS1053 Data REQuest, ideally an Interrupt pin.
   #define SD_CS      4    // D4   SD CS       (MP3 shield) SD-Card chip select pin
   #define MP3_DCS    6    // D6   MP3 Data CS (MP3 shield) VS1053 Data/Command S  elect pin (output)
   #define MP3_CS     7    // D7   MP3 CS      (MP3 shield) VS1053 Chip Select pin (output)

   #define AGAIN      2    // D2   Digital In     avec hardware interrupt 0
   #define NEXT       18   // D18  Digital In     avec hardware interrupt 5
   #define PROMOTE    19   // D19  Digital In     avec hardware interrupt 4

   #define MODE_4     22   // D22  input   C-MODE-4    bouton Mode
   #define MODE_3     24   // D24  input   C-MODE-3    bouton Mode
   #define MODE_2     26   // D26  input   C-MODE-2    bouton Mode
   #define MODE_1     28   // D28  input   C-MODE-1    bouton Mode
   #define SPARE1     37   // D37  Spare1 Connector
   #define SPARE_LED  43   // D43  input   SPARE LED (connector JP5)

   #define LED_1      29   // D  output  LED
   #define LED_2      27   // D  output  LED
   #define LED_3      25   // D  output  LED
   #define LED_4      23   // D  output  LED
  
   #define SPARE2     49   // D49  Spare2 Connector

   // ------------------ Pour SPI
   #define SPI_MISO   50    // D50  input  
   #define SPI_MOSI   51    // D51  output 
   #define SPI_SCLK   52    // D52  output 
   #define SPI_SS     53    // D53  input  (configuré en output car Master)

   #define TUNE_OUT   A8    // Analog output for bouton Tune (charge pin): Créneaux de 5v.
   #define TUNE_IN    A9    // Analog input for bouton Tune: Read value



#endif // DUCRETET_H_INCLUDED
