#include <stdio.h>

struct BMPHeader{
  char nameOfFormat[2];
  unsigned int sizeOfFile;
  int reservedBytes;
  unsigned int offsetOfImage;  
};

struct DIBHeader{
  unsigned int sizeOfHeader;
  unsigned int width;
  unsigned int height;
  unsigned short int colorPalet;
  unsigned short int bitPerPixel;
  char notImportant[24];
};
int main(){
  FILE *fp = fopen("amad.bmp","rb");
  if(fp == NULL){ printf("Error happened while opening BMP file.\n"); return 1;}

  struct BMPHeader bmpHeader;
  struct DIBHeader dibHeader; 
  char firstcolor[4]; 

  fread(&bmpHeader.nameOfFormat,2,1,fp);
  fread(&bmpHeader.sizeOfFile,sizeof(int),1,fp);
  fread(&bmpHeader.reservedBytes,sizeof(int),1,fp);
  fread(&bmpHeader.offsetOfImage,sizeof(int),1,fp);
  fread(&dibHeader,sizeof(struct DIBHeader),1,fp);


  fseek(fp,bmpHeader.offsetOfImage,SEEK_SET);
  fread(&firstcolor,4,1,fp);

  printf("%c  %c\nSize with byte: %u\n",bmpHeader.nameOfFormat[0],bmpHeader.nameOfFormat[1],
  bmpHeader.sizeOfFile);
  printf("Offest image data: %u\n",bmpHeader.offsetOfImage);
  printf("Size: %d\nWidth: %d\nHeight: %d\n",dibHeader.sizeOfHeader,dibHeader.width,dibHeader.height);
  printf("Color Palet: %d\nBit per Pixel: %d",dibHeader.colorPalet,dibHeader.bitPerPixel);
  printf("\n%02x %02x %02x %02x",(unsigned char)firstcolor[0],(unsigned char)firstcolor[1],(unsigned char)firstcolor[2],(unsigned char)firstcolor[3]);
  fclose(fp);

  printf("\n");
}

