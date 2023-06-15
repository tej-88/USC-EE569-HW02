#include "./dither.h"

int main(int argc, char *argv[]){
    if (argc != 6){
        printf("[Usage]: ./dithering_matrix input_filename rows cols color index_matrix_dimension\n");
        exit(1);
    }

    char *input_filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    colors color = (colors)atoi(argv[4]);
    int dimension = (int) pow(2, atoi(argv[5]));

    char *filename = get_image_filename(input_filename);
    char dithering_filename[50];
    sprintf(dithering_filename, "./output_images/%s_%d.raw", filename, dimension);

    Image *image_ptr = read_image(input_filename, rows, cols, color);

    float **index_matrix = get_index_matrix(dimension);
    float **threshold_matrix = get_threshold_matrix(index_matrix, dimension);
    
    Image *halftone_image_ptr = get_halftoned_image(image_ptr, threshold_matrix, dimension);

    write_image(dithering_filename, halftone_image_ptr);

    delete_float_matrix(dimension, dimension, index_matrix);
    delete_float_matrix(dimension, dimension, threshold_matrix);
    delete_image(image_ptr);
    delete_image(halftone_image_ptr);

    return 0;
}

