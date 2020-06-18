/* *******************************************************************************
 *  
 * Cette classe déclenche des Interruptions à une fréquence donnée.
 * 
 * startBeat/stopBeat = utilise le Timer1
 * 
 *  https://oscarliang.com/arduino-timer-and-interrupt-tutorial/
 *  
Timer0:
  Timer0 is a 8bit timer.
  UNO : PWM on Pins 5 and 6  are controlled by timer0
  MEGA: PWM on Pins 4 and 13 are controlled by timer0
  In the Arduino world, timer0 is used for functions like delay(), millis() and micros().
  If you change Timer0 registers, this may influence the Arduino timer function. So you should know what you are doing.
Timer1:
  Timer1 is a 16bit timer.
  UNO : PWM on Pins 9 and 10 are controlled by timer1
  MEGA: PWM on Pins 11 and 12 are controlled by timer1
  In the Arduino world, the Servo library uses timer1 on Arduino Uno.
Timer2:
  Timer2 is a 8bit timer.
  UNO : PWM on Pins 11 and 3 are controlled by timer2
  MEGA: PWM on Pins 9 and 10 are controlled by timer2
  In the Arduino world, the tone() function uses timer2.
Timer3, Timer4, Timer5: 
  Timer 3,4,5 are only available on Arduino Mega boards. 
  These timers are all 16bit timers. 
  MEGA: PWM on Pin 2,3,5    are controlled by timer 3
  MEGA: PWM on Pin 6,7,8    are controlled by timer 4
  MEGA: PWM on Pin 44,45,46 are controlled by timer 5
  In the Arduino world, the Servo library uses timer5 on Arduino Mega.
 * ********************************************************************************/
#include "Arduino.h"
#include "Rythmic.h"


// *******************************************************************************
// Constructor 
// *******************************************************************************
Rythmic::Rythmic(byte led1, byte led2)
{
  this->match = 8080;   // par defaut: 116 BPM
  this->led_beat = led1;
  this->led_fast = led2;

  pinMode(led_beat, OUTPUT);
  pinMode(led_fast, OUTPUT);
  digitalWrite(led_beat,HIGH);  // OFF
  digitalWrite(led_fast,HIGH);  // OFF
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
  TCCR2B |= (1 << CS21);    // set CS21 bit for 8 prescaler
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
  sei();                    // allow interrupts
}

/* *******************************************************************************
 * Definit la fréquence du TIMER1 et démarre le timer
 *    parameter = BeatPerMinute (mesure du tempo).
 *    Le timer se déclenchera 4 fois par mesure.
 * *******************************************************************************
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
void  Rythmic::startBeat(int beat)
{
    const float scaled_freq = 16000000 /256 *60 /4;     // = 937500.00
    match = uint16_t(scaled_freq/beat);
    Serial.print(F("ISR match counter ")); Serial.println(match);

    noInterrupts();            // désactiver toutes les interruptions
    TCCR1A = 0;                // set entire TIMER1 TCCR1A register to 0
    TCCR1B = 0;                // set entire TIMER1 TCCR1B register to 0
    TCNT1  = 0;                // initialize TIMER1 counter value to 0
    OCR1A = match;             // set CompareRegister 
    TCCR1B |= bit(WGM12);      // turn ON the CTC mode
    TCCR1B |= bit(CS12);       // set CS12 bit for 256 prescaler
    // TCCR1B |= (1 << CS12) | (1 << CS10);       // set CS12 and CS10 bits for 1024 prescaler
    TIMSK1 |= bit(OCIE1A);     // enable timer "Compare" interrupt
    interrupts();              // activer toutes les interruptions
}

/* *******************************************************************************
 * Stoppe le TIMER1
 * *******************************************************************************
 * TIMSK1 register = Timer1 Interrupt MaSK
 * TOIE1 bit       = Timer1 Overflow Interrupt Enable
 * OCIE1A bit      = Timer1 Output Compare Match Interrupt Enable
 * *******************************************************************************
 * "Compare interrupt" est générée lorsque le compteur atteint la valeur du match register.
 * "Overflow interrupt" est générée lorsque le compteur déborde 
 * *******************************************************************************
 *    Pour positionner le bit à 1 : TIMSK1 |= bit(OCIE1A);
 *    Pour positionner le bit à 0 : TIMSK1 &= ~bit (OCIE1A);
 * ******************************************************************************* */
void  Rythmic::stopBeat()
{
    Serial.println("stopBeat"); 
    cli();                        // stop interrupts
    TIMSK1 &= ~bit(OCIE1A);       // disable timer "Compare" interrupt 
    sei();                        // allow interrupts
    digitalWrite(led_beat,HIGH);  // OFF
    digitalWrite(led_fast,HIGH);  // OFF
}



//storage variables for ISR
volatile byte    beat_tempo = 0;

// *******************************************************************************
// timer1 interrupt toggles LED pin 
// (takes two cycles for full wave: toggle high then toggle low)
// *******************************************************************************
ISR(TIMER1_COMPA_vect)
{
  beat_tempo++;
  switch (beat_tempo)
  {
    case 1:
        digitalWrite(LED_4,LOW);  // mesure
        digitalWrite(LED_3,LOW);  // (fast) 4 temps
        break;
    case 2:
        digitalWrite(led_beat,LOW);
        digitalWrite(led_fast,HIGH);
        break;
    case 3:
        digitalWrite(led_beat,HIGH);
        digitalWrite(led_fast,LOW);  // (fast) 4 temps
        break;
    case 4:
        digitalWrite(led_beat,HIGH);
        digitalWrite(led_fast,HIGH);
        break;
    case 5:
        digitalWrite(led_beat,LOW);  // demi-mesure
        digitalWrite(led_fast,LOW);  // (fast) 4 temps
        break;
    case 6:
        digitalWrite(led_beat,HIGH);
        digitalWrite(led_fast,HIGH);
        break;
    case 7:
        digitalWrite(led_beat,HIGH);
        digitalWrite(led_fast,LOW);  // (fast) 4 temps
        break;
    case 8:
        digitalWrite(led_beat,HIGH);
        digitalWrite(led_fast,HIGH);
        beat_tempo=0;
        break;
  }
}
  
