#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

const size_t BYTES_PER_PIXEL = 3; /// red, green, & blue
const size_t FILE_HEADER_SIZE = 14;
const size_t INFO_HEADER_SIZE = 40;

void generateBitmapImage(unsigned char* image, size_t height, size_t width, char* imageFileName);
unsigned char* createBitmapFileHeader(size_t height, size_t stride);
unsigned char* createBitmapInfoHeader(size_t height, size_t width);

const size_t height = 1080;
const size_t width = 2048;

int main ()
{
    
     unsigned char image[height][width][BYTES_PER_PIXEL];
     char* imageFileName = (char*)"image.png";

     size_t i, j;
     for(i = 0; i < height; i++) {
          for(j = 0; j < width; j++) {
               image[i][j][2] = (unsigned char)(i * 255 / height);             ///red
               image[i][j][1] = (unsigned char)(j * 255 / width);              ///green
               image[i][j][0] = (unsigned char)((i + j) * 255 / (height + width)); ///blue
          }
     }

     generateBitmapImage((unsigned char*)image, height, width, imageFileName);
     printf("Image generated!!");
}


void generateBitmapImage (unsigned char* image, size_t height, size_t width, char* imageFileName)
{

     size_t widthInBytes = width * BYTES_PER_PIXEL;

     unsigned char padding[3] = {0, 0, 0};
     size_t paddingSize = (4 - (widthInBytes) % 4) % 4;

     size_t stride = (widthInBytes)+paddingSize;

     FILE* imageFile = fopen(imageFileName, "wb");

     unsigned char* fileHeader = createBitmapFileHeader(height, stride);
     fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

     unsigned char* infoHeader = createBitmapInfoHeader(height, width);
     fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

     size_t i;
     for(i = 0; i < height; i++) {
          fwrite(image + (i * widthInBytes), BYTES_PER_PIXEL, width, imageFile);
          fwrite(padding, 1, paddingSize, imageFile);
     }

     fclose(imageFile);
}

unsigned char* createBitmapFileHeader (size_t height, size_t stride)
{
     size_t fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

     static unsigned char fileHeader[] = {
          0, 0,     /// signature
          0, 0, 0, 0, /// image file size in bytes
          0, 0, 0, 0, /// reserved
          0, 0, 0, 0, /// start of pixel array
     };

     fileHeader[0] = (unsigned char)('B');
     fileHeader[1] = (unsigned char)('M');
     fileHeader[2] = (unsigned char)(fileSize);
     fileHeader[3] = (unsigned char)(fileSize >> 8);
     fileHeader[4] = (unsigned char)(fileSize >> 16);
     fileHeader[5] = (unsigned char)(fileSize >> 24);
     fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

     return fileHeader;
}

unsigned char* createBitmapInfoHeader (size_t height, size_t width)
{
     static unsigned char infoHeader[] = {
          0, 0, 0, 0, /// header size
          0, 0, 0, 0, /// image width
          0, 0, 0, 0, /// image height
          0, 0,     /// number of color planes
          0, 0,     /// bits per pixel
          0, 0, 0, 0, /// compression
          0, 0, 0, 0, /// image size
          0, 0, 0, 0, /// horizontal resolution
          0, 0, 0, 0, /// vertical resolution
          0, 0, 0, 0, /// colors in color table
          0, 0, 0, 0, /// important color count
     };

     infoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
     infoHeader[4] = (unsigned char)(width);
     infoHeader[5] = (unsigned char)(width >> 8);
     infoHeader[6] = (unsigned char)(width >> 16);
     infoHeader[7] = (unsigned char)(width >> 24);
     infoHeader[8] = (unsigned char)(height);
     infoHeader[9] = (unsigned char)(height >> 8);
     infoHeader[10] = (unsigned char)(height >> 16);
     infoHeader[11] = (unsigned char)(height >> 24);
     infoHeader[12] = (unsigned char)(1);
     infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL * 8);

     return infoHeader;
}
