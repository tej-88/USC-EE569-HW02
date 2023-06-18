#include "./separable_error.h"

int main(int argc, char *argv[]){
    if (argc != 5){
        printf("[Usage]: ./separable_error input_filename rows cols color \n");
        exit(1);
    }

    char *input_filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    colors color = (colors)atoi(argv[4]);

    char *filename = get_image_filename(input_filename);
    char fs_filename[50];
    sprintf(fs_filename, "./output_images/%s_separable_error.raw", filename);

    Image *image_ptr = read_image(input_filename, rows, cols, color);
    Image *cmy_image_ptr = RGB2CMY(image_ptr);

    float **kernel = get_fs_kernel();

    Image *cmy_halftoned_image_ptr = serpentine_error_diffusion(cmy_image_ptr, kernel, KERNEL_SIZE);
    Image *rgb_halftoned_image_ptr = CMY2RGB(cmy_halftoned_image_ptr);

    write_image(fs_filename, rgb_halftoned_image_ptr);

    delete_float_matrix(KERNEL_SIZE, KERNEL_SIZE, kernel);
    delete_image(image_ptr);
    delete_image(cmy_image_ptr);
    delete_image(cmy_halftoned_image_ptr);
    delete_image(rgb_halftoned_image_ptr);

    return 0;
}