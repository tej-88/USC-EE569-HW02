#ifndef FS
#define FS

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

void write_image(char *filename, Image *image_ptr);

Image *create_image(int rows, int cols, colors color);

Image *image_padding(Image *image_ptr, int kernel_size);

float ***image_to_float(Image *image_ptr);

float **get_mirror_kernel(float **kernel, int kernel_size);

Image *serpentine_error_diffusion(Image *image_ptr, float **kernel, int kernel_size);

float **get_fs_kernel(void);

#endif