/* *******************************************************************************
 *  Gestion du Catalogue des MP3 et des recherches dans le catalogue
 ********************************************************************************/
#include "Arduino.h"
#include "Catalog.h"
#include "Plexiglass.h"
#include "Media.h"


// *******************************************************************************
// Constructor (avec Classe de Base = CatalogFile)
// *******************************************************************************
Catalog::Catalog() : CatalogFile()
{
  SearchInProgress=false;
}

// *******************************************************************************
// N'a besoin d'être fait qu'une fois, mais pas immédiatement car la carte SD doit être prête.
// *******************************************************************************
void Catalog::initialize()
{
  CatalogFile::begin();
}

// *******************************************************************************
// Il faut appeler cette méthode lorsque le tuning a changé.
// On déduit les valeurs textuelles du Genre/Year/Rating demandé par le bouton Tuning
// *******************************************************************************
void Catalog::setNewRequestedValues(int tuning)
{
  Plexi.setTuning(tuning);
}

// *******************************************************************************
// Lors du take, on remplit le CurrentClip et On continue la recherche là où elle s'était arrêtée.
// *******************************************************************************
void Catalog::takeClip()
{
  CurrentMedia=NextMediaToPlay;       // Take clip
  NextMediaToPlay.fillWith("",0);     // NOISE
  Serial.println("  takeClip: "+CurrentMedia.getID());
  SearchInProgress=true;              // On continue la recherche là où elle s'était arrêtée.
}


// *******************************************************************************
// Renvoie les infos du clip courant (après un Take)
// *******************************************************************************
String Catalog::getSelectedClipID()     { return CurrentMedia.getID(); }
String Catalog::getSelectedClipGenre()  { return CurrentMedia.getGenre(); }
String Catalog::getSelectedClipYear()   { return String(CurrentMedia.getYear()); }
String Catalog::getSelectedClipRating() { return String(CurrentMedia.getRating()); }


/* *******************************************************************************
 * MODE RANDOM: 
 * Sélectionne un clip au hasard, tiré dans le catalogue.
 * Le resultat est dans : NextMediaToPlay
 * ******************************************************************************* */
void Catalog::selectRandomClip()
{
  String  medialine;
  long    CurrentPosition;
  bool    FileAccessOK;
  String  LastMedia_ID;
  String  NextMedia_ID;

  // Si aucune recherche n'est demandée ou en cours: on sort.
  if (!SearchInProgress) return;
  
  // On ouvre le catalogue au hasard, de façon à trouver une position initiale viable.
  FileAccessOK = CatalogFile::openCatalogAtRandomPosition();
  // En cas d'erreur d'ouverture du fichier, on renvoie "NOISE"
  if (!FileAccessOK) this->NextMediaToPlay.fillWith("",0); // NOISE
  medialine       = CatalogFile::readNextLine();
  CurrentPosition = CatalogFile::getCurrentPosition();
  CatalogFile::closeCatalog(); 

  if ((medialine=="ERROR") || (medialine=="EOF"))
  {
     NextMediaToPlay.fillWith("",0);   // Noise
  }

  // On a trouvé une ligne valide.
  // On extrait l'ID du media trouvé et du précédent
  LastMedia_ID = NextMediaToPlay.getID();
  NextMediaToPlay.fillWith(medialine,CurrentPosition);
  NextMedia_ID = NextMediaToPlay.getID();
  if (LastMedia_ID==NextMedia_ID) 
    // Si on est tombé 2 fois sur le même ID, on met du Noise
    NextMediaToPlay.fillWith("",0);
  else 
    // Sinon, la recherche est terminée.
    SearchInProgress=false;
  
  Serial.print(F("  Found: ")); Serial.println (NextMediaToPlay.getID());

}


/* *******************************************************************************
 * MODE GENRE:
 *          Step 1: on se positionne sur un media random du catalogue (1 appel)
 *          Step 2: à partir de là, on cherche un media avec le RequestedGenre (N appels)
 * *******************************************************************************
 * Initialisation des recherches de clip pour les genres.
 * Cette fonction initialise CurrentPositionG.
 * ******************************************************************************* */
void Catalog::initSearchForRequestedGenre()
{
    Serial.println(F(" initSearchForRequestedGenre"));
    // On ouvre le catalogue au hasard, de façon à trouver une position initiale viable.
    CurrentPositionG = CatalogFile::getRandomPosition();
    // On peut demarrer la recherche
    SearchInProgress=true;
    FirstMediaForRequestedGenre.isValid(false);
    Serial.print(F("  New CurrentPositionG="));  Serial.println(CurrentPositionG);
}
// *******************************************************************************
// Recherche un media de RequestedGenre à partir de la deniere position
// Positionne NextMediaToPlay une fois le media trouvé.
// *******************************************************************************
void Catalog::searchClipForRequestedGenre()
{
    if (SearchInProgress) 
      CurrentPositionG = this->searchClipInCatalog(
                                  CurrentPositionG, 
                                  FirstMediaForRequestedGenre, 
                                  genre, 
                                  Plexi.Genre);
}

/* *******************************************************************************
 * MODE YEAR:
 *          Step 1: on se positionne sur le premier media du catalogue (1 appel)
 *          Step 2: à partir de là, on cherche un media avec le RequestedYear (N appels)
 *          (Le premier Step2 va demander N appels, les suivants: 1 appel)
 * *******************************************************************************
 * Initialisation des recherches de clip pour une année donnée.
 * Il faut appeler cette méthode lorsque l'année demandée change, ou que l'on vient de basculer sur le mode "YEAR".
 * ******************************************************************************* */
void Catalog::initSearchForRequestedYear()
{
  Serial.print(F(" initSearchForRequestedYear ")); Serial.println(Plexi.Year);
  // on se positionne au debut du Catalogue
  openCatalogAtPosition();
  CurrentPositionY = getCurrentPosition();  // On mémorise la position courante
  closeCatalog();
  // On peut demarrer la recherche
  SearchInProgress=true;
  FirstMediaForRequestedYear.isValid(false);
  Serial.print(F("  New CurrentPositionY="));  Serial.println(CurrentPositionY);
}
/* *******************************************************************************
 * Renvoie le clip suivant dans l'index, correspondant à une periode allant 
 * de l'année sélectionnée à la fin de la période.
 * La fin de la période est définie dans getYearValue().
 * L'index (ordonné) Catalog.ndx permet d'avancer d'année en année.
 * ******************************************************************************* */
void Catalog::searchClipForRequestedYear()
{
      if (SearchInProgress) 
      CurrentPositionY = this->searchClipInCatalog(
                                  CurrentPositionY, 
                                  FirstMediaForRequestedYear, 
                                  year, 
                                  Plexi.getYear());
}


/* *******************************************************************************
 * MODE RATING:
 *          Step 1: on se positionne sur un media random du catalogue (1 appel)
 *          Step 2: à partir de là, on cherche un media avec le Rating demandé (N appels)
 * *******************************************************************************
 * Initialisation des recherches de clip pour un Rating donné.
 * Il faut appeler cette méthode à chaque fois qu'un clip commence pour initier la recherche du suivant
 * ******************************************************************************* */
void Catalog::initSearchForRequestedRating()
{
    Serial.print(F(" initSearchForRequestedRating ")); Serial.println(Plexi.Rating);
    // Pour anticiper le cas EOF, on prermplit le FirstMedia avec un valeur aléatoire.    
    this->selectRandomClip();
    FirstMediaForRequestedRating = NextMediaToPlay;
    // On peut demarrer la recherche
    SearchInProgress=true;
}
/* *******************************************************************************
 * On cherche un clip ayant le bon rating, à partir d'une position aléatoire à chaque fois différente.
 * ******************************************************************************* */
void Catalog::searchClipForRequestedRating()
{
    // La valeur textuelle à chercher dans le catalogue
    String RequestedValue = String(Plexi.Rating);

    if (SearchInProgress) 
    {
        // Position aléatoire de debut de recherche
        CurrentPositionR = CatalogFile::getRandomPosition();
        Serial.print(F("  New CurrentPositionR="));  Serial.println(CurrentPositionR);
        // recherche
        this->searchClipInCatalog(CurrentPositionR, 
                                  FirstMediaForRequestedRating, 
                                  rating, 
                                  RequestedValue);
    }
}



/* *******************************************************************************
 * Fonction de recherche générale (test)
 * Params:
 *   StartingPosition = Là où commencer la recherche
 *   Le first Media, sur lequel se replier en cas de EOF (passage par référence, car on va le modifier)
 *   RequestedValue = La valeur textuelle à chercher dans le catalogue
 * ******************************************************************************* */
long Catalog::searchClipInCatalog(long starting_position, Media &first_media, SearchType search_type, String requested_value)
{
  bool   FileAccessOK;
  String medialine;
  long   CurrentPosition;
  bool   Found=false;
        
  // Si aucune recherche n'est demandée ou en cours: on sort.
  if (!SearchInProgress) return;
  
  Serial.println ("  searchClipInCatalog: "+requested_value);
    
  // On se positionne a la dernière position utilisée.
  FileAccessOK = this->openCatalogAtPosition(starting_position);
  // En cas d'erreur d'ouverture du fichier, on renvoie "NOISE"
  if (!FileAccessOK) CursorMedia.fillWith("",0); // NOISE

  // On lit des lignes jusqu'à ce qu'on trouve une ligne du critère demandé (ou pas)
  for (int i=0; i<SEARCH_PACKET; i++)
  {
     // On lit une ligne
     // long pos = this->getCurrentPosition();  // position de debut de ligne
     medialine = this->readNextLine();
     CurrentPosition = this->getCurrentPosition();  // On mémorise la position du Media suivant dans le catalogue
     // Serial.print (F("  line read in Catalog.ndx[")); Serial.print(pos); Serial.print("]: "); Serial.println (medialine);
     // ----------------------------------------------
     // Cas d'une ligne erronée:
     // ----------------------------------------------
     if (medialine=="ERROR") 
     {
        CursorMedia.fillWith("",0);   // NOISE
        // On sort, on reprendra la recherche au prochain appel;
        break;
     }
     // ----------------------------------------------
     // Si on est arrivé à la fin du fichier catalogue
     // ----------------------------------------------
     if (medialine=="EOF")
     {
        if (first_media.isValid())
        {        
            Serial.println("  Rewind to: "+first_media.getID());
            // On rewind sur le premier et la recherche est terminée.
            NextMediaToPlay=first_media;
            CurrentPosition=first_media.getNextMediaPosition();
            SearchInProgress=false;
            break;
        }
        else
        {
            // On n'a pas de First media: on reprend la recherche depuis le début.
            CursorMedia.fillWith("",0);   // NOISE
            CurrentPosition=0;
            break;
        }      
     }
     // ----------------------------------------------
     // La ligne est valide: on peut l'analyser     
     // ----------------------------------------------
     CursorMedia.fillWith(medialine, CurrentPosition);
     // ----------------------------------------------
     // Si la ligne correspond au critère
     // ----------------------------------------------
     if (search_type==rating) Found = CursorMedia.hasRating(requested_value);
     if (search_type==genre)  Found = CursorMedia.hasGenre(requested_value);
     if (search_type==year)   Found = CursorMedia.hasYearBetween(requested_value.toInt(), Plexi.RangeEnd);
     if (Found)
     {
        // Si on a trouvé notre FirstMedia: on le mémorise
        if (!first_media.isValid())
        {
          first_media=CursorMedia;
          Serial.print(F("  first_media set to: "));Serial.println(first_media.getID());
        }
        // On retourne le media trouvé
        NextMediaToPlay=CursorMedia;
        SearchInProgress=false; // La recherche est terminée
        Serial.println("  Found: "+NextMediaToPlay.getID());
        break;
     }
     // ----------------------------------------------
     // Cas particulier YEAR: Le FirstMedia
     // ----------------------------------------------
     // Le FirstMediaForYear correspond au premier media de la décade, et pas au premier qui correspond au critère.
     if ((search_type==year) && (CursorMedia.getYear()>Plexi.RangeStart) )
     {
        if (!first_media.isValid()) 
        {
          first_media=CursorMedia;
          Serial.print(F("  first_media set to Start of Decade: "));Serial.println(first_media.getID());
        }
     }
     // ----------------------------------------------
     // Cas particulier YEAR: on est arrivé à la fin de la décade
     // ----------------------------------------------
     if ((search_type==year) && (CursorMedia.getYear()>Plexi.RangeEnd) )
     {
        // Si on a depassé la fin de la période, alors on rewinde.
        Serial.println("  End of decade. Rewinding to : "+first_media.getID());
        NextMediaToPlay  = first_media;
        CurrentPositionY = first_media.getNextMediaPosition();
        SearchInProgress=false;
        break;
     }
  } 
  this->closeCatalog();
  return CurrentPosition;
}


// *******************************************************************************
// Enleve une Star dans le media courant et dans le fichier index
// *******************************************************************************
void Catalog::removeStar()
{
  int newRatingValue; // valeur du rating apres décrementation
  // décremente le Rating dans le fichier.
  newRatingValue = writeRemoveStar(CurrentMedia.getRatingPosition());
  // Positionne le Rating du Clip En Cours.
  CurrentMedia.setRating(newRatingValue);
}

// *******************************************************************************
// Ajoute une Star dans le media courant et dans le fichier index
// *******************************************************************************
void Catalog::addStar()
{
  int newRatingValue; // valeur du rating apres décrementation
  // décremente le Rating dans le fichier.
  newRatingValue = writeAddStar(CurrentMedia.getRatingPosition());
  // Positionne le Rating du Clip En Cours.
  CurrentMedia.setRating(newRatingValue);
}
