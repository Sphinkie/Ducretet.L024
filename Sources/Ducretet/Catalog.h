/* *******************************************************************************
 *  Gestion du Catalogue des MP3 et des recherches dans le catalogue
 ********************************************************************************/

#ifndef CATALOGCLASS_H_INCLUDED
#define CATALOGCLASS_H_INCLUDED

#include "CatalogFile.h"
#include "Plexiglass.h"
#include "Media.h"

#define SEARCH_PACKET 100    // Nombre de lignes du catalogue que l'on parcourt à chaque seconde.

typedef enum {genre, year, rating} SearchType;


// ---------------------------------------------------------------------------------------------------------------------------------
// La classe Catalog dérive de la classe CatalogFile (qui contient les primitives de lecture/ecriture dans le fichier de la carte SD
// ---------------------------------------------------------------------------------------------------------------------------------
class Catalog : CatalogFile
{
    public:
        Catalog();
        void   initialize();
        void   setNewRequestedValues(int tuning);
        void   takeClip();
        void   removeStar();
        void   addStar();
        // Initialisation des recherches
        void   initSearchForRequestedGenre();
        void   initSearchForRequestedYear();
        void   initSearchForRequestedRating();
        // Recherche d'un clip (fonction itératives)
        void   selectRandomClip();
        void   searchClipForRequestedGenre();
        void   searchClipForRequestedYear();
        void   searchClipForRequestedRating();
        // recupération des infos du clip sélectionné
        String getSelectedClipID();
        String getSelectedClipGenre();
        String getSelectedClipYear();
        String getSelectedClipRating();

    private:              
        long   searchClipInCatalog(long starting_position, Media &first_media, SearchType search_type, String requested_value);

    // ---------------------------------------------------------------------------------------------------------------------------------
    // Variables:
    // ---------------------------------------------------------------------------------------------------------------------------------
    public:
        Plexiglass    Plexi;

    private:
        bool          SearchInProgress;
        Media         CursorMedia;
        Media         NextMediaToPlay;
        Media         CurrentMedia;
        Media         FirstMediaForRequestedYear;    // Premier Media du catalogue ayant la date demandée
        Media         FirstMediaForRequestedGenre;   // Premier Media ayant le genre demandé
        Media         FirstMediaForRequestedRating;  // Premier Media ayant le rating demandé

		    // Les curseurs dans le catalogue
        long          FirstcurrentPositionY; // curseur de debut de décade (mode YEAR)
        long          FirstcurrentPositionG; // curseur de debut de genre  (mode GENRE)
        
        long          CurrentPositionY;      // curseur pour le mode YEAR
        long          CurrentPositionG;      // curseur pour le mode GENRE
        long          CurrentPositionR;      // curseur pour le mode RATING

};


#endif // CATALOGCLASS_H_INCLUDED
