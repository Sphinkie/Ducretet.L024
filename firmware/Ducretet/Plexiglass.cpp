/* *******************************************************************************
 *  Face-Avant en Plexiglass
 *  
 *  Les valeurs à l'interieur des fonctions doivent être ajustées en fonction 
 *  du design de la Face Avant en plexiglass.
 ********************************************************************************/
#include "Arduino.h"
#include "Plexiglass.h"


// *******************************************************************************
// Constructeur
// *******************************************************************************
Plexiglass::Plexiglass()
{
}

// *******************************************************************************
// Memorise le Tuning et determines les valeurs associées
// *******************************************************************************
void Plexiglass::setTuning(int tuning)
{
  Serial.println(" setTuning "+String(tuning));
  this->Genre  = this->getGenreLabel(tuning);   // La valeur du genre demandé
  this->Year   = this->getYearValue(tuning);    // La valeur de l'année demandée
  this->Rating = this->getStarsValue(tuning);   // La valeur du rating demandé
  this->Beat   = this->getBeatValue(tuning);    // La valeur du BPM demandé
}

// *********************************************************************
// Renvoie le GENRE correspondant à la position du bouton TUNE
//   "Classique;Blues;Jazz;Folk;Rock n'Roll;Rock;Chanson;Musiques du monde;"
// On positionne aussi GenreLength qui correspond à la 'largeur' de ce genre
// *********************************************************************
String Plexiglass::getGenreLabel(int tuning)
{
  if (tuning < 360)  {this->GenreLength=360; return "Classique"; }
  if (tuning < 450)  {this->GenreLength=90;  return "Blues"; }
  if (tuning < 530)  {this->GenreLength=80;  return "Jazz"; }
  if (tuning < 630)  {this->GenreLength=100; return "Folk"; }
  if (tuning < 700)  {this->GenreLength=70;  return "Rock n'Roll"; }
  if (tuning < 750)  {this->GenreLength=50;  return "Rock"; }
  if (tuning < 840)  {this->GenreLength=90;  return "Chanson"; }
  if (tuning < 890)  {this->GenreLength=50;  return "Musiques du monde"; }
  return "*";    // Genre "Autres"
}


// *********************************************************************
// Renvoie le RATING correspondant à la position du bouton TUNE
// *********************************************************************
int Plexiglass::getStarsValue(int tuning)
{
  if (tuning < 300)  {return 0; }   /* 000..300  : 0 étoile  */
  if (tuning < 450)  {return 1; }   /* 300..450  : 1 étoile  */  
  if (tuning < 580)  {return 2; }   /* 450..580  : 2 étoiles */
  if (tuning < 680)  {return 3; }   /* 580..680  : 3 étoiles */
  if (tuning < 780)  {return 4; }   /* 680..780  : 4 étoiles */
  else  {return 5; }                /* 780..1023 : 5 étoiles */
}


// *********************************************************************
// Renvoie l'ANNEE correspondant à la position du bouton TUNE
// *********************************************************************
int Plexiglass::getYearValue(int tuning)
{
  /*
   map(value, fromLow, fromHigh, toLow, toHigh)
   Parameters:
     value   : the number to map.
     fromLow : the lower bound of the value’s current range.
     fromHigh: the upper bound of the value’s current range.
     toLow   : the lower bound of the value’s target range.
     toHigh  : the upper bound of the value’s target range.
  */
  if (tuning < 270)       {RangeStart=0;    RangeEnd=1700; return map(tuning,   0, 270,RangeStart,RangeEnd); }   // 0000...1700
  if (tuning < 360)       {RangeStart=1700; RangeEnd=1800; return map(tuning, 270, 360,RangeStart,RangeEnd); }   // 1700...1800
  if (tuning < 410)       {RangeStart=1800; RangeEnd=1900; return map(tuning, 360, 410,RangeStart,RangeEnd); }   // 1800...1900
  if (tuning < 460)       {RangeStart=1900; RangeEnd=1940; return map(tuning, 410, 460,RangeStart,RangeEnd); }   // 1900...1940
  if (tuning < 540)       {RangeStart=1940; RangeEnd=1950; return map(tuning, 460, 530,RangeStart,RangeEnd); }   // 40's
  if (tuning < 600)       {RangeStart=1950; RangeEnd=1960; return map(tuning, 530, 600,RangeStart,RangeEnd); }   // 50's
  if (tuning < 700)       {RangeStart=1960; RangeEnd=1970; return map(tuning, 600, 700,RangeStart,RangeEnd); }   // 60's
  if (tuning < 790)       {RangeStart=1970; RangeEnd=1980; return map(tuning, 700, 790,RangeStart,RangeEnd); }   // 70's
  if (tuning < 840)       {RangeStart=1980; RangeEnd=1990; return map(tuning, 790, 840,RangeStart,RangeEnd); }   // 80's
  if (tuning < 860)       {RangeStart=1990; RangeEnd=2000; return map(tuning, 840, 860,RangeStart,RangeEnd); }   // 1990...2000
  if (tuning < 1023)      {RangeStart=2000; RangeEnd=2050; return map(tuning, 860,1023,RangeStart,RangeEnd); }   // 2000...2050
}

// *********************************************************************
// Renvoie le Beat correspondant à la position du bouton TUNE
// *********************************************************************
int Plexiglass::getBeatValue(int tuning)
{
  /*
   map(value, fromLow, fromHigh, toLow, toHigh)
   Parameters:
     value   : the number to map.
     fromLow : the lower bound of the value’s current range.
     fromHigh: the upper bound of the value’s current range.
     toLow   : the lower bound of the value’s target range.
     toHigh  : the upper bound of the value’s target range.
  */
/*  
  if (tuning < 100 )      {RangeStart=0;    RangeEnd=20; return map(tuning, 860,1023,RangeStart,RangeEnd); }  
  if (tuning < 200)       {RangeStart=0;    RangeEnd=1700; return map(tuning,   0, 270,RangeStart,RangeEnd); }   
  if (tuning < 400)       {RangeStart=1700; RangeEnd=1800; return map(tuning, 270, 360,RangeStart,RangeEnd); }   
  if (tuning < 500)       {RangeStart=1800; RangeEnd=1900; return map(tuning, 360, 410,RangeStart,RangeEnd); }   
  if (tuning < 600)       {RangeStart=1960; RangeEnd=1970; return map(tuning, 600, 700,RangeStart,RangeEnd); }   
  if (tuning < 700)       {RangeStart=1970; RangeEnd=1980; return map(tuning, 700, 790,RangeStart,RangeEnd); }   
  if (tuning < 800)       {RangeStart=1980; RangeEnd=1990; return map(tuning, 790, 840,RangeStart,RangeEnd); }   
  if (tuning < 900)       {RangeStart=1980; RangeEnd=1990; return map(tuning, 790, 840,RangeStart,RangeEnd); }   
  if (tuning < 1023)       {RangeStart=1990; RangeEnd=2000; return map(tuning, 840, 860,RangeStart,RangeEnd); }   
*/
return 130;
}
/* 00.......40 Larghissimo  60................80 adagio          100 .................. 128 allegretto
 *      Gravemente                   Adagissimo
 *          40..............60 largo       76....................100 andante   112.................160 allegro      188 prestissimo
 *                Lentissimo(48)                          Andantino 
 *                     52........68 lento            88...................112 moderato       140 vivace 140..................200 presto
 *                            Adagietto                                                               Vivacissimo   
 */


// *******************************************************************************
// Renvoie l'année sous forme de String de 4 chars
// *******************************************************************************
String Plexiglass::getYear()
{
  char Annee[5]; //4 digits + the null char
  sprintf(Annee, "%04d", this->Year);
  return String(Annee);
}

        
// *******************************************************************************
// Debug: Affiche tous les genres en fonction de la valeur du "tuning"
// *******************************************************************************
void Plexiglass::debugGenre()
{
  // Les entrées Analog renvoient une valeur entre 0 et 1023
  for (int i=0; i<1023; i++)
  {
    Serial.print(i);Serial.print("=>");Serial.println(this->getGenreLabel(i));
  }
}

// *******************************************************************************
// Debug: Affiche toutes les années en fonction de la valeur du "tuning"
// *******************************************************************************
void Plexiglass::debugYear()
{
  // Les entrées Analog renvoient une valeur entre 0 et 1023
  for (int i=0; i<1023; i++)
  {
    Serial.print(i);Serial.print("=>");Serial.println(this->getYearValue(i));
  }
}
