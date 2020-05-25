/* *******************************************************************************
 *  Gestion d'un MEDIA du catalogue
 ********************************************************************************/
#ifndef MEDIA_H_INCLUDED
#define MEDIA_H_INCLUDED


class Media
{
  public:
        Media();
        void   fillWith(String medialine, long Nextmediaposition);
        // Setters
        void   setNextMediaPosition(long pos);
        void   setRating(int rating);
        // Getters
        int    getYear();
        String getID();
        String getGenre();
        int    getRating();
        long   getNextMediaPosition();
        long   getRatingPosition();
        // Checkers
        bool   isValid();
        void   isValid(bool validity);
        bool   hasRating(String rating);
        bool   hasGenre(String genre);
        bool   hasYearBetween(int YearStart, int YearEnd);
        // Operateur d'affectation
        Media& operator=(const Media& media_source);

  private:
        String        Field1;       // year
        String        Field2;       // media_id
        String        Field3;       // genre
        String        Field4;       // rating
        String        Field5;       // BeatPerMinute
        bool          Valid=true;   // Set pour savoir si un FirstMedia est à jour ou non
        bool          WhiteListed;  // True si le genre fait partie de la Whitelist
        long          NextMediaPosition=0;
        // Liste des genres apparaissant sur la face avant (White List)
        const String  GenreWhiteList="Classique;Blues;Jazz;Folk;Rock n'Roll;Rock;Chanson;Musiques du monde;";  // Ne pas oublier le ; final

};

#endif // MEDIA_H_INCLUDED
