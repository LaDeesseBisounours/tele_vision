#include <stdio.h>
#include <gtk/gtk.h>

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto: 

  
  But: 

  Entrees: 
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes à la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes à la suite des autres)


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
}

void analyse(guchar *pucImaRes, int NbLine, int NbCol) {
    guchar *cloudClass = calloc(NbLine * NbCol, sizeof(guchar));
    int cloudClassLength = 0;

    guchar *groundClass = calloc(NbLine * NbCol, sizeof(guchar));
    int groundClassLength = 0;

    unsigned char cloudCenter[5] = {250, 250, 250, 250, 250};
    unsigned char groundCenter[5] = {240, 240, 240, 240, 240};

    char isChanged = 1;
    while (isChanged) {
        for(int iNumPix = 0; iNumPix < iNbPixelsTotal * iNbChannels; iNumPix = iNumPix + iNbChannels) {
            unsigned char *pixel = getNeighborsList((void *)pucImaRes, iNumPix, NbLine, NbCol);
            unsigned diffCloud = 0;
            for (int i = 0; i < 5; i++)
                diffCloud += (pixel[i] - cloudCenter[i]) * (pixel[i] - cloudCenter[i]);
            unsigned diffGround = 0;
            for (int i = 0; i < 5; i++)
                diffGround += (pixel[i] - groundCenter[i]) * (pixel[i] - groundCenter[i]);
            if (diffCloud < diffGround)
              cloudClass[cloudClassLength++] = pixel;
            else
              groundClass[groundClassLength++] = pixel;
        }
        unsigned tmpcloudCenter[5] = {0,0,0,0,0};
        for (int i = 0; i < cloudClassLength; i++)
        {
            tmpcloudCenter[0] += cloudClass[i][0];
            tmpcloudCenter[1] += cloudClass[i][1];
            tmpcloudCenter[2] += cloudClass[i][2];
            tmpcloudCenter[3] += cloudClass[i][3];
            tmpcloudCenter[4] += cloudClass[i][4];
        }
        
    }
}

