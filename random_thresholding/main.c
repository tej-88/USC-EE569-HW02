#include "./random.h"

int main(int argc, char *argv[]){
    if (argc != 5){
        printf("[Usage]: ./random input_filename rows cols color\n");
        exit(1);
    }

    char *input_filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    colors color = (colors)atoi(argv[4]);

    char *filename = get_image_filename(input_filename);
    char random_threshold_filename[50];
    sprintf(random_threshold_filename, "./output_images/random/%s.raw", filename);

    Image *image_ptr = read_image(input_filename, rows, cols, color);

    Image *halftone_image_ptr = random_thresholding(image_ptr);

    write_image(random_threshold_filename, halftone_image_ptr);  

    delete_image(image_ptr);
    delete_image(halftone_image_ptr);

    return 0;
}
