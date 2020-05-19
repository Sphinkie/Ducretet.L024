/* ***********************************************************
 *  Mesure de la capacité variable
 ************************************************************* */

#ifndef CAPACIMETRECLASS_H_INCLUDED
#define CAPACIMETRECLASS_H_INCLUDED

// *******************************************************************************
// Fonction statique de tri pour Qsort (tri de tableau)
// *******************************************************************************
int sort_function(const void* a, const void* b);

// *******************************************************************************
// Classe Cap Button
// *******************************************************************************
class CapButton
{
    public:
        CapButton(int in_pin,int out_pin);
        void   begin();
        int    readValue(bool debug = false);
        int    getValue();
        int    getLastValue();
        bool   hasChanged();
        void   dischargeCapacitor();
        
    private:
        int    captureMeanValue(int ecart=4);
        int    chargeAndMesure();
        int    normalizeValue(float value);
        
    //variables:
        int   Value;
        int   LastValue;
        int   PrevPrevValue;
        bool  Changed;
        bool  Stabilized;
        int   Pin_In,Pin_Out;

        const float MIN_VALUE = 30.0;   // la valeur la plus petite, jamais mesurée.
        const float MAX_VALUE = 400.0;  // la valeur la plus grande, jamais mesurée.
        // le FACTOR sert à normaliser les mesures
        const float FACTOR = 1023.0/(MAX_VALUE-MIN_VALUE);   // 2.76
        // Le status hasChanged est positionné si la variation est supérieure à l'imprecision
        const int  IMPRECISION = 3*FACTOR;                   // 8.00
        // On considère que la valeur est stable une fois qu'elle ne varie plus que dans +/- STABILITY
        const int  STABILITY   = 2*FACTOR;                   // 5.00
 
};

#endif // CAPACIMETRECLASS_H_INCLUDED
 
