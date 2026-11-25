/* *******************************************************************************
 *  Face-Avant en Plexiglass
 ********************************************************************************/
#ifndef PLEXIGLASS_H_INCLUDED
#define PLEXIGLASS_H_INCLUDED

class Plexiglass
{
  public:
        Plexiglass();
        void setTuning(int tuning);
        String getYear();

  private:
        String getGenreLabel(int tuning);
        int    getYearValue(int tuning);
        int    getBeatValue(int tuning);
        int    getStarsValue(int tuning);
        // debuggage
        void   debugGenre();
        void   debugYear();

  public:
        // Valeurs correspondant au Tuning reçu
        String        Genre;                 // Le Genre pointé par le bouton Tuning
        float         GenreLength;           // Correspond à la 'largeur' du Genre pointé par le bouton Tuning
        int           Year;                  // L'Année pointée par le bouton Tuning (ex: 1964)
        int           RangeStart;            // Début de la décade pointée par le Tuning (ex: 1960)
        int           RangeEnd;              // Fin de la décade pointée par le Tuning (ex: 1970)
        int           Rating;                // Le Rating pointé par le bouton Tunnig
        String        Beat;                  // Le BPM pointé par le bouton Tunnig

};

#endif // PLEXIGLASS_H_INCLUDED
