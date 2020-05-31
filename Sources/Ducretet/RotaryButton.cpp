/* ***********************************************************
 *  Gestion de Bouton de type Commutateur Rotatif
 *  
 * INPUT:
 *   Pushbutton attached to pin N from +5V
 *   10K resistor attached to pin N from Ground. 
 *   Cela permet que les entrées au repos (normaly open) soient à la masse.
 *   
 * For debuging, you can add in your code:
 *   #define DEBUG
 *   #define DEBUG_ROTVALUE   1    // Value returned by Rotary Button in debug mode
 ************************************************************* */
#include "Arduino.h"
#include "RotaryButton.h"
     
#define DEBUG
#define DEBUG_ROTVALUE   2    // Value returned by Rotary Button in Debug mode

// ****************************************************************        
// Constructeur
//    4pin max => 4 positions max.
//    Note: Serial n'est pas encore initialisé.
// ****************************************************************        
Rotary::Rotary(int pin1, int pin2, int pin3=0, int pin4=0)
{
  // On initialise les variables
  Changed   = true;
  Value     = 1;    // default value
  LastValue = 0;
  Pin1=pin1;
  Pin2=pin2;
  Pin3=pin3;
  Pin4=pin4;
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  if (pin3 != 0) pinMode(pin3, INPUT);
  if (pin4 != 0) pinMode(pin4, INPUT);
}

      
        
// ****************************************************************        
// Normalement, une seule de ces positions est active à un instant donné.
// On renvoie une valeur de 1 à 4 
// ****************************************************************        
int Rotary::readPosition()
{
  LastValue=Value;
  Value=0;
  if ((Pin3!=0) && (digitalRead(Pin1)==HIGH)) Value=1;
  if ((Pin3!=0) && (digitalRead(Pin2)==HIGH)) Value=2;
  if ((Pin3!=0) && (digitalRead(Pin3)==HIGH)) Value=3;
  if ((Pin4!=0) && (digitalRead(Pin4)==HIGH)) Value=4;
  #ifdef DEBUG
  Value=DEBUG_ROTVALUE;
  #endif
  if (Value != LastValue) Changed=true;
  return Value;
}

// ****************************************************************        
// On renvoie la position lue la plus récente (valeur de 1 à 4)
// ****************************************************************        
int Rotary::getValue()
{
  Changed = false;
  return Value;
}

// ****************************************************************        
// On renvoie la position précédente (valeur de 1 à 4)
// ****************************************************************        
int Rotary::getLastValue()
{
  return LastValue;
}

// ****************************************************************        
// Indique si le bouton a changé de position depuis le dernier appel à la fonction
// ****************************************************************        
bool Rotary::hasChanged()
{
  bool ChangeStatus = Changed;
  Changed = false;
  return ChangeStatus;
}
