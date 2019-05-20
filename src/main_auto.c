#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <math.h>
#include "compute_auto.h"
#include "misc.h"

int main(void) {
  int NbCol, NbLine;
  guchar *pucImaOrig, *pucImaRes;
  GdkPixbuf *pGdkPixbufIma;

  gchar *pcFileName;// = "../../IMAGES/20342579204091110141x.bmp";
  while ((pcFileName = listPictures()) != NULL) {
    printf("Analysing %s\n", pcFileName);
    /* chargement en memoire de l'image */
    pGdkPixbufIma = gdk_pixbuf_new_from_file (pcFileName, NULL);

    /* echec du chargement de l'image en memoire */
    if (pGdkPixbufIma == NULL){
      printf("Impossible de charger le fichier %s\n", pcFileName);
      exit(0);
    }

    NbCol = gdk_pixbuf_get_width(pGdkPixbufIma); 
    NbLine = gdk_pixbuf_get_height(pGdkPixbufIma);
    pucImaOrig = gdk_pixbuf_get_pixels(pGdkPixbufIma);
    /* recuperation du tableau des pixels de l'image resultat */
    pucImaRes = gdk_pixbuf_get_pixels(pGdkPixbufIma);
    ComputeImage(pucImaOrig, NbLine, NbCol, pucImaRes);
    free(pcFileName);
    GdkPixbufDestroyNotify(pucImaOrig);
    GdkPixbufDestroyNotify(pucImaRes);
  }
}
