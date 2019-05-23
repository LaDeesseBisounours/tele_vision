#include <stdio.h>
#include <gtk/gtk.h>
#include "misc.h"
#include <time.h>
#include <string.h>
#include <omp.h>

#define NB_CENTER 6
#define CLOUD_CENTER_INDEX 0

unsigned long
square(unsigned long n) {
  return n * n;
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
  unsigned long nbTotalPixels = (unsigned long)NbLine * NbCol;
  char *classSelection = calloc(nbTotalPixels, sizeof(char));
  unsigned char** pixels = calloc(nbTotalPixels, sizeof(char *));

  if (classSelection == NULL || pixels == NULL)
    return;
#pragma omp for
  for (int k = 0; k < nbTotalPixels; ++k) {
    pixels[k] = getNeighborList((void*)pucImaRes, k, NbLine, NbCol);
  }

  struct gravityCenter centers[NB_CENTER]; //step 2
  for (unsigned k = 0; k < 5; ++k)
    centers[CLOUD_CENTER_INDEX].curr[k] = 250;

  unsigned maxGround = 200;
  unsigned diff = maxGround / (NB_CENTER - 1);
#pragma omp for
  for (unsigned k = 0; k < NB_CENTER; ++k) {
    if (k == CLOUD_CENTER_INDEX)
      continue;
    for (unsigned j = 0; j < 5; ++j) 
      centers[k].curr[j] = maxGround;
    maxGround -= diff;
  }

#ifdef DEBUG_MODE
  printCenters(centers);
  unsigned long nbLoop = 0;
#endif
  char centersChanged = 1;
  while (centersChanged) {
#ifdef DEBUG_MODE
    nbLoop++;
    printf("loop nb %zu\n", nbLoop);
#endif
#pragma omp for
    for (unsigned k = 0; k < NB_CENTER; ++k) {
      centers[k].nb = 0;
      memset(&centers[k].tmp, 0, 5 * sizeof(unsigned long));
    }
#pragma omp for
    for(unsigned long index = 0; index < nbTotalPixels; ++index) { //step 3
      unsigned classIndex = 0;
      unsigned long old_diff = 0;
#pragma omp for
      for (unsigned k = 0; k < NB_CENTER; ++k) {
        unsigned long diff = 0;
        for (int i = 0; i < 5; i++) {
          diff +=
            square((unsigned long)pixels[index][i] - centers[k].curr[i]);
        }
        if (diff < old_diff || k == 0){
          classIndex = k;
          old_diff = diff;
        }
      }
      classSelection[index] = classIndex;
      #pragma omp critical
      {
        centers[classIndex].nb++;
        for (unsigned k = 0; k < 5; ++k)
          centers[classIndex].tmp[k] += pixels[index][k];
      }
    }
#pragma omp for
    for (unsigned k = 0; k < NB_CENTER; ++k) {
      for (unsigned j = 0; j < 5; ++j) {
        if ( centers[k].nb != 0)
          centers[k].tmp[j] /= centers[k].nb;
        else {
          #ifdef DEBUG_MODE
          printf("center %u empty\n", k);
          #endif
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

#pragma omp for
    for (int k = 0; k < NB_CENTER; k++) {
      for (int j = 0; j < 5; j++) {
        centers[k].curr[j] = (char)centers[k].tmp[j];
      }
    }
#ifdef DEBUG_MODE
    printCenters(centers);
#endif
  }
#ifdef DEBUG_MODE
  printf("number ground1 %zu on %zu\n", centers[1].nb, nbTotalPixels);
  printf("number cloud %zu on %zu\n", centers[CLOUD_CENTER_INDEX].nb, nbTotalPixels);
#endif
  float f = (float)centers[CLOUD_CENTER_INDEX].nb / (float)nbTotalPixels;
  printf("le pourcentage de nuages est : %f%%\n", f * 100);
#ifdef DEBUG_MODE
  float f2 = (float)(centers[1].nb + centers[CLOUD_CENTER_INDEX].nb) / (float)nbTotalPixels;
  printf("le pourcentage de nuages est : %f%%\n", f2 * 100);
#endif

  if (visualMode) {
#pragma omp for
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
#pragma omp for
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
  #pragma omp for
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

