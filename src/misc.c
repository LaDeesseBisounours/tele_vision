#include<stdio.h>
#include<stdlib.h>
#include"misc.h"

unsigned char*
getNeighborList(struct packedPixel* tab, int index, int height, int width) {
  unsigned char* res = calloc(5, sizeof(unsigned char));
  if (res == NULL)
    return NULL;
  if (index < 0 || index >= height * width) {
    return NULL;
  }
  res[0] = tab[index].a1;  //first char for first bit

  if ((index % width) - 1 < 0)  //clockwise starting with the left one
    res[1] = tab[index].a1;
  else
    res[1] = tab[index - 1].a1;

  if (index - width < 0)  //up
    res[2] = tab[index].a1;
  else
    res[2] = tab[index - width].a1;

  if ((index % width) + 1 >= width && index + 1 < height * width)  //right
    res[3] = tab[index].a1;
  else
    res[3] = tab[index + 1].a1;


  if (index + width > height * width)  // down
    res[4] = tab[index].a1;
  else
    res[4] = tab[index + width].a1;

  return res;
}


