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
  HasRewinded=false;
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
  CurrentMedia = this->NextMediaToPlay;       // Take clip
  NextMediaToPlay.fillWith("",0);             // NOISE
  Serial.println("takeClip: "+CurrentMedia.getID());
  SearchInProgress=true;              // On continue la recherche là où elle s'était arrêtée.
}


// *******************************************************************************
// Renvoie les infos du clip courant (après un Take)
// *******************************************************************************
String Catalog::getSelectedClipID()     { return CurrentMedia.getID();    }
String Catalog::getSelectedClipGenre()  { return CurrentMedia.getGenre(); }
int    Catalog::getSelectedClipYear()   { return CurrentMedia.getYear();  }
String Catalog::getSelectedClipRating() { return String(CurrentMedia.getRating()); }
int    Catalog::getSelectedClipBeat()   
{
  int beat = CurrentMedia.getBeat();
  if (beat==0) return 116;
  else return(beat);
}


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
  Serial.println(F("  selectRandomClip"));
  
  // On mémorise l'ID du media trouvé précédemment
  LastMedia_ID = NextMediaToPlay.getID();
  
  // On ouvre le catalogue au hasard, de façon à trouver une position initiale viable.
  FileAccessOK = CatalogFile::openCatalogAtRandomPosition();
  // En cas d'erreur d'ouverture du fichier, on renvoie "NOISE"
  if (FileAccessOK) 
  {
      medialine       = CatalogFile::readNextLine();
      CurrentPosition = CatalogFile::getCurrentPosition();
      CatalogFile::closeCatalog();      
      Serial.println(F("close Catalog.ndx"));
      NextMediaToPlay.fillWith(medialine,CurrentPosition);
  }
  else
  {
      this->NextMediaToPlay.fillWith("",0); // NOISE
  }
  // On extrait l'ID du media trouvé et du précédent
  NextMedia_ID = NextMediaToPlay.getID();
  if (LastMedia_ID==NextMedia_ID) 
    // Si on est tombé 2 fois sur le même ID, on met du Noise
    NextMediaToPlay.fillWith("",0);
  else 
    // Sinon, la recherche est terminée.
    SearchInProgress=false;
  
  Serial.print(F("  Found a random clip: ")); Serial.println (NextMediaToPlay.getID());
}


/* *******************************************************************************
 * MODE GENRE:
 *          Step 1: on se positionne sur un media random du catalogue (1 appel)
 *          Step 2: à partir de là, on cherche un media avec le RequestedGenre (N appels)
 * ******************************************************************************* */
// ******************************************************************************* 
// Initialisation des recherches de clip pour les Genres.
// Cette fonction initialise CurrentPositionG.
// ******************************************************************************* 
void Catalog::initSearchForRequestedGenre()
{
    Serial.println(F(" initSearchForRequestedGenre"));
    // On ouvre le catalogue au hasard, de façon à trouver une position initiale viable.
    CurrentPositionGenre = CatalogFile::getRandomPosition();
    // On démarre la recherche
    SearchInProgress=true;
    HasRewinded=false;
    // FirstMediaForRequestedGenre.isValid(false);
    Serial.print(F("  New CurrentPositionGenre="));  Serial.println(CurrentPositionGenre);
}
// *******************************************************************************
// Recherche un media de RequestedGenre à partir de la dernière position
// Positionne NextMediaToPlay et SearchInProgress une fois le media trouvé.
// *******************************************************************************
void Catalog::searchClipForRequestedGenre()
{
    if (SearchInProgress) 
      CurrentPositionGenre = this->searchClipInCatalog(
                                  CurrentPositionGenre, 
//                                  NULL, 
                                  genre, 
                                  Plexi.Genre);
}

/* *******************************************************************************
 * MODE BEAT:
 *          Step 1: on se positionne sur un media random du catalogue (1 appel)
 *          Step 2: à partir de là, on cherche un media avec le RequestedBeat (N appels)
 * ******************************************************************************* */
// ******************************************************************************* 
// Initialisation des recherches de clip pour les Beat Per Minuet.
// Cette fonction initialise CurrentPositionBeat.
// ******************************************************************************* 
void Catalog::initSearchForRequestedBeat()
{
    Serial.println(F(" initSearchForRequestedBeat"));
    // On ouvre le catalogue au hasard, de façon à trouver une position initiale viable.
    CurrentPositionBeat = CatalogFile::getRandomPosition();
    // On démarre la recherche
    SearchInProgress=true;
    HasRewinded=false;
    // FirstMediaForRequestedBeat.isValid(false);
    Serial.print(F("  New CurrentPositionBeat="));  Serial.println(CurrentPositionBeat);
}
// *******************************************************************************
// Recherche un media de RequestedBeat à partir de la dernière position
// Positionne NextMediaToPlay et SearchInProgress une fois le media trouvé.
// *******************************************************************************
void Catalog::searchClipForRequestedBeat()
{
    if (SearchInProgress) 
      CurrentPositionBeat = this->searchClipInCatalog(
                                  CurrentPositionBeat,
//                                  NULL, 
                                  beat, 
                                  Plexi.Beat);
}

/* *******************************************************************************
 * MODE YEAR:
 *          Step 1: on se positionne sur le premier media du catalogue (1 appel)
 *          Step 2: à partir de là, on cherche un media avec le RequestedYear (N appels)
 *          (Le premier Step2 va demander N appels, les suivants: 1 appel)
 * ******************************************************************************* */
// ******************************************************************************* 
// Initialisation des recherches de clip pour une année donnée.
// Il faut appeler cette méthode lorsque l'année demandée change, ou que l'on vient de basculer sur le mode "YEAR".
// ******************************************************************************* 
void Catalog::initSearchForRequestedYear()
{
    Serial.print(F(" initSearchForRequestedYear ")); Serial.println(Plexi.Year);
    // on se positionne au debut du Catalogue
    openCatalogAtPosition();
    CurrentPositionYear = getCurrentPosition();  // On mémorise la position courante
    closeCatalog();
    // On peut demarrer la recherche
    SearchInProgress=true;
    HasRewinded=false;
    this->FirstMediaForRequestedYear.isValid(false);
    Serial.print(F("  New CurrentPositionYear="));  Serial.println(CurrentPositionYear);
}
// *******************************************************************************
// Renvoie le clip suivant dans l'index, correspondant à une période allant 
// de l'année sélectionnée à la fin de la période.
// La fin de la période est définie dans getYearValue().
// L'index (ordonné) Catalog.ndx permet d'avancer d'année en année.
// ******************************************************************************* 
void Catalog::searchClipForRequestedYear()
{
      if (SearchInProgress) 
      CurrentPositionYear = this->searchClipInCatalog(
                                  CurrentPositionYear, 
//                                  FirstMediaForRequestedYear, 
                                  year, 
                                  Plexi.getYear());
}


/* *******************************************************************************
 * MODE RATING:
 *          Step 1: on se positionne sur un media random du catalogue (1 appel)
 *          Step 2: à partir de là, on cherche un media avec le Rating demandé (N appels)
 * ******************************************************************************* */
// ******************************************************************************* 
// Initialisation des recherches de clip pour un Rating donné.
// Il faut appeler cette méthode à chaque fois qu'un clip commence pour initier la recherche du suivant
// ******************************************************************************* 
void Catalog::initSearchForRequestedRating()
{
    Serial.print(F(" initSearchForRequestedRating ")); Serial.println(Plexi.Rating);
    this->selectRandomClip();
    // On peut demarrer la recherche
    SearchInProgress=true;
    HasRewinded=false;
    // Pour anticiper le cas EOF, on pré-remplit le FirstMedia avec un clip aléatoire.    
//    FirstMediaForRequestedRating = NextMediaToPlay;
//    FirstMediaForRequestedYear.isValid(true);
}
// ******************************************************************************* 
// On cherche un clip ayant le bon rating, à partir d'une position aléatoire à chaque fois différente.
// ******************************************************************************* 
void Catalog::searchClipForRequestedRating()
{
    // La valeur textuelle à chercher dans le catalogue
    String RequestedValue = String(Plexi.Rating);

    if (SearchInProgress) 
    {
        // Position aléatoire de debut de recherche
        CurrentPositionRating = CatalogFile::getRandomPosition();
        Serial.print(F("  New CurrentPositionRating="));  Serial.println(CurrentPositionRating);
        // recherche
        this->searchClipInCatalog(CurrentPositionRating, 
//                                  NULL, 
                                  rating, 
                                  RequestedValue);
    }
}


/* *******************************************************************************
 * Fonction de recherche générale
 * ******************************************************************************* 
 * Params:
 *   StartingPosition = Là où commencer la recherche
 *   SearchType       = le type de recherche (rating|genre|year)
 *   RequestedValue   = La valeur textuelle à chercher dans le catalogue
 * FirstMediaForRequestedYear est actualisé.
 * ******************************************************************************* 
 * Si le Media est trouvé:
 *    NextMediaToPlay  positionné
 *    SearchInProgress passe a false
 * Si rien de trouvé lors de ce passage:
 *    NextMediaToPlay  garde sa valeur précédente
 *    SearchInProgress reste à true
 * Si on arrive au bout de la decade 
 *    NextMediaToPlay  reçoit FirstMediaForRequestedYear
 *    SearchInProgress passe a false
 * Si on arrive au bout du fichier:
 *    On se remet au debut du fichier Catalogue
 *    SearchInProgress reste à true
 * ******************************************************************************* */
//unsigned long Catalog::searchClipInCatalog(unsigned long starting_position, Media &first_media, SearchType search_type, String requested_value)
unsigned long Catalog::searchClipInCatalog(unsigned long starting_position, SearchType search_type, String requested_value)
{
  bool   FileAccessOK;
  String medialine;
  unsigned long   CurrentPosition;
  bool   Found=false;
        
  // Si aucune recherche n'est demandée ou en cours: on sort.
  if (!SearchInProgress) return;
  
  Serial.println ("  searchClipInCatalog: "+requested_value);
    
  // On se positionne a la dernière position utilisée.
  FileAccessOK = CatalogFile::openCatalogAtPosition(starting_position);
  if (!FileAccessOK) 
  {
    // En cas d'erreur d'ouverture du fichier, on renvoie "NOISE"
    CursorMedia.fillWith("",0);
    return CurrentPosition;
  }

  // On lit un nombre fixe de lignes jusqu'à ce qu'on trouve une ligne du critère demandé (ou pas)
  for (int i=0; i<SEARCH_PACKET; i++)
  {
     medialine = CatalogFile::readNextLine();
     CurrentPosition = CatalogFile::getCurrentPosition();  // On mémorise la position du Media suivant dans le catalogue
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
         CursorMedia.fillWith("",0);           // NOISE
         // On reprendra la recherche depuis le début du Catalogue, au prochain passage.
         CurrentPosition=0;
         break;
     }
     // ----------------------------------------------
     // La ligne est valide: on peut l'analyser     
     // ----------------------------------------------
     CursorMedia.fillWith(medialine, CurrentPosition);
     Serial.print("    ?:"+CursorMedia.getID()+"-"); Serial.print(CursorMedia.getYear());Serial.print(" NextPos:");Serial.println(CurrentPosition);
     // ----------------------------------------------
     // On vérifie si la ligne correspond au critère
     // ----------------------------------------------
     if (search_type==rating) Found = CursorMedia.hasRating(requested_value);
     if (search_type==genre)  Found = CursorMedia.hasGenre(requested_value);
     if (search_type==beat)   Found = CursorMedia.hasBeat(requested_value);
     // --------------------------------------------------------------------------------------------------
     // Cas particulier YEAR
     // --------------------------------------------------------------------------------------------------
     if (search_type==year)
     {
         // ----------------------------------------------------------------------------------------------
         // Vérification
         // ----------------------------------------------------------------------------------------------
         int Begin;
         if (HasRewinded) Begin=Plexi.RangeStart;        // Aux autres passages, on cherche un media dans la décade (entre le début et la fin de la décade)
         else             Begin=requested_value.toInt(); // Au premier passage, on cherche entre le YEAR pointé par le Tuning et la fin de la décade
         Found = CursorMedia.hasYearBetween(Begin, Plexi.RangeEnd);
         // ----------------------------------------------------------------------------------------------
         // Détermine FirstMedia (premier media de la décade)
         // ----------------------------------------------------------------------------------------------
         if (CursorMedia.getYear()>Plexi.RangeStart)
         {
            if (!this->FirstMediaForRequestedYear.isValid()) 
            {
              FirstMediaForRequestedYear=CursorMedia;
              FirstMediaForRequestedYear.isValid(true);
              Serial.print(F("  first_media of Decade: "));Serial.println(FirstMediaForRequestedYear.getID());
            }
         }
         // ----------------------------------------------------------------------------------------------
         // Cas particulier YEAR: on est arrivé à la fin de la décade
         // ----------------------------------------------------------------------------------------------
         if (CursorMedia.getYear()>Plexi.RangeEnd)
         {
            // Si on a depassé la fin de la période, alors on rewinde.
            Serial.print(F("  End of decade. Rewinding to : ")); Serial.println(FirstMediaForRequestedYear.getID());
            NextMediaToPlay = FirstMediaForRequestedYear;
            CurrentPosition = FirstMediaForRequestedYear.getNextMediaPosition();
            SearchInProgress=false;
            HasRewinded=true;
            break;
         }
     }
     // ----------------------------------------------------------------------------------------------
     // On mémorise le media trouvé. (La recherche est terminée).
     // ----------------------------------------------------------------------------------------------
     if (Found)
     {
        NextMediaToPlay=CursorMedia;
        SearchInProgress=false; 
        Serial.print(F("  Found next media to play: "));Serial.println(NextMediaToPlay.getID());
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
  newRatingValue = updateRating(CurrentMedia.getRatingPosition(), -1);
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
  newRatingValue = updateRating(CurrentMedia.getRatingPosition(), +1);
  // Positionne le Rating du Clip En Cours.
  CurrentMedia.setRating(newRatingValue);
}
