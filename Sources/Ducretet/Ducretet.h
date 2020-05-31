/* ************************************************************************************************
 *  
 *   DUCRETET L.024
 *
 ************************************************************************************************ */

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
   
