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
        bool          Valid=true;   // Set pour savoir si un FirstMedia est à jour ou non
        long          NextMediaPosition=0;
        // Infos sur les genres apparaissant sur la face avant (White List)
        const String  GenreWhiteList="Classique;Blues;Jazz;Folk;Rock n'Roll;Rock;Chanson;Musiques du monde;";  // Ne pas oublier le ; final

};

#endif // MEDIA_H_INCLUDED
