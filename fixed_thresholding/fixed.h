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

char *get_image_filename(char *full_path);

Image *read_image(char *filename, int rows, int cols, colors color);

void write_image(char *filename, Image *image_ptr);

void delete_image(Image *image_ptr);

Image *create_image(int rows, int cols, colors color);

byte float_to_byte(float value);

Image *fixed_thresholding(Image *image_ptr, byte threshold);

#endif