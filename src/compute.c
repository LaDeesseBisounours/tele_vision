#include <stdio.h>
#include <gtk/gtk.h>
#include "misc.h"
#include <string.h>


unsigned long
square(unsigned long n) {
  return n * n;
}

void analyse(guchar *pucImaRes, int NbLine, int NbCol) {
#define CLOUD_CLASS 1
#define GROUND_CLASS 2
  unsigned long nbTotalPixels = (unsigned long)NbLine * NbCol;
  char *classSelection = calloc(nbTotalPixels, sizeof(char));
  unsigned char** pixels = calloc(nbTotalPixels, sizeof(char *));

  if (classSelection == NULL || pixels == NULL)
    return;

  for (int k = 0; k < nbTotalPixels; ++k) {
    pixels[k] = getNeighborList((void*)pucImaRes, k, NbLine, NbCol);
  }
  unsigned char cloudCenter[5] = {250, 250, 250, 250, 250}; //step 2
  unsigned char groundCenter[5] = {240, 240, 240, 240, 240};
  unsigned long nbClouds = 0;

  char isChanged = 1;
  while (isChanged) {
    nbClouds = 0;
    for(unsigned long index = 0; index < nbTotalPixels; ++index) { //step 3
      unsigned long diffCloud = 0;
      unsigned long diffGround = 0;
      for (int i = 0; i < 5; i++) {
        diffCloud +=  square((unsigned long)pixels[index][i] - cloudCenter[i]);
        diffGround += square((unsigned long)pixels[index][i] - groundCenter[i]);
      }

      if (diffCloud < diffGround) {
        nbClouds++;
        classSelection[index] = CLOUD_CLASS;
      } else
        classSelection[index] = GROUND_CLASS;
    }
    unsigned long tmpCloudCenter[5] = {0,0,0,0,0};   // step 4
    unsigned long tmpGroundCenter[5] = {0,0,0,0,0};   // step 4
    for (unsigned long i = 0; i < nbTotalPixels; i++) {
      unsigned long *tmp;
      if (classSelection[i] == CLOUD_CLASS)
        tmp = tmpCloudCenter;
      else
        tmp = tmpGroundCenter;

      for (unsigned k = 0; k < 5; ++k) 
        tmp[k] += pixels[i][k];
    }

    if (nbClouds == 0 || nbClouds == nbTotalPixels)
      return;

    for (unsigned k = 0; k < 5; ++k) {
      tmpCloudCenter[k] /= nbClouds;
      tmpGroundCenter[k] /= nbTotalPixels - nbClouds;
    }

    unsigned long med = getMediane(tmpCloudCenter);
    for (unsigned k = 0; k < 5; ++k)
      tmpCloudCenter[k] = med;

    isChanged = 0;
    for (unsigned k = 0; k < 5 && !isChanged; ++k) {
      if (tmpCloudCenter[k] != cloudCenter[k]
          || tmpGroundCenter[k] != groundCenter[k]) {
        isChanged++;
        memcpy(groundCenter, tmpGroundCenter, sizeof(groundCenter));
        memcpy(cloudCenter, tmpCloudCenter, sizeof(groundCenter));
      }
    }
  }
  printf("j aime les bananes\n");
  float f = nbClouds / nbTotalPixels;
  printf("le pourcentage de nuages est : %f\n", f);
}

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/

/*---------------------------------------
  Proto: 

  
  But: 

  Entrees: 
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes ? la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes ? la suite des autres)


  Sortie:

  Rem: 

  Voir aussi:

  ---------------------------------------*/
void ComputeImage(guchar *pucImaOrig, 
		  int NbLine,
		  int NbCol, 
		  guchar *pucImaRes)
{
  int iNbPixelsTotal, iNumPix;
  int iNumChannel, iNbChannels=3; /* on travaille sur des images couleurs*/
  guchar ucMeanPix;

  printf("Segmentation de l'image.... A vous!\n");
  
  iNbPixelsTotal=NbCol*NbLine;
  for(iNumPix=0;
      iNumPix<iNbPixelsTotal*iNbChannels;
      iNumPix=iNumPix+iNbChannels){
    /*moyenne sur les composantes RVB */
    ucMeanPix=(unsigned char)
	((
	  *(pucImaOrig+iNumPix) +
	  *(pucImaOrig+iNumPix+1) +
	  *(pucImaOrig+iNumPix+2))/3);
    /* sauvegarde du resultat */
    for(iNumChannel=0;
	iNumChannel<iNbChannels;
	iNumChannel++)
      *(pucImaRes+iNumPix+iNumChannel)= ucMeanPix;
  }
  analyse(pucImaRes, NbLine, NbCol);
}

