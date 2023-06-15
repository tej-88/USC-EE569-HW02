#ifndef SOBEL
#define SOBEL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INTENSITY 255
#define MIN_INTENSITY 0
#define KERNEL_SIZE 3

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

Image *image_padding(Image *image_ptr, int kernel_size);

Image *image_convolution(Image *image_ptr, float **kernel, int kernel_size);

void write_image(char *filename, Image *image_ptr);

Image *create_image(int rows, int cols, colors color);

void delete_image(Image *image_ptr);

float **create_float_matrix(int rows, int cols);

void delete_float_matrix(int rows, int cols, float **matrix);

byte float_to_byte(float value);

unsigned int **get_histogram(Image *image_ptr);

float **get_PDF(Image *image_ptr);

float **get_CDF(Image *image_ptr);

float **get_Gx(void);

float **get_Gy(void);

Image *rgb_to_gray(Image *rgb_image_ptr);

Image *image_normalization(Image *image_ptr);

Image *create_gradient_magnitude_map(Image *image_Gx_ptr, Image *image_Gy_ptr);

Image *create_edge_map(Image *image_ptr, float threshold);

#endif