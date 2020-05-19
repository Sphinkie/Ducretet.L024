/* ***********************************************************
 *  Gestion de Bouton de type Commutateur Rotatif
 *  
 * mode INPUT:
 *   Pushbutton attached to pin N from +5V
 *   10K resistor attached to pin N from Ground. 
 *   Cela permet que les entrées au repos (normaly open) soient à la masse.
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

    //variables:
      int   Value;
      int   LastValue;
      bool  Changed;
      int   Pin1,Pin2,Pin3,Pin4;
 
};

#endif // ROTARYCLASS_H_INCLUDED
 
 
 
