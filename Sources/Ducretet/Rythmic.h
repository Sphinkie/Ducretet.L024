/* ***********************************************************
 * Cette classe déclenche des Interruptions à une fréquence donnée.
 ************************************************************* */

#ifndef RYTHMIC_H_INCLUDED
#define RYTHMIC_H_INCLUDED

class Rythmic
{
  public:
    Rythmic(byte led1, byte led2);
    void setFrequency(int frequency);
    void startBeat(int beat);
    void stopBeat();

  private:
    unsigned int match;
    byte led_beat;
    byte led_fast;
 
};

#endif
