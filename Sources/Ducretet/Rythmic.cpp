/* *******************************************************************************
 *  
 ********************************************************************************/
#include "Arduino.h"
#include "Rythmic.h"


// *******************************************************************************
// Constructor 
// *******************************************************************************
Rythmic::Rythmic()
{
  this->match = 31250;   // par defaut: 2 Hz
}

// *******************************************************************************
// Definit la fréquence du timer en IT par secondes
// timer speed (Hz)  = Arduino clock speed (16MHz) / prescaler
// Note sur timer2:
//     compteur 8bits => match register = 256 max
//     prescaler max = 256
//     donc sa frequence minimale = 16M/256/256 =244 Hz = 4 ms
// Prescaler allowed values: 1,8,64,256
// *******************************************************************************
void  Rythmic::setFrequency(int frequency)
{
 // set timer2 interrupt at 8 Hz

  cli();                    // stop interrupts
  TCCR2A = 0;               // set entire TIMER2 TCCR2A register to 0
  TCCR2B = 0;               // set entire TIMER2 TCCR2B register to 0
  TCNT2  = 0;               // initialize TIMER2 counter value to 0
  OCR2A  = 249;             // set CompareRegister to (16MHz /8000 /8) -1   (must be <256)
  TCCR2A |= (1 << WGM21);   // turn on CTC mode
  TCCR2B |= (1 << CS21);    // Set CS21 bit for 8 prescaler
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
  sei();                    // allow interrupts
}

/* *******************************************************************************
 * Definit la fréquence du TIMER1 et démarre le timer
 *    parameter = BeatPerMinute (mesure du tempo).
 *    Le timer se déclenchera 4 fois par mesure.
 * *******************************************************************************
 * Note sur timer0:
 *     est utilisé par les fonctions système delay() et millis().
 * Note sur timer1:
 *     compteur 16bits => match register = 65.536 max
 *     prescaler max = 1024
 *     donc sa frequence minimale = 16M/1024/65536 = 0.238 Hz = 4.19 sec
 * Prescaler allowed values: 1,8,64,256,1024
 * *******************************************************************************
 * cpu frequency       = 16.000.000 (16 MHz)
 * diviseur prescaler  = 256
 * secondes par minute = 60
 * facteur 60          = le Beat est en Beat Per Minute
 * facteur 4           = on veut une IT à chaque temps de la mesure (hypothèse = musique à 4 temps)
 * *******************************************************************************
 * Exemples:
 *   Match 1.61 Hz = 16.000.000/256/1.61   = 38819
 *   Match    2 Hz = 16.000.000/256/2      = 31250
 *   Match 135 BPM = 16.000.000/256/135*60 = 27777
 * *******************************************************************************
 * le timer incremente son compteur à 16M/prescaler = 16M/256 = 62500 Hz
 * le timer génère une IT lorsque le compteur atteint la valeur du match register
 * on calcule donc sa valeur: MatchRegister + 1 = 16M / 256 / DesiredInterruptFrequency
 * (si on veut être précis, le +1 prend en compte la RAZ du compteur).
 * ******************************************************************************* */
void  Rythmic::setBeat(int beat)
{
    const float scaled_freq = 16000000 /256 *60 /4;

    match = int(scaled_freq/beat);

    noInterrupts();            // désactiver toutes les interruptions
    TCCR1A = 0;                // set entire TIMER1 TCCR1A register to 0
    TCCR1B = 0;                // set entire TIMER1 TCCR1B register to 0
    TCNT1  = 0;                // initialize TIMER1 counter value to 0
    OCR1A = match;             // set CompareRegister 
    TCCR1B |= (1 << WGM12);    // Turn On CTC mode
    TCCR1B |= (1 << CS12);     // Set CS12 bit for 256 prescaler
    // TCCR1B |= (1 << CS12) | (1 << CS10);       // Set CS12 and CS10 bits for 1024 prescaler
    TIMSK1 |= (1 << OCIE1A);   // enable timer compare interrupt
    interrupts();              // activer toutes les interruptions
  
}
