/* ************************************************************************************************
 *  
 *   RADIAL-V
 *
 * Software pour la radio Radial-V.
 ************************************************************************************************ */

   typedef void (*voidFuncPtr)(void);

   // -----------------------------------------------------
   // Declaration des fonctions Interrupt forward 
   // -----------------------------------------------------
   void ISR_NextButton();
   void ISR_AgainButton();
   void ISR_PromoteButton();

   // -----------------------------------------------------
   // Constantes
   // -----------------------------------------------------
   #define _IDLE    0
   #define _NEXT    1
   #define _AGAIN   2
   #define _PROMOTE 3
