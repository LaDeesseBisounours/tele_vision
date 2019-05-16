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

unsigned long getMediane(unsigned long *tab) {
  for (int i = 0; i < 5; i++) {
    int greater = 0;
    int smaller = 0;
    unsigned long tmp = tab[i];
    for (int j = 0; j < 5; j++) {
      if (tmp > tab[j])
        greater++;
      if (tmp < tab[j])
        smaller++;
    }
    if (greater <= 2 && smaller <= 2)
      return tmp;
  }
}

DIR *d = NULL;
struct dirent *dir = NULL;
char *listPictures() {
  if (!d) {
    d = opendir(".");
    if (!d)
      printf("error\n");
  }
  if ((dir = readdir(d)) != NULL) {
      char *tmp = dir->d_name;
      size_t size = strlen(dir->d_name);
      char *new = calloc(1, size + 1);
      strncpy(new, tmp, strlen(tmp));

      if (size > 4 && !strncmp(new + size - 4, ".bmp", 4))
        return new;
      free(new);
      return listPictures();
  }
  else {
    closedir(d);
    return NULL;
  }
}
