/* *******************************************************************************
 *  Gestion du fichier catalogue des MP3 : Primitives d'accès Lecture/Ecriture
 ********************************************************************************/
#include "CatalogFile.h"


// *******************************************************************************
// On initialise le Seed du générateur aléatoire.
// N'a besoin d'être fait qu'une fois, mais pas immédiatement car SD doit être prêt.
// *******************************************************************************
void CatalogFile::begin()
{
  
   // On initialise le Seed du générateur aléatoire avec une valeur chaque fois différente.
   randomSeed(analogRead(0));

   File Cat = SD.open("/Catalog.ndx",FILE_READ);
   if (!Cat) 
   {
      this->CatalogSize = 0;
      Serial.println(F("CatalogFile:begin could not open Catalog.ndx"));
   }
   else
   {
      // On mémorise la taille du fichier Catalog.
      this->CatalogSize = Cat.size();
      Cat.close();
      Serial.print(F(" taille Catalog.ndx: ")); Serial.println(this->CatalogSize);
   }
}


// *******************************************************************************
// Ouvre le fichier Catalog.ndx, en gérant les cas d'erreur.
// Au debut du fichier index, il y a quelques octets inutiles. En dessous de 5, le programme plante.
// Retourne TRUE si le fichier a bien pu être ouvert.
// *******************************************************************************
bool CatalogFile::openCatalogAtPosition(unsigned long pos=5)
{
  if (pos<5) pos=5;
  Serial.print (F("open Catalog.ndx at "));Serial.println(pos);
  this->FichierIndex = SD.open("/Catalog.ndx");      // on ouvre le fichier Catalog
  if (!this->FichierIndex)
  {
    // en cas d'erreur d'ouverture du fichier, on renvoie false
    Serial.println(F("openCatalogAtPosition could not open Catalog.ndx"));
    return (false);
  }
  // Vérification
  if (pos > this->CatalogSize) 
  {
    Serial.print(pos); Serial.println(F(" exceeds Catalog file size")); 
    pos=5;
  }
  // On se positionne dans l'index à la position demandée
  bool FileAccessOK = this->FichierIndex.seek(pos);
  if (!FileAccessOK)
  {
    Serial.print(F("openCatalogAtPosition: Cannot go to position ")); Serial.println(pos);
    this->FichierIndex.close();
    return (false);
  }
  return true;
}

// *******************************************************************************
// Ouvre le fichier Catalog.ndx, a une position aléatoire "viable".
// *******************************************************************************
bool CatalogFile::openCatalogAtRandomPosition()
{
  bool  FileAccessOK;
  unsigned long  pos;
  
  Serial.println(F("  Catalog.ndx: going to random position"));
  // On se positionne entre [0 .. TailleCatalog]
  // on prend une marge de plusieurs chars pour ne pas être trop près de la fin du catalogue
  pos = random(10, this->CatalogSize-100);
  FileAccessOK = this->openCatalogAtPosition(pos);
  // Comme on est à une position aleatoire, on est possiblement au milieu d'une ligne.
  // On lit la fin de cette ligne, de façon à se positionner en début de ligne suivante.
  if (this->FichierIndex.available()) this->FichierIndex.readStringUntil('\n');
  return FileAccessOK;
}


// *******************************************************************************
// Ferme le fichier Catalog.ndx.
// *******************************************************************************
void CatalogFile::closeCatalog()
{
  FichierIndex.close();
  Serial.println (F("close Catalog.ndx"));
}

// *******************************************************************************
// Renvoie un index viable d'un media aléatoire dans le catalogue.
// *******************************************************************************
unsigned long CatalogFile::getRandomPosition()
{
    // On ouvre le catalogue sur un Clip aléatoire
    this->openCatalogAtRandomPosition();
    unsigned long pos = this->FichierIndex.position();
    this->closeCatalog();
    return pos;
}

// *******************************************************************************
// On lit la ligne suivante du fichier
// Le FichierIndex est ouvert préalablement.
// *******************************************************************************
String CatalogFile::readNextLine()
{
  /* char   Line[MAX_LG_LINE];   // ligne lue dans le fichier */
  /* Lg = FichierIndex.fgets(Line,MAX_LG_LINE);   //  SDFAT */
  /* Medialine=String(Line); */

  if (!this->FichierIndex.available()) return "EOF";
  String Medialine = this->FichierIndex.readStringUntil('\n');
  int Lg = Medialine.length();
  
  if (Lg==-1) {Serial.println (F("Cannot read Catalog.ndx")); return ("ERROR");}     // Erreur de lecture
  if (Lg==0 ) {Serial.println (F("End of Catalog"));          return ("EOF");}       // la ligne lue est vide ou EOF
  if (Lg <10) {Serial.print(F("line too short [")); Serial.print(Lg); Serial.print(F(" chars]: ")); Serial.print(Medialine); return ("ERROR");}     // ligne trop courte

  Medialine.trim();
  return Medialine;
}

// *******************************************************************************
// Renvoie la position dans le fichier (c'est le début de la ligne suivante)
// *******************************************************************************
unsigned long CatalogFile::getCurrentPosition()
{
  /* return FichierIndex.curPosition(); // SDFAT  */
  return FichierIndex.position();
}

// *******************************************************************************
// Lit une ligne au hasard dans le fichier.
// Le FichierIndex est ouvert préalablement.
// Renvoie la ligne, ou "EOF" ou "ERROR"
// *******************************************************************************
String CatalogFile::readRandomLine()
{
   unsigned int RandomNb;           // Numéro aléatoire entre 1 et taille
   RandomNb   = random(this->CatalogSize)+1;       // random(Max) renvoie un nombre aléatoire entre 0 et Max-1
   Serial.print("  read random line, number "); Serial.print (RandomNb); Serial.print (" out of "); Serial.println (CatalogSize); 
   
   // On se positionne sur un octet au hasard dans l'index
   if (!this->FichierIndex.seek(RandomNb)) 
   {
      Serial.println (F("readRandomLine: Cannot go to position ")); Serial.println(RandomNb);
      return ("ERROR");
   }
  
   // On lit la fin de la ligne en cours (généralement tronquée, puisqu'on s'est placé au hasard dans le fichier)
   if (this->FichierIndex.available()) this->FichierIndex.readStringUntil('\n');
  
   // On lit et on retourne la ligne suivante
   return (this->readNextLine());
}


// *******************************************************************************
// Modifie (max=5) le Rating du clip demandé, dans le fichier Catalog.ndx
// Le paramètre 'increment' accepte usuellement -1 ou +1
// *******************************************************************************
int CatalogFile::updateRating(unsigned long RatingPosition, int increment)
{
  int  Rating;
  
  Serial.print (F("Adding {increment} Star at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;

  // On lit le nombre d'étoiles actuel
  Rating = this->readRating(RatingPosition);
  Serial.print (F(" value ")); Serial.print(Rating);
  // On incrémente
  Rating+=increment;
  if (Rating > 5) Rating=5; 
  if (Rating < 0) Rating=0;
  Serial.print (F(" is replaced with ")); Serial.println(Rating);
  // On ecrit le nouveau nombre d'étoiles
  this->writeRating(RatingPosition, increment);

  return Rating;
}


// *******************************************************************************
// Lit le Rating du clip demandé, dans le fichier Catalog.ndx
// *******************************************************************************
int CatalogFile::readRating(unsigned long RatingPosition)
{
  File Fichier;
  char Stars='A';
  
  Serial.print (F("reading Rating at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;
  
  // On ouvre le fichier (renvoie NULL si error)
  Fichier = SD.open("/Catalog.ndx", FILE_READ);
  if (!Fichier)
  {
    // En cas d'erreur d'ouverture du fichier, on sort.
    Serial.println (F("Cannot open Catalog.ndx"));
    return 0;
  } 
  
  // On se positionne sur la ligne du clip
  Fichier.seek(RatingPosition);
  // On lit 1 octet
  Stars = Fichier.read();

 #ifdef DEBUG
  // DEBUG: on affiche la fin de la ligne lue
     Fichier.seek(RatingPosition-12);
     Serial.print (F(" End of line: "));
     for (int i=0; i<14; i++) Serial.print(Fichier.read());
     Serial.println(); 
#endif
  
  // On ferme le fichier
  Fichier.close();
  return (Stars-'0');  // renvoie une valeur numérique
}

// *******************************************************************************
// Ecrit le Rating du clip demandé, dans le fichier Catalog.ndx
// (on convertit les ints 0..5 en chars '0'..'5' 
// *******************************************************************************
void CatalogFile::writeRating(unsigned long RatingPosition, int rating)
{
  File Fichier;
  
  Serial.print (F("writing Rating at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;
  
  // On ouvre le fichier (renvoie NULL si error)
  Fichier = SD.open("/Catalog.ndx", FILE_WRITE);
  if (!Fichier)
  {
    // En cas d'erreur d'ouverture du fichier, on sort.
    Serial.println (F("Cannot open Catalog.ndx"));
    return;
  }
  
  // On se positionne sur la ligne du clip
  Fichier.seek(RatingPosition);
  // On ecrit 1 octet
  char stars = char(rating +'0');
  Fichier.print(stars);

#ifdef DEBUG
  // DEBUG: on affiche la fin de la ligne lue
     Fichier.seek(RatingPosition-12);
     Serial.print (F(" End of line after write: "));
     for (int i=0; i<14; i++) Serial.print(Fichier.read());
     Serial.println(); 
#endif
  // On ferme le fichier
  Fichier.close();
}