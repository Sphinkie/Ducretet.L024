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

   // On cherche le nombre maximum, correspondant à la taille du fichier index.
   FichierIndex = SD.open("/Catalog.ndx");
   if (!FichierIndex) 
   {
      RandomMax = 0;
      Serial.println(F("Catalog: Cannot open Catalog.ndx"));
   }
   else
   {
      RandomMax = FichierIndex.size();
      FichierIndex.close();
      Serial.print(" taille Catalog.ndx: "); Serial.println(RandomMax);
   }
}


// *******************************************************************************
// Ouvre le fichier Catalog.ndx, en gérant les cas d'erreur.
// Au debut du fichier index, il y a quelques octets inutiles. En dessous de 5, le programme plante.
// *******************************************************************************
bool CatalogFile::openCatalogAtPosition(long pos=5)
{
  bool FileAccessOK;
  if (pos<5) pos=5;
  
  FichierIndex = SD.open("/Catalog.ndx");      // on ouvre le fichier Catalog
  if (!FichierIndex)
  {
    // en cas d'erreur d'ouverture du fichier, on renvoie false
    Serial.println (F("Cannot open Catalog.ndx"));
    return (false);
  }
  
  // On se positionne dans l'index à la position demandée
  // Serial.print(F("  Catalog.ndx: going to position ")); Serial.println(pos);
  FileAccessOK = FichierIndex.seek(pos);
  if (!FileAccessOK)
  {
    Serial.print(F("Catalog.ndx: Cannot go to position ")); Serial.println(pos);
    FichierIndex.close();
    return (false);
  }
  return true;
}

// *******************************************************************************
// Ouvre le fichier Catalog.ndx, a une position aléatoire
// *******************************************************************************
bool CatalogFile::openCatalogAtRandomPosition()
{
  char  Line[MAX_LG_LINE];   // ligne lue dans le fichier
  bool  FileAccessOK;
  long  pos;
  
  Serial.println(F("  Catalog.ndx: going to random position"));
  // On se positionne entre [0 .. TailleCatalog]  (TailleCatalog = randomMax)
  // on prend une marge de 100 chars pour ne pas être trop près de la fin du catalogue
  pos = random(100, RandomMax-100);
  FileAccessOK = openCatalogAtPosition(pos);
  // Comme on est à une position aleatoire, on est possiblement au milieu d'une ligne.
  // On lit la fin de cette ligne, de façon à se positionner en début de ligne suivante.
  /* if (FileAccessOK) FichierIndex.read(Line,MAX_LG_LINE);   */
  FichierIndex.readStringUntil('\n');
  return FileAccessOK;
}


// *******************************************************************************
// Ferme le fichier Catalog.ndx.
// *******************************************************************************
void CatalogFile::closeCatalog()
{
  FichierIndex.close();
}

// *******************************************************************************
// Renvoie un index viable d'un media aléatoire dans le catalogue
// *******************************************************************************
long CatalogFile::getRandomPosition()
{
    // On ouvre le catalogue au hasard, de façon à trouver une position initiale viable.
    this->openCatalogAtRandomPosition();
    long pos = FichierIndex.position();
    FichierIndex.close();
    return pos;
}

// *******************************************************************************
// On lit la ligne suivante du fichier
// Le FichierIndex est ouvert préalablement.
// *******************************************************************************
String CatalogFile::readNextLine()
{
  /* char   Line[MAX_LG_LINE];   // ligne lue dans le fichier */
  String Medialine;
  int Lg = 0;
   
  // On lit la ligne suivante
  Medialine = FichierIndex.readStringUntil('\n');
  /* Lg = FichierIndex.fgets(Line,MAX_LG_LINE);   NO_SDFAT */
  /* Medialine=String(Line); */
  Lg = Medialine.length();
  Medialine.trim();
  
  if (Lg==-1) {Serial.println (F("Cannot read Catalog.ndx")); return ("ERROR");}     // Erreur de lecture
  if (Lg==0 ) {Serial.println (F("End of Catalog"));          return ("EOF");}       // la ligne lue est vide ou EOF
  if (Lg <10) {Serial.print(F("line too short [")); Serial.print(Lg); Serial.print(F(" chars]: ")); Serial.print(Medialine); return ("ERROR");}     // ligne trop courte

  return Medialine;
}

// *******************************************************************************
// Renvoie la position dans le fichier (c'est le début de la ligne suivante)
// *******************************************************************************
long CatalogFile::getCurrentPosition()
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
   char         line[MAX_LG_LINE];  // ligne lue dans le fichier
   unsigned int RandomNb;           // Numéro aléatoire entre 1 et taille
   int          Lg=0;
   String       Medialine;

  RandomNb   = random(RandomMax)+1;   // random(Max) renvoie un nombre aléatoire entre 0 et Max-1
  Serial.print("  read random line, number "); Serial.print (RandomNb); Serial.print (" out of "); Serial.println (RandomMax); 
   
  // On se positionne sur un octet au hasard dans l'index
  if (!FichierIndex.seek(RandomNb)) 
  {
    Serial.println (F("Ratings.txt: Cannot go to position ")); Serial.println(RandomNb);
    return ("ERROR");
  }
  
  // On lit la fin de la ligne en cours (généralement tronquée, puisqu'on s'est placé au hasard dans le fichier)
  FichierIndex.readStringUntil('\n');
  
  // On lit la ligne suivante
  Medialine = FichierIndex.readStringUntil('\n');
  Lg = Medialine.length();
  if (Lg==-1) return ("ERROR");    // Erreur de lecture
  if (Lg==0)  return ("EOF");      // la ligne lue est vide ou EOF

  Medialine.trim();
  return Medialine;
}


// *******************************************************************************
// Modifie (max=5) le Rating du clip demandé, dans le fichier Catalog.ndx
// Le paramètre 'increment' accepte usuellement -1 ou +1
// *******************************************************************************
int CatalogFile::updateRating(long RatingPosition,int increment)
{
  int  Rating;
  
  Serial.print (F("Adding {increment} Star at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;

  // On lit le nombre d'étoiles actuel
  Rating=readRating(RatingPosition);
  Serial.print (F(" value ")); Serial.print(Rating);
  // On incrémente
  Rating+=increment;
  if (Rating > 5) Rating=5; 
  if (Rating < 0) Rating=0;
  Serial.print (F(" is replaced with ")); Serial.println(Rating);
  // On ecrit le nouveau nombre d'étoiles
  writeRating(increment, RatingPosition);

  return Rating;
}


// *******************************************************************************
// Lit le Rating du clip demandé, dans le fichier Catalog.ndx
// *******************************************************************************
int CatalogFile::readRating(long RatingPosition)
{
  File FichierIndex;
  char   Stars='A';
  
  Serial.print (F("reading Rating at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;
  
  // On ouvre le fichier (renvoie false si error)
  FichierIndex = SD.open("/Catalog.ndx", FILE_READ);
  if (!FichierIndex)
  {
    // En cas d'erreur d'ouverture du fichier, on sort.
    Serial.println (F("Cannot open Catalog.ndx"));
    return 0;
  } 
  
  // On se positionne sur la ligne du clip
  FichierIndex.seek(RatingPosition);
  // On lit 1 octet
  Stars = FichierIndex.read();

  // DEBUG: on affiche la fin de la ligne lue
     FichierIndex.seek(RatingPosition-12);
     Serial.print (F(" End of line: "));
     for (int i=0; i<14; i++) Serial.print(FichierIndex.read());
     Serial.println(); 
  
  // On ferme le fichier
  FichierIndex.close();
  return (Stars-'0');  // renvoie une valeur numérique
}

// *******************************************************************************
// Ecrit le Rating du clip demandé, dans le fichier Catalog.ndx
// (on convertit les ints 0..5 en chars '0'..'5' 
// *******************************************************************************
void CatalogFile::writeRating(int rating, long RatingPosition)
{
  File FichierIndex;
  
  Serial.print (F("writing Rating at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;
  
  // On ouvre le fichier (renvoie false si error)
  FichierIndex = SD.open("/Catalog.ndx", FILE_WRITE);
  if (!FichierIndex)
  {
    // En cas d'erreur d'ouverture du fichier, on sort.
    Serial.println (F("Cannot open Catalog.ndx"));
    return;
  }
  
  // On se positionne sur la ligne du clip
  FichierIndex.seek(RatingPosition);
  // On ecrit 1 octet
  char stars = char(rating +'0');
  FichierIndex.print(stars);

  // DEBUG: on affiche la fin de la ligne lue
     FichierIndex.seek(RatingPosition-12);
     Serial.print (F(" End of line after write: "));
     for (int i=0; i<14; i++) Serial.print(FichierIndex.read());
     Serial.println(); 
  
  // On ferme le fichier
  FichierIndex.close();
}
