struct packedPixel{
  unsigned char a1;
  unsigned char a2;
  unsigned char a3;
};

unsigned char*
getNeighborList(struct packedPixel* tab, int index, int height, int width);


unsigned long getMediane(unsigned long *tab);
