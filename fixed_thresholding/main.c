#include "./fixed.h"

int main(int argc, char *argv[]) {
    if (argc != 6){
        printf("[Usage]: ./fixed input_filename rows cols color threshold\n");
        exit(1);
    }

    char *input_filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    colors color = (colors) atoi(argv[4]);
    float threshold = atof(argv[5]);

    char *filename = get_image_filename(input_filename);
    char fixed_threshold_filename[50];
    sprintf(fixed_threshold_filename, "./output_images/%s_%.0f.raw", filename, threshold);

    Image *image_ptr = read_image(input_filename, rows, cols, color);

    Image *halftone_image_ptr = fixed_thresholding(image_ptr, float_to_byte(threshold));

    write_image(fixed_threshold_filename, halftone_image_ptr);  

    delete_image(image_ptr);
    delete_image(halftone_image_ptr);

    return 0;
}

