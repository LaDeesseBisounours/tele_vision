#include <dirent.h>
#include <string.h>

struct packedPixel{
  unsigned char a1;
  unsigned char a2;
  unsigned char a3;
};

unsigned char*
getNeighborList(struct packedPixel* tab, int index, int height, int width);

struct gravityCenter {
  unsigned char curr[5]; //current valu
  unsigned long tmp[5];  //used to compute the new center
  unsigned long nb;      //number of pixels corresponding
  unsigned long diff;
};

unsigned long getMediane(unsigned long *tab);


char *listPictures();
