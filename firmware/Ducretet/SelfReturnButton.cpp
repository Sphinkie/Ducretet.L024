/* *************************************************************
 *  Gestion de Bouton de type SelfReturn ou PushButton
 *  par interruption matérielle
 *************************************************************** */
#include "Arduino.h"
#include "SelfReturnButton.h"

// ------------------------------------------------------------------
// Constructor
// Cette fonction déclare la Pin comme Entrée de type PULL-UP
// et attache une interruption à cette Pin.
// ------------------------------------------------------------------
SelfReturnButton::SelfReturnButton(int pin, voidFuncPtr ISR_function)
{
  Pushed=false;
  pinMode(pin, INPUT_PULLUP);
  switch(pin)
  {
    // Le numéro de l'IT dépend de la Pin choisie
    case 2:  attachInterrupt(0, ISR_function, FALLING); break;
    case 3:  attachInterrupt(1, ISR_function, FALLING); break;
    case 21: attachInterrupt(2, ISR_function, FALLING); break;  // also used by I2C
    case 20: attachInterrupt(3, ISR_function, FALLING); break;  // also used by I2C
    case 19: attachInterrupt(4, ISR_function, FALLING); break;
    case 18: attachInterrupt(5, ISR_function, FALLING); break;
    // autres cas: No valid Pin number. (for Arduino Mega)  
  }
}


// ------------------------------------------------------------------
// Cette fonction renvoie VRAI si le bouton a été poussé ou tourné récemment, 
// puis remet la variable à FAUX.
// ------------------------------------------------------------------
bool SelfReturnButton::wasPushed()
{
  int Retour = Pushed;
  Pushed = false;
  return (Retour);
}


// ------------------------------------------------------------------
// ------------------------------------------------------------------
void SelfReturnButton::setStatus(bool pushed)
{
  Pushed = pushed;
}
