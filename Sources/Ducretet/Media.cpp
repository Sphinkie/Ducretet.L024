/* *******************************************************************************
 *  Gestion d'un MEDIA du catalogue
 ********************************************************************************/
#include "Arduino.h"
#include "Media.h"


// *******************************************************************************
// Constructeur
// *******************************************************************************
Media::Media()
{
    this->Field1 = "0000";
    this->Field2 = "NOISE";
    this->Field3 = "-";
    this->Field4 = "-";
    this->Valid  = true;
    this->NextMediaPosition=0;
}

 
// *******************************************************************************
// Parsing des 4 champs d'une ligne de catalogue  (ex: 1956;E76E79B1;Calipso;0; )
// *******************************************************************************
void Media::fillWith(String medialine, long nextmediaposition)
{
  // Serial.print("  fillWith: "); Serial.println (medialine);
  if (medialine.length()<18) 
  {
    // Si la ligne est trop courte pour être exploitable, on met les valeurs par défaut
    this->Field1 = "0000";
    this->Field2 = "NOISE";
    this->Field3 = "-";
    this->Field4 = "-";
    this->NextMediaPosition=0;
  }
  else
  {
    this->NextMediaPosition = nextmediaposition;
    // Si la ligne a une longueur correcte, on fait le parsing
    int    Separ1   = medialine.indexOf(';');              // pointeur sur le premier séparateur
    int    Separ2   = medialine.indexOf(';',Separ1+1);     // pointeur sur le second séparateur
    int    Separ3   = medialine.indexOf(';',Separ2+1);     // pointeur sur le troisième séparateur
    int    Separ4   = medialine.indexOf(';',Separ3+1);     // pointeur sur le quatrième séparateur
    this->Field1 = medialine.substring(0       ,Separ1);
    this->Field2 = medialine.substring(Separ1+1,Separ2);
    this->Field3 = medialine.substring(Separ2+1,Separ3);          
    this->Field4 = medialine.substring(Separ3+1,Separ4);
  }

  // Si après le parsing, un des champs est vide, on met la valeur par défaut
  if (this->Field1.length()==0) this->Field1="0000";
  if (this->Field2.length()==0) this->Field2="NOISE";
  if (this->Field3.length()==0) this->Field3="-";
  if (this->Field4.length()==0) this->Field4="-";
}

// *******************************************************************************
// Autres fonctions: Setters et Getters
// *******************************************************************************
int    Media::getYear()                {return Field1.toInt();}
String Media::getID()                  {return Field2;}
String Media::getGenre()               {return Field3;}
int    Media::getRating()              {return Field4.toInt();}
bool   Media::isValid()                {return Valid;}
void   Media::isValid(bool validity)   {Valid=validity;}
long   Media::getNextMediaPosition()   {return NextMediaPosition;}
long   Media::getRatingPosition()      {return NextMediaPosition-4;}
void   Media::setRating(int rating)    {Field4=String(rating);}
void   Media::setNextMediaPosition(long pos)    {NextMediaPosition=pos;}


// *******************************************************************************
// Renvoie TRUE si l'année du média est comprise entre les deux bornes
// *******************************************************************************
bool Media::hasYearBetween(int YearStart, int YearEnd)
{
  int year = this->Field1.toInt();
  return ((year>=YearStart) && (year<YearEnd));
}


// *******************************************************************************
// Renvoie TRUE si le genre du média est egal au Genre demandé.
// Renvoie TRUE si le genre du média est "hors-WhiteList" et le genre demandé est "*"
// *******************************************************************************
bool Media::hasGenre(String genre)
{
  //  Serial.println ("hasGenre "+genre);
  if (genre=="*") 
    // si le genre attendu est Others, alors on regarde si le genre lu est dans la Whitelist
    // Si il est dedans : on renvoie FALSE.
    {
      Serial.print (" others:"+Field3+":"); Serial.println (GenreWhiteList.indexOf(Field3));
      return (GenreWhiteList.indexOf(this->Field3)==-1); // -1=>on n'a pas trouvé=> on renvoie VRAI
    }
  else 
    // si le genre attendu est defini, on le compare au genre lu
    return (this->Field3.equalsIgnoreCase(genre));
}

// *******************************************************************************
// Renvoie TRUE si le Rating du média est egal au Rating demandé.
// *******************************************************************************
bool Media::hasRating(String rating)
{
  //  Serial.println ("hasRating "+rating);
  if (rating=="*") 
    // si le rating attendu est 'All', alors on renvoie TRUE
    {
      return true;
    }
  else 
    // sinon on compare 
    return (this->Field4.equals(rating));
}

// *******************************************************************************
// Surcharge de l'opérateur d'affectation =
// Tous les champs du Media sont recopiés.
// *******************************************************************************
Media& Media::operator=(const Media& media_source)
{
  this->Field1 = media_source.Field1;
  this->Field2 = media_source.Field2;
  this->Field3 = media_source.Field3;
  this->Field4 = media_source.Field4;
  this->Valid  = media_source.Valid;
  this->NextMediaPosition = media_source.NextMediaPosition;
}
