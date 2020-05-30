/* *******************************************************************************
 *  Mesure de la capacité variable
 *  
 * Capacité à mesurer:
 *   La capacité à mesure est entre in_pin (Analog) et out_pin (Analog)
 * 
 * Capacité parasite (Stray):
 *   Capacitance between Pin_In and Ground.
 *   Stray capacitance is always present. 
 *   Extra capacitance can be added to allow higher capacitance to be measured.
 ********************************************************************************* */
#include "Arduino.h"
#include "CapButton.h"

#define DEBUG
#define DEBUG_CAPMESUR 250    // Average value (+/-5) measured by Capacity Button in Debug mode (not normalized)



// *******************************************************************************
// Constructor
// *******************************************************************************
CapButton::CapButton(int in_pin, int out_pin)
{
 // On initialise les variables
  Pin_In        = in_pin;   // Discharge pin.
  Pin_Out       = out_pin;  // Charge pin.  Créneaux de 5v.
  PrevPrevValue = 0;
  LastValue     = 0;
  Value         = 0;
  Changed       = false;
  Stabilized    = true;

  // On initialise les outputs a leur état par defaut
  pinMode(out_pin, OUTPUT);
  digitalWrite(out_pin, LOW);
  pinMode(in_pin, OUTPUT);
  digitalWrite(in_pin, LOW);
}

// *******************************************************************************
// Initialisations
// *******************************************************************************
void CapButton::initialize()
{
  this->dischargeCapacitor();
  // On dépoussière en lisant quelques premières valeurs
  this->_captureMeanValue();
}

// *******************************************************************************
// On lit et renvoie la position de l'entrée analogique
// *******************************************************************************
int CapButton::readValue(bool debug = false)
{
  // On met à jour l'historique
  PrevPrevValue=LastValue;
  LastValue=Value;

  // On mesure la valeur courante (moyenne de plusieurs captures)
  Value = this->_captureMeanValue();
  // Au debut, on aligne les 3 valeurs de l'historique
  if (PrevPrevValue==0) PrevPrevValue=LastValue=Value;

  // On détermine les variations
  int Variation         = abs(Value-LastValue);
  int PreviousVariation = abs(LastValue-PrevPrevValue);
  int GlobalVariation   = abs(Value-PrevPrevValue);
  
  // Il y a changement si la dernière valeur lue s'écarte de la précédente de +/- IMPRECISION.
  if (Variation>IMPRECISION) {Changed=true;  Stabilized=false; }
  // On vérifie si le changement n'a pas été éphémère.
  // (cad si la variation actuelle ne fait que compenser la variation précédente).
  if ((Variation>IMPRECISION) and (GlobalVariation<IMPRECISION)) {Changed=false; }

  // Il y a Stabilisation si les trois valeurs de l'historique sont identiques (a l'imprécision près)
  Stabilized = (Variation<STABILITY) and (PreviousVariation<STABILITY);
    
  if (debug)
  {
    Serial.print(Value);
    /*
    Serial.print(": PrevPrevValue="+String(PrevPrevValue)); 
    Serial.print(" LastValue="+String(LastValue)); 
    Serial.print(" Value="+String(Value));
    if (Changed) Serial.print(F(" Changed"));
    if (Variation>IMPRECISION) Serial.print(" ("+String(Value-LastValue)+")");
    if (Stabilized) Serial.print(" Stabilized ("+String(PreviousVariation)+") ("+String(Variation)+")");
    */
    Serial.println();
  }
  return Value;
}

// *******************************************************************************
// On renvoie la position lue la plus récente (valeur de 0 à 1023)
// *******************************************************************************
int CapButton::getValue()
{
  return Value;
}

// *******************************************************************************
// On renvoie la position lue précédente (valeur de 0 à 1023)
// *******************************************************************************
int CapButton::getLastValue()
{
    return LastValue;
}

// *******************************************************************************
// Indique si le bouton a changé de position depuis le dernier appel à la fonction
// et s'est stabilisé.
// *******************************************************************************
bool CapButton::hasChanged()
{
  bool ChangedAndStabilized = (Changed and Stabilized);
  if (ChangedAndStabilized) 
  {
     Changed = false;
     // Serial.print(F("  IMPRECISION=")); Serial.print(IMPRECISION);
     // Serial.print(F("  STABILITY=")); Serial.println(STABILITY);
  }
  return ChangedAndStabilized;
}

// *******************************************************************************
// Décharge la Capa en mettant les deux pins à LOW (=GND)
// *******************************************************************************
void CapButton::dischargeCapacitor()
{
   pinMode(Pin_Out, OUTPUT);
   pinMode(Pin_In,  OUTPUT);
   digitalWrite(Pin_In,  LOW);
   digitalWrite(Pin_Out, LOW);
   delay (200);
}

// *******************************************************************************
// Effectue plusieurs mesures pour obtenir une valeur estimée.
//   ecart   = ecart entre chaque mesure (en ms)
//   retour  = valeur de la mesure, normalisée [entre 0 et 1023].
// *******************************************************************************
int CapButton::_captureMeanValue(int ecart=4)
{
  int  nbsamples =11;  // prendre un nombre impair
  int  index_mediane = (int)(nbsamples/2)+1;
  int  mediane;
  int  value;
  int  tableau[nbsamples];
  
  // On met les mesures dans un tableau
  for (int i=0; i<nbsamples; i++)
  {
     tableau[i] = this->_chargeAndMesure();
     delay(ecart);
  }

  // On trie le tableau
  qsort(tableau, nbsamples, sizeof(int), sort_function);
  //for (int i=0; i<nbsamples; i++) {Serial.print(" "); Serial.print(tableau[i]);}  Serial.println();
  
  /* Il y a plusieurs façons d'estimer la valeur vraie:
   * Méthode 1: la médiane (la moitié des mesures sont en dessous de la médiane)
   * Méthode 2: la moyennne réduite (on enlève 1 (ou 2) valeur à chaque extrémité, et on fait la moyenne).
   * Méthode 3: on enleve les mesures aberrantes que l'on peut trouver grace à la méthode de Dixon ou de Gubbs.
   */
  
  mediane = tableau[index_mediane];
  value=this->_normalizeValue(mediane);
  // Serial.print(F("  mediane= ")); Serial.print(mediane);
  // Serial.print(F("  valeur lue calibree= "));  Serial.print(value);
  return (value);
}
        
// *******************************************************************************
// Effectue une lecture de Pin_In (mesure théoriquement entre 0 et 1023).
// On envoie une impulsion sur Pin_Out et 0.1ms plus tard, on lit la valeur de Pin_In
// *******************************************************************************
int CapButton::_chargeAndMesure()
{
   int measure;
   // On génère un Front Montant (charge) sur Pin_Out
   pinMode(Pin_In, INPUT);
   digitalWrite(Pin_Out, HIGH);   // Durée avant exécution = environ 2ms
   // On lit la valeur sur Pin_In (pendant la charge)
   measure = analogRead(Pin_In);  // Durée environ 0.1ms
   // On redescend le niveau sur Pin_Out (le creneau a duré 0.1 ms)
   digitalWrite(Pin_Out, LOW);
   pinMode(Pin_In, OUTPUT);
#ifdef DEBUG
   measure = random(DEBUG_CAPMESUR-2, DEBUG_CAPMESUR+2);
#endif
   return (measure);
}

// *******************************************************************************
// Normalise la valeur.
// Etale la mesure qui est comprise entre MIN_MESURE et MAX_MESURE en une valeur comprise entre 0 et 1023
// *******************************************************************************
int CapButton::_normalizeValue(float value)
{
  // Normalisation linéaire inversée:
  value = MAX_MESURE - value;
  value = value * FACTOR;
  /*
  // Normalisation logarithmique:
  value = log10(value-MIN_MESURE);  // value varie entre 1.2 et 2.3
  value = (value-1)*755;            // on étale entre 0 et 1023     
  */
  // Gestion des limites:
  value = max(0,value);
  value = min(value,1023);

  return (int(value));
}

// *******************************************************************************
// Fonction statique de tri pour Qsort (tri de tableau)
// *******************************************************************************
int sort_function(const void* a, const void* b)
{
    // Cast les void * en int *
    const int* ia = (const int*)a;
    const int* ib = (const int*)b;
    // Comparaison (positif pour mettre "a" en fin de tableau)
    return (*ia  - *ib);
}
