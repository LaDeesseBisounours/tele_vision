#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include "compute.h"

int main(void) {
  GtkWidget **ppwTabArgs;
  GtkFileSelection *pfsOpenFile;
  int NbCol, NbLine;
  guchar *pucImaOrig, *pucImaRes;
  GdkPixbuf *pGdkPixbufIma;

  pfsOpenFile = GTK_FILE_SELECTION(*(ppwTabArgs));
  if (pfsOpenFile == NULL){
    printf("Mauvaise initialisation du GtkFileSection.\n");
    exit(0);
  }
  /* chargement en memoire de l'image */
  pGdkPixbufImaIn = gdk_pixbuf_new_from_file (pcFileName,NULL);
  /* echec du chargement de l'image en memoire */
  if (pGdkPixbufImaIn == NULL){
    printf("Impossible de charger le fichier %s\n", pcFileName);
    exit(0);
  }

  gchar *pcFileName;
  pcFileName = (gchar*) gtk_file_selection_get_filename(pfsOpenFile);
  NbCol=gdk_pixbuf_get_width(pGdkPixbufImaOrig); 
  NbLine=gdk_pixbuf_get_height(pGdkPixbufImaOrig);

}
