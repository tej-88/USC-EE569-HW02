#ifndef FIXED
#define FIXED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INTENSITY 255
#define MIN_INTENSITY 0

typedef enum {rgb = 3, gray = 1} colors;

typedef unsigned char byte;

typedef struct Image
{
    int rows;
    int cols;
    colors color;
    byte ***image;
}Image;


Image *fixed_thresholding(Image *image_ptr, byte threshold);

#endif