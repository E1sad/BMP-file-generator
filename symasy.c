#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

struct BMPHeader{
  char nameOfFormat[2]; unsigned int sizeOfFile;
  int reservedBytes; unsigned int offsetOfImage;};
struct DIBHeader{
  unsigned int sizeOfHeader; unsigned int width; unsigned int height;
  unsigned short int colorPalet; unsigned short int bitPerPixel; char notImportant[24];};

int isInteger(const char* str) {
  char* endptr;
  strtol(str, &endptr, 10);
  if(*endptr == '\0') return 1;
  else return 0;}
char* intToString(int number) {
    int length = snprintf(NULL, 0, "%d", number); // Determine the length of the string
    char* str = malloc(length + 1); // Allocate memory for the string (+1 for the null terminator)
    snprintf(str, length + 1, "%d", number); // Convert the integer to a string
    return str;}
int createBMP(struct BMPHeader bmpHeader, struct DIBHeader dibHeader,unsigned char image[],char name[]){
  FILE *fp = fopen(name,"wb");
  if(fp == NULL){printf("Error while creating %s-file",name); return 1;}
  char header[bmpHeader.offsetOfImage];
  int index = 0;
  memset(&header, 0, bmpHeader.offsetOfImage);
  strcpy(header,bmpHeader.nameOfFormat); index += 2;
  memset(&header[index], (unsigned int)bmpHeader.sizeOfFile,1); index += 8;
  memset(&header[index], bmpHeader.offsetOfImage,1); index += 4;
  memset(&header[index], dibHeader.sizeOfHeader,1); index += 4;
  memcpy(&header[index], &dibHeader.width, sizeof(unsigned int)); index += sizeof(unsigned int);
  memcpy(&header[index], &dibHeader.height, sizeof(unsigned int)); index += sizeof(unsigned int);
  memset(&header[index], dibHeader.colorPalet,1); index += 2;
  memset(&header[index], dibHeader.bitPerPixel,1);
  fwrite(header, 1, bmpHeader.offsetOfImage, fp);  // write header array to file
  fwrite(image, 1, bmpHeader.sizeOfFile, fp);  // write image data to file
  fclose(fp); return 0;}
int main(int argc, char *argv[]){
  int number = 1, size = 0, widthAndHeight = 32, seed = 0,asymetric = 1, isVertical = 2; 
  int i,j,n,isMemoryAllocated = 0, isSeedProvided = 0, isNothingProvided = 1;
  char *nameFromUser, *fileName;
  struct BMPHeader bmpHeader; 
  struct DIBHeader dibHeader;
  for(int i = 1; i < argc; i++){
    if(argv[i][0] == '-'){
      if (strcmp(argv[i], "-size") == 0){
        if(isInteger(argv[i+1])){ widthAndHeight = atoi(argv[i+1]); isNothingProvided = 0; i++;}
        else{printf("You should provide size with integer!\n"); return 1;}}
      else if (strcmp(argv[i], "-seed") == 0){
        if(isInteger(argv[i+1])){ seed = atoi(argv[i+1]); isSeedProvided = 1; 
        isNothingProvided = 0; i++;
        }else{printf("You should provide seed with integer!\n"); return 1;}}
      else if (strcmp(argv[i], "-n") == 0){
        if(isInteger(argv[i+1])){number = atoi(argv[i+1]); isNothingProvided = 0; i++;          
        }else{printf("You should provide number with integer!\n"); return 1;}}
      else if (strcmp(argv[i], "-o") == 0){
        if(argv[i+1][0] != '-'){ 
          nameFromUser = (char*)malloc(strlen(argv[i+1])*sizeof(char));
          strcpy(nameFromUser,argv[i+1]); isMemoryAllocated = 1; isNothingProvided = 0; i++;
        }else{printf("You should provide name!\n"); return 1;}}
      else if (argv[i][1] == 'a' || argv[i][1] == 'h' || argv[i][1] == 's' || argv[i][1] == 'v'){
        for(int j = 1; j < strlen(argv[i]); j++){
          if(argv[i][j] == 'a'){ asymetric = 1; isNothingProvided = 0;
          }else if(argv[i][j] == 's'){ asymetric = 0; isNothingProvided = 0;
          }else if(argv[i][j] == 'h'){ isVertical = 0; isNothingProvided = 0;
          }else if(argv[i][j] == 'v'){ isVertical = 1; isNothingProvided = 0;
          }else{printf("You can only choose -[ahsv]"); return 1;}}
      }else{ printf("Usage of symasy: [-ahsv] [-size int] [-seed int] [-n int] [-o name]\n"); return 1;}
    }else{ printf("Usage of symasy: [-ahsv] [-size int] [-seed int] [-n int] [-o name]\n"); return 1;}} 
  if(widthAndHeight < 32){ printf("You can't set size less than 32!\n"); return 1;}
  if(number < 1){printf("You can't set number less than 1!\n"); return 1;}
  if(isMemoryAllocated == 0){ nameFromUser = (char*)malloc(7*sizeof(char));
    strcpy(nameFromUser,"symasy");}
  size = 4*widthAndHeight*widthAndHeight;
  bmpHeader.nameOfFormat[0] = 'B';   bmpHeader.nameOfFormat[1] = 'M';
  bmpHeader.sizeOfFile = (54+size); bmpHeader.reservedBytes = 0;
  bmpHeader.offsetOfImage = 54; dibHeader.sizeOfHeader = 40;
  dibHeader.width = widthAndHeight; dibHeader.height = widthAndHeight;
  dibHeader.colorPalet = 1; dibHeader.bitPerPixel = 32;
  memset(dibHeader.notImportant, 0, sizeof(dibHeader.notImportant));
  unsigned char image[size];
  if(isSeedProvided) srand(seed);
  else srand(time(NULL));
  if(isVertical == 0 || isVertical == 1){}
  else  isVertical = rand()%2;
  for(n = 0; n < number; n++){
    for(i = widthAndHeight - 1; i >= 0; i--){
      for(j = 0; j < widthAndHeight; j++){
        if(j == 0 || j == widthAndHeight-1 || i == 0 || i == widthAndHeight-1){
          image[(i*widthAndHeight+j)*4] = 0;
          image[(i*widthAndHeight+j)*4+1] = 0; 
          image[(i*widthAndHeight+j)*4+2] = 0 ;
          image[(i*widthAndHeight+j)*4+3] = 0;
        }else{
          image[(i*widthAndHeight+j)*4] = 255;
          image[(i*widthAndHeight+j)*4+1] = 255;
          image[(i*widthAndHeight+j)*4+2] = 255;
          image[(i*widthAndHeight+j)*4+3] = 0;}}} 
    for(i = widthAndHeight - 1; i >= 0; i--){
      int lockForColoring = 1;
      if(!asymetric && i == widthAndHeight/2){break;}
      for(j = 0; j < widthAndHeight; j++){
        if(isVertical){
          int startingPoint,shift; shift = rand()%2;
          if(isSeedProvided && shift) srand(seed+(i*-1)+j+n);
          else if(isSeedProvided && !shift) srand(seed+i+n+j);          
          else if(!isSeedProvided && shift) srand(time(NULL)+j+(i*-1)+n); 
          else srand(time(NULL)+j+i+n); 
          if(i == widthAndHeight - 1 && j == 0)startingPoint = rand()%(widthAndHeight-4);
          else if(lockForColoring){
            if(shift) startingPoint += 1;
            else startingPoint -= 1;
            if(startingPoint <= 3 || startingPoint >= widthAndHeight-4){
              if(shift) startingPoint -= 1;
              else startingPoint += 1;}
            lockForColoring = 0;}
          if(j == startingPoint){
            image[(i*widthAndHeight+j)*4] = 0; image[(i*widthAndHeight+j)*4+1] = 0; 
            image[(i*widthAndHeight+j)*4+2] = 0; image[(i*widthAndHeight+j)*4+3] = 0;
            image[(i*widthAndHeight+j+1)*4] = 0; image[(i*widthAndHeight+j+1)*4+1] = 0; 
            image[(i*widthAndHeight+j+1)*4+2] = 0; image[(i*widthAndHeight+j+1)*4+3] = 0;    
            image[(i*widthAndHeight+j-1)*4] = 0; image[(i*widthAndHeight+j-1)*4+1] = 0; 
            image[(i*widthAndHeight+j-1)*4+2] = 0; image[(i*widthAndHeight+j-1)*4+3] = 0;  
            if(!asymetric){
              image[((widthAndHeight-1-i)*widthAndHeight+j)*4] = 0;
              image[((widthAndHeight-1-i)*widthAndHeight+j)*4+1] = 0; 
              image[((widthAndHeight-1-i)*widthAndHeight+j)*4+2] = 0 ;
              image[((widthAndHeight-1-i)*widthAndHeight+j)*4+3] = 0;
              image[((widthAndHeight-1-i)*widthAndHeight+j+1)*4] = 0;
              image[((widthAndHeight-1-i)*widthAndHeight+j+1)*4+1] = 0; 
              image[((widthAndHeight-1-i)*widthAndHeight+j+1)*4+2] = 0 ;
              image[((widthAndHeight-1-i)*widthAndHeight+j+1)*4+3] = 0;    
              image[((widthAndHeight-1-i)*widthAndHeight+j-1)*4] = 0;
              image[((widthAndHeight-1-i)*widthAndHeight+j-1)*4+1] = 0; 
              image[((widthAndHeight-1-i)*widthAndHeight+j-1)*4+2] = 0 ;
              image[((widthAndHeight-1-i)*widthAndHeight+j-1)*4+3] = 0;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j)*4] = 0;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j)*4+1] = 0; 
              image[((widthAndHeight-1-i+1)*widthAndHeight+j)*4+2] = 0 ;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j)*4+3] = 0;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j+1)*4] = 0;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j+1)*4+1] = 0; 
              image[((widthAndHeight-1-i+1)*widthAndHeight+j+1)*4+2] = 0 ;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j+1)*4+3] = 0;    
              image[((widthAndHeight-1-i+1)*widthAndHeight+j-1)*4] = 0;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j-1)*4+1] = 0; 
              image[((widthAndHeight-1-i+1)*widthAndHeight+j-1)*4+2] = 0 ;
              image[((widthAndHeight-1-i+1)*widthAndHeight+j-1)*4+3] = 0;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j)*4] = 0;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j)*4+1] = 0; 
              image[((widthAndHeight-1-i+2)*widthAndHeight+j)*4+2] = 0 ;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j)*4+3] = 0;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j+1)*4] = 0;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j+1)*4+1] = 0; 
              image[((widthAndHeight-1-i+2)*widthAndHeight+j+1)*4+2] = 0 ;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j+1)*4+3] = 0;    
              image[((widthAndHeight-1-i+2)*widthAndHeight+j-1)*4] = 0;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j-1)*4+1] = 0; 
              image[((widthAndHeight-1-i+2)*widthAndHeight+j-1)*4+2] = 0 ;
              image[((widthAndHeight-1-i+2)*widthAndHeight+j-1)*4+3] = 0;}           
            lockForColoring = 1;}
        }if(!isVertical){
          int startingPoint,shift; shift = rand()%2;
          if(isSeedProvided && shift) srand(seed+(i*-1)+j+n);
          else if(isSeedProvided && !shift) srand(seed+i+j+n);          
          else if(!isSeedProvided && shift) srand(time(NULL)+j+(i*-1)+n); 
          else srand(time(NULL)+j+i+n); 
          if(i == widthAndHeight - 1)startingPoint = rand()%(widthAndHeight-4);
          else if(lockForColoring){
            if(startingPoint <= 3 || startingPoint >= widthAndHeight-4){
              if(shift) startingPoint -= 1;
              else startingPoint += 1;
            }else{
              if(shift) startingPoint += 1;
              else startingPoint -= 1;}
            lockForColoring = 0;
          }if(j == startingPoint){
            image[(i+widthAndHeight*j)*4] = 0; image[(i+widthAndHeight*j)*4+1] = 0; 
            image[(i+widthAndHeight*j)*4+2] = 0; image[(i+widthAndHeight*j)*4+3] = 0;
            image[(i+widthAndHeight*(j+1))*4] = 0; image[(i+widthAndHeight*(j+1))*4+1] = 0; 
            image[(i+widthAndHeight*(j+1))*4+2] = 0; image[(i+widthAndHeight*(j+1))*4+3] = 0;    
            image[(i+widthAndHeight*(j-1))*4] = 0; image[(i+widthAndHeight*(j-1))*4+1] = 0; 
            image[(i+widthAndHeight*(j-1))*4+2] = 0; image[(i+widthAndHeight*(j-1))*4+3] = 0;
            if(!asymetric){
              image[((widthAndHeight -1 - i)+widthAndHeight*j)*4] = 0;
              image[((widthAndHeight -1 - i)+widthAndHeight*j)*4+1] = 0; 
              image[((widthAndHeight -1 - i)+widthAndHeight*j)*4+2] = 0 ;
              image[((widthAndHeight -1 - i)+widthAndHeight*j)*4+3] = 0;
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight+1))*4] = 0;
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight+1))*4+1] = 0; 
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight+1))*4+2] = 0 ;
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight+1))*4+3] = 0;    
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight-1))*4] = 0;
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight-1))*4+1] = 0; 
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight-1))*4+2] = 0 ;
              image[((widthAndHeight -1 - i)+widthAndHeight*(widthAndHeight-1))*4+3] = 0;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*j)*4] = 0;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*j)*4+1] = 0; 
              image[((widthAndHeight -1 - i+1)+widthAndHeight*j)*4+2] = 0 ;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*j)*4+3] = 0;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight+1))*4] = 0;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight+1))*4+1] = 0; 
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight+1))*4+2] = 0 ;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight+1))*4+3] = 0;    
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight-1))*4] = 0;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight-1))*4+1] = 0; 
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight-1))*4+2] = 0 ;
              image[((widthAndHeight -1 - i+1)+widthAndHeight*(widthAndHeight-1))*4+3] = 0;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*j)*4] = 0;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*j)*4+1] = 0; 
              image[((widthAndHeight -1 - i+2)+widthAndHeight*j)*4+2] = 0 ;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*j)*4+3] = 0;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight+1))*4] = 0;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight+1))*4+1] = 0; 
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight+1))*4+2] = 0 ;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight+1))*4+3] = 0;    
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight-1))*4] = 0;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight-1))*4+1] = 0; 
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight-1))*4+2] = 0 ;
              image[((widthAndHeight -1 - i+2)+widthAndHeight*(widthAndHeight-1))*4+3] = 0;}                           
            lockForColoring = 1;}}}}
    char* numberStr = intToString(n);
    fileName = malloc(strlen(nameFromUser) + strlen(numberStr) + strlen(".bmp"));      
    strcpy(fileName, nameFromUser); strcat(fileName, numberStr); strcat(fileName, ".bmp");
    createBMP(bmpHeader,dibHeader,image,fileName); free(numberStr); free(fileName);    
    for(i = widthAndHeight - 1; i >= 0; i--){
      for(j = 0; j < widthAndHeight; j++){
          image[(i*widthAndHeight+j)*4] = 255; image[(i*widthAndHeight+j)*4+1] = 255;
          image[(i*widthAndHeight+j)*4+2] = 255; image[(i*widthAndHeight+j)*4+3] = 0;}}}
  free(nameFromUser);
  return 0;}