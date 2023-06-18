#include "./stucki.h"

int main(int argc, char *argv[]){
    if (argc != 5){
        printf("[Usage]: ./stucki input_filename rows cols color \n");
        exit(1);
    }

    char *input_filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    colors color = (colors)atoi(argv[4]);

    char *filename = get_image_filename(input_filename);
    char stucki_filename[50];
    sprintf(stucki_filename, "./output_images/%s_stucki.raw", filename);

    Image *image_ptr = read_image(input_filename, rows, cols, color);

    float **kernel = get_stucki_kernel();

    Image *halftoned_image_ptr = serpentine_error_diffusion(image_ptr, kernel, KERNEL_SIZE);

    write_image(stucki_filename, halftoned_image_ptr);

    delete_float_matrix(KERNEL_SIZE, KERNEL_SIZE, kernel);
    delete_image(image_ptr);
    delete_image(halftoned_image_ptr);

    return 0;
}
