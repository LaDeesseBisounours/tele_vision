#include <stdio.h>
#include <gtk/gtk.h>
#include "misc.h"
#include <time.h>
#include <string.h>

#define NB_CENTER 6
#define CLOUD_CENTER_INDEX 0

unsigned long
square(unsigned long n) {
  return n * n;
}
#ifdef DEBUG_MODE
#endif

unsigned minDiff(struct gravityCenter centers[]){
  unsigned min = 0;
  for (unsigned k = 1; k < NB_CENTER; ++k) {
    if (centers[min].diff > centers[k].diff)
      min = k;
  }
  return min;
}

void printCenters(struct gravityCenter centers[]) {
  for (unsigned k = 0; k < NB_CENTER; k++) {
    printf("center %u : { ", k);
    for (int j = 0; j < 5; j++) {
      printf("%zu, ", centers[k].curr[j]);
    }
    printf("}\n");
  }
}

void analyse(guchar *pucImaRes, int NbLine, int NbCol) {
  clock_t t1 = clock();
  printf("starting analyse\n");
  unsigned long nbTotalPixels = (unsigned long)NbLine * NbCol;
  char *classSelection = calloc(nbTotalPixels, sizeof(char));
  unsigned char** pixels = calloc(nbTotalPixels, sizeof(char *));

  if (classSelection == NULL || pixels == NULL)
    return;

  for (int k = 0; k < nbTotalPixels; ++k) {
    pixels[k] = getNeighborList((void*)pucImaRes, k, NbLine, NbCol);
  }

  struct gravityCenter centers[NB_CENTER]; //step 2
  for (unsigned k = 0; k < 5; ++k)
    centers[CLOUD_CENTER_INDEX].curr[k] = 250;

  unsigned maxGround = 200;
  unsigned diff = maxGround / (NB_CENTER - 1);

  for (unsigned k = 0; k < NB_CENTER; ++k) {
    if (k == CLOUD_CENTER_INDEX)
      continue;
    for (unsigned j = 0; j < 5; ++j) 
      centers[k].curr[j] = maxGround;
    maxGround -= diff;
  }

#ifdef DEBUG_MODE
  printCenters(centers);
#endif

  char centersChanged = 1;
#ifdef DEBUG_MODE
  unsigned long nbLoop = 0;
#endif
  while (centersChanged) {
#ifdef DEBUG_MODE
    nbLoop++;
    printf("loop nb %zu\n", nbLoop);
#endif
    for (unsigned k = 0; k < NB_CENTER; ++k) {
      centers[k].nb = 0;
      memset(&centers[k].tmp, 0, 5 * sizeof(unsigned long));
    }
    for(unsigned long index = 0; index < nbTotalPixels; ++index) { //step 3
      for (unsigned k = 0; k < NB_CENTER; ++k) {
        centers[k].diff = 0;
        for (int i = 0; i < 5; i++) {
          centers[k].diff +=
            square((unsigned long)pixels[index][i] - centers[k].curr[i]);
        }
      }
      unsigned classIndex = minDiff(centers);
      centers[classIndex].nb++;
      classSelection[index] = classIndex;
      //compressing more code in one loop
      for (unsigned k = 0; k < 5; ++k)
        centers[classIndex].tmp[k] += pixels[index][k];
    }

    for (unsigned k = 0; k < NB_CENTER; ++k) {
      for (unsigned j = 0; j < 5; ++j) {
        if ( centers[k].nb != 0)
          centers[k].tmp[j] /= centers[k].nb;
        else {
          printf("center %u empty", k);
          centers[k].tmp[j] = centers[k].curr[j];
        }
      }
    }

    unsigned long med = getMediane(centers[CLOUD_CENTER_INDEX].tmp);
    if (centers[CLOUD_CENTER_INDEX].curr[0] - med > 50)
      med = centers[CLOUD_CENTER_INDEX].curr[0];
    for (unsigned k = 0; k < 5; ++k)
      centers[CLOUD_CENTER_INDEX].tmp[k] = med;



    centersChanged = 0;

    for (int k = 0; !centersChanged && k < NB_CENTER; k++) {
      for (int j = 0; j < 5; j++) {
        if (centers[k].curr[j] != centers[k].tmp[j]) {
          centersChanged++;
          break;
        }
      }
    }


    for (int k = 0; k < NB_CENTER; k++) {
      for (int j = 0; j < 5; j++) {
        centers[k].curr[j] = (char)centers[k].tmp[j];
      }
    }
#ifdef DEBUG_MODE
    printCenters(centers);
#endif
  }
  printf("number ground1 %zu on %zu\n", centers[1].nb, nbTotalPixels);
  printf("number cloud %zu on %zu\n", centers[CLOUD_CENTER_INDEX].nb, nbTotalPixels);
  float f = (float)centers[CLOUD_CENTER_INDEX].nb / (float)nbTotalPixels;
  float f2 = (float)(centers[1].nb + centers[CLOUD_CENTER_INDEX].nb) / (float)nbTotalPixels;
  printf("le pourcentage de nuages est : %f%%\n", f * 100);
  printf("le pourcentage de nuages est : %f%%\n", f2 * 100);

  if (visualMode) {
    for (unsigned i = 0; i < nbTotalPixels; i++) {
      if (classSelection[i] == CLOUD_CENTER_INDEX) {
        pucImaRes[i * 3] = 255;
        pucImaRes[i * 3 + 1] = 0;
        pucImaRes[i * 3 + 2] = 0;
      }
      if (classSelection[i] == 1) {
        pucImaRes[i * 3] = 0;
        pucImaRes[i * 3 + 1] = 255;
        pucImaRes[i * 3 + 2] = 0;
      }
    }
  }
  for (unsigned long k= 0; k < nbTotalPixels; ++k)
    free(pixels[k]);
  free(pixels);
  free(classSelection);
  clock_t t2 = clock();
  double duration = 1000 * (t2 - t1);
  printf("duration : %.2f ms\n", duration / CLOCKS_PER_SEC);
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

