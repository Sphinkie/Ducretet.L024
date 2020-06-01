/* *******************************************************************************
 *  Gestion du Catalogue des MP3 et des recherches dans le catalogue
 ********************************************************************************/

#ifndef CATALOGCLASS_H_INCLUDED
#define CATALOGCLASS_H_INCLUDED

#include "CatalogFile.h"
#include "Plexiglass.h"
#include "Media.h"

#define SEARCH_PACKET 160    // Nombre de lignes du catalogue que l'on parcourt à chaque seconde.

typedef enum {genre, year, rating, beat} SearchType;


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
        void   initSearchForRequestedYear();
        void   initSearchForRequestedBeat();
        void   initSearchForRequestedGenre();
        void   initSearchForRequestedRating();
        // Recherche d'un clip (fonctions itératives)
        void   selectRandomClip();
        void   searchClipForRequestedYear();
        void   searchClipForRequestedBeat();
        void   searchClipForRequestedGenre();
        void   searchClipForRequestedRating();
        // recupération des infos du clip sélectionné
        String getSelectedClipID();
        String getSelectedClipYear();
        String getSelectedClipBeat();
        String getSelectedClipGenre();
        String getSelectedClipRating();

    private:              
        //long   searchClipInCatalog(long starting_position, Media &first_media, SearchType search_type, String requested_value);
        unsigned long   searchClipInCatalog(unsigned long starting_position, SearchType search_type, String requested_value);

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
/*        Media         FirstMediaForRequestedGenre;   // Premier Media ayant le genre demandé
        Media         FirstMediaForRequestedRating;  // Premier Media ayant le rating demandé
        Media         FirstMediaForRequestedBeat;    // Premier Media ayant le BPM demandé
*/
		    // Les curseurs dans le catalogue
//      long          FirstcurrentPositionYear;  // curseur de debut de décade (mode YEAR)
//      long          FirstcurrentPositionGenre;     // curseur de debut de Genre
//      long          FirstcurrentPositionBeat;  // curseur de debut de Beat
        
        long          CurrentPositionYear;       // curseur pour le mode YEAR
        long          CurrentPositionGenre;      // curseur pour le mode GENRE
        long          CurrentPositionRating;     // curseur pour le mode RATING
        long          CurrentPositionBeat;       // curseur pour le mode BEAT
        bool          HasRewinded;               // Indique si on a deja atteint la fin (EOF ou Decade) et que l'on est revenu au début du catalogue.

};


#endif // CATALOGCLASS_H_INCLUDED
