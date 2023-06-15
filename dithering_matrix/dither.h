#ifndef DITHER
#define DITHER

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

float **create_float_matrix(int rows, int cols);

void delete_float_matrix(int rows, int cols, float **matrix);

float **get_index_matrix(int dimension);

float **get_threshold_matrix(float **index_matrix, int dimension);

Image *get_halftoned_image(Image *image_ptr, float **threshold_matrix, int dimension);
#endif

