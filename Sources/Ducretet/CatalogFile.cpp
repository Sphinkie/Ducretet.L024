/* *******************************************************************************
 *  Gestion du fichier catalogue des MP3 : Primitives d'accès Lecture/Ecriture
 ********************************************************************************/
#include <SdFat.h>
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
   if (!FichierIndex.open("/Catalog.ndx")) Serial.println(F("Catalog: Cannot open Catalog.ndx"));

   RandomMax = FichierIndex.fileSize();
   FichierIndex.close();
   Serial.print(" taille Catalog.ndx: "); Serial.println(RandomMax);
}


// *******************************************************************************
// Ouvre le fichier Catalog.ndx, en gérant les cas d'erreur.
// Au debut du fichier index, il y a quelques octets inutiles. En dessous de 5, le programme plante.
// *******************************************************************************
bool CatalogFile::openCatalogAtPosition(long pos=5)
{
  bool FileAccessOK;
  if (pos<5) pos=5;
  
  FileAccessOK = FichierIndex.open("/Catalog.ndx");      // on ouvre le fichier Catalog
  if (!FileAccessOK)
  {
    // en cas d'erreur d'ouverture du fichier, on renvoie "NOISE.MP3"
    Serial.println (F("Cannot open Catalog.ndx"));
    return (false);
  }
  
  // On se positionne dans l'index à la position demandée
  // Serial.print(F("  Catalog.ndx: going to position ")); Serial.println(pos);
  FileAccessOK = FichierIndex.seekSet(pos);
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
  if (FileAccessOK) FichierIndex.fgets(Line,MAX_LG_LINE);;
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
    long pos = this->getCurrentPosition();
    this->closeCatalog();
    return pos;
}

// *******************************************************************************
// On lit la ligne suivante du fichier
// Le FichierIndex est ouvert préalablement.
// *******************************************************************************
String CatalogFile::readNextLine()
{
  char   Line[MAX_LG_LINE];   // ligne lue dans le fichier
  String Medialine;
   
  // On lit la ligne suivante
  int Lg = FichierIndex.fgets(Line,MAX_LG_LINE);

  if (Lg==-1) {Serial.println (F("Cannot read Catalog.ndx")); return ("ERROR");}     // Erreur de lecture
  if (Lg==0 ) {Serial.println (F("End of Catalog"));          return ("EOF");}       // la ligne lue est vide ou EOF
  if (Lg <10) {Serial.print(F("line too short [")); Serial.print(Lg); Serial.print(F(" chars]: ")); Serial.print(Line); return ("ERROR");}     // ligne trop courte

  Medialine=String(Line);
  Medialine.trim();
  return Medialine;
}

// *******************************************************************************
// Renvoie la position dans le fichier (c'est le début de la ligne suivante)
// *******************************************************************************
long CatalogFile::getCurrentPosition()
{
  return FichierIndex.curPosition();
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
   String       medialine;

  RandomNb   = random(RandomMax)+1;   // random(Max) renvoie un nombre aléatoire entre 0 et Max-1
  Serial.print("  read random line, number "); Serial.print (RandomNb); Serial.print (" out of "); Serial.println (RandomMax); 
   
  // On se positionne sur un octet au hasard dans l'index
  if (!FichierIndex.seekSet(RandomNb))
  {
    Serial.println (F("Ratings.txt: Cannot go to position ")); Serial.println(RandomNb);
    return ("ERROR");
  }
  
  // On lit la fin de la ligne en cours (généralement tronquée, puisqu'on s'est placé au hasard dans le fichier)
  Lg = FichierIndex.fgets(line,MAX_LG_LINE);
  if (Lg==-1) return ("ERROR");      // Erreur de lecture
  if (Lg==0)  return ("EOF");      // la ligne lue est vide ou EOF
  // Serial.print (" positioned at : "); Serial.println (line);
  
  // On lit la ligne suivante
  Lg = FichierIndex.fgets(line,MAX_LG_LINE);
  if (Lg==-1) return ("ERROR");      // Erreur de lecture
  if (Lg==0)  return ("EOF");      // la ligne lue est vide ou EOF

  medialine=String(line);
  medialine.trim();
  return medialine;
}


// *******************************************************************************
// Ajoute +1 (max=5) au Rating du clip demandé, dans le fichier Catalog.ndx
// *******************************************************************************
int CatalogFile::writeAddStar(long RatingPosition)
{
  SdFile FichierIndex;
  char   Stars='A';
  
  Serial.print (F("ADDING a Star at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;
  
  // On ouvre le fichier (renvoie false si error)
  if (!FichierIndex.open("/Catalog.ndx", O_RDWR))      // renvoie False en cas d'erreur
  {
    // En cas d'erreur d'ouverture du fichier, on sort.
    Serial.println (F("Cannot open Catalog.ndx"));
    return;
  }
  
  // On se positionne sur la ligne du clip
  FichierIndex.seekSet(RatingPosition);
  
  // On lit le nombre d'etoiles (1 octet)
  Stars = FichierIndex.read();
  Serial.print (F(" I have read a byte:")); Serial.print(Stars);
  // On incrémente
  if (++Stars >'5') Stars='5'; 
  if (Stars   <'0') Stars='0';
  // On ecrit le nouveau nombre d'étoiles
  FichierIndex.seekSet(RatingPosition);
  FichierIndex.print(Stars);
  Serial.print (F(" and replaced with ")); Serial.println(Stars);

  // On vérifie (pour le debug)
  FichierIndex.seekSet(RatingPosition-12);
  Serial.print (F(" End of line after update: "));
  for (int i=0; i<14; i++) 
    {
      char Debug = FichierIndex.read(); 
      Serial.print(Debug);
    }
  Serial.println(); 
  
  // On ferme le fichier
  FichierIndex.close();
  
  return Stars;
}


// *******************************************************************************
// Soustrait 1 (min=0) au Rating du clip demandé, dans le fichier Catalog.ndx
// *******************************************************************************
int CatalogFile::writeRemoveStar(long RatingPosition)
{
  SdFile FichierIndex;
  char   Stars='A';
  
  Serial.print (F("REMOVING a Star at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;

  // On ouvre le fichier
  if (!FichierIndex.open("/Catalog.ndx", O_RDWR))      // renvoie false si error
  {
    // En cas d'erreur d'ouverture du fichier, on sort
    Serial.println (F("Cannot open Catalog.ndx"));
    return;
  }

  // On se positionne sur la ligne du clip
  FichierIndex.seekSet(RatingPosition);
  
  // On lit le nombre d'etoiles
  Stars = FichierIndex.read();
  Serial.print (F(" I have read byte ")); Serial.print(Stars);
  // On décrémente
  if (--Stars < '0') Stars='0';
  if (Stars   >'5') Stars='5';
  // On ecrit le nouveau nombre d'étoiles
  FichierIndex.seekSet(RatingPosition);
  FichierIndex.print(Stars);
  Serial.print (F(" and replaced with ")); Serial.println(Stars);
  
  // On vérifie (pour le debug)
  FichierIndex.seekSet(RatingPosition-12);
  Serial.print (F(" End of line after update: "));
  for (int i=0; i<14; i++) 
    {
      char Debug = FichierIndex.read(); 
      Serial.print(Debug);
    }
  Serial.println(); 
  
  // On ferme le fichier
  FichierIndex.close();
  return Stars;
}



// *******************************************************************************
// Lit le Rating du clip demandé, dans le fichier Catalog.ndx
// *******************************************************************************
int CatalogFile::readRating(long RatingPosition)
{
  SdFile FichierIndex;
  char   Stars='A';
  
  Serial.print (F("reading Rating at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;
  
  // On ouvre le fichier (renvoie false si error)
  if (!FichierIndex.open("/Catalog.ndx", O_RDWR))      // renvoie False en cas d'erreur
  {
    // En cas d'erreur d'ouverture du fichier, on sort.
    Serial.println (F("Cannot open Catalog.ndx"));
    return 0;
  }
  
  // On se positionne sur la ligne du clip
  FichierIndex.seekSet(RatingPosition);
  // On lit 1 octet
  Stars = FichierIndex.read();

  // On vérifie (pour le debug)
  FichierIndex.seekSet(RatingPosition-12);
  Serial.print (F(" End of line: "));
  for (int i=0; i<14; i++) 
    {
      char Debug = FichierIndex.read(); 
      Serial.print(Debug);
    }
  Serial.println(); 
  
  // On ferme le fichier
  FichierIndex.close();
  return (Stars-'0');  // renvoie un entier
}

// *******************************************************************************
// Ecrit le Rating du clip demandé, dans le fichier Catalog.ndx
// (on convertit les ints 0..5 en chars '0'..'5' 
// *******************************************************************************
void CatalogFile::writeRating(int rating, long RatingPosition)
{
  SdFile FichierIndex;
  
  Serial.print (F("writing Rating at ")); Serial.println (RatingPosition);
  if (RatingPosition==NULL) return;
  
  // On ouvre le fichier (renvoie false si error)
  if (!FichierIndex.open("/Catalog.ndx", O_RDWR))      // renvoie False en cas d'erreur
  {
    // En cas d'erreur d'ouverture du fichier, on sort.
    Serial.println (F("Cannot open Catalog.ndx"));
    return;
  }
  
  // On se positionne sur la ligne du clip
  FichierIndex.seekSet(RatingPosition);
  // On ecrit 1 octet
  char stars = char(rating +'0');
  FichierIndex.print(stars);

  // On vérifie (pour le debug)
  FichierIndex.seekSet(RatingPosition-12);
  Serial.print (F(" End of line after update: "));
  for (int i=0; i<14; i++) 
    {
      char Debug = FichierIndex.read(); 
      Serial.print(Debug);
    }
  Serial.println(); 
  
  // On ferme le fichier
  FichierIndex.close();
}
