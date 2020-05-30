/* ***********************************************************
 *  Gestion de Bouton de type Commutateur Rotatif
 *  
 ************************************************************* */

#ifndef ROTARYCLASS_H_INCLUDED
#define ROTARYCLASS_H_INCLUDED


class Rotary
{
    public:
      Rotary(int pin1, int pin2, int pin3=0, int pin4=0);
      int    readPosition();
      int    getValue();
      int    getLastValue();
      bool   hasChanged();
        
    private:
      // Private member variables:
      int   Value;
      int   LastValue;
      bool  Changed;
      int   Pin1,Pin2,Pin3,Pin4;
 
};

#endif // ROTARYCLASS_H_INCLUDED
 
 
 
