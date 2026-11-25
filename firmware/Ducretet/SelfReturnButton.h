/* ***********************************************************
 *  Gestion de Bouton de type SelfReturn ou PushButton
 *  par interruption matérielle
 ************************************************************** */
#ifndef SELFRETURNCLASS_H_INCLUDED
#define SELFRETURNCLASS_H_INCLUDED


typedef void (*voidFuncPtr)(void);    // callback des ISR Arduino

class SelfReturnButton
{
    public:
        SelfReturnButton(int pin, voidFuncPtr ISR_function);
        bool    wasPushed();
        void    setStatus(bool pushed);
        
    private:
        // Private Member Variables:
        // Ce qui est lié aux interruptions doit etre déclaré en volatile (cad en RAM, et non en Registre)
        volatile bool  Pushed = 0; 
 
};

#endif // SELFRETURNCLASS_H_INCLUDED
