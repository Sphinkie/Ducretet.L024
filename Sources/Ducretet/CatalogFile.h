/* *******************************************************************************
 *  Gestion du fichier Catalog.ndx : Primitives d'accès Lecture/Ecriture
 ********************************************************************************/
#ifndef CATALOGFILE_H_INCLUDED
#define CATALOGFILE_H_INCLUDED

#include <SD.h>

#define MAX_LG_LINE   40          // longueur maximale prévisionelle des lignes du catalogue

class CatalogFile
{
   protected:
        void   begin();
        bool   openCatalogAtPosition(unsigned long pos=5);
        bool   openCatalogAtRandomPosition();
        void   closeCatalog();
        unsigned long getCurrentPosition();
        unsigned long getRandomPosition();

        // Lecture/Ecriture de lignes
        String readRandomLine();
        String readNextLine();
              
        // Lecture/Ecriture du Rating
        int    updateRating(unsigned long clipPosition, int increment);

      private:
        int    readRating(unsigned long clipPosition);
        void   writeRating(unsigned long clipPosition, int rating);
        // Private Member Variables
        File          FichierIndex;
        unsigned int  CatalogSize=10;

};

#endif // CATALOGFILE_H_INCLUDED
