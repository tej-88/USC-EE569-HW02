#include "./sobel.h"

int main(int argc, char *argv[]) {
    if (argc != 6){
        printf("[Usage]: ./sobel input_filename rows cols colors threshold\n");
        exit(1);
    }

    char *input_filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    colors color = (colors) atoi(argv[4]);
    float threshold = atof(argv[5]);

    char *filename = get_image_filename(input_filename);
    char gray_image_filename[50];
    char image_Gx_filename[50];
    char image_Gy_filename[50];
    char image_grad_mag_filename[50];
    char edge_map_filename[50];

    float **Gx = get_Gx();
    float **Gy = get_Gy();

    sprintf(gray_image_filename, "./output_images/sobel/%s_gray_%.0f.raw", filename, threshold);
    sprintf(image_Gx_filename, "./output_images/sobel/%s_Gx_%.0f.raw", filename, threshold);
    sprintf(image_Gy_filename, "./output_images/sobel/%s_Gy_%.0f.raw", filename, threshold);
    sprintf(image_grad_mag_filename, "./output_images/sobel/%s_grad_mag_map_%.0f.raw", filename, threshold);
    sprintf(edge_map_filename, "./output_images/sobel/%s_edge_map_%.0f.raw", filename, threshold);

    Image *rgb_image_ptr = read_image(input_filename, rows, cols, color);

    Image *gray_image_ptr = rgb_to_gray(rgb_image_ptr);

    Image *image_Gx_ptr = image_convolution(gray_image_ptr, Gx, KERNEL_SIZE);
    Image *image_Gx_norm_ptr = image_normalization(image_Gx_ptr);

    Image *image_Gy_ptr = image_convolution(gray_image_ptr, Gy, KERNEL_SIZE);
    Image *image_Gy_norm_ptr = image_normalization(image_Gy_ptr);

    Image *image_grad_mag_ptr = create_gradient_magnitude_map(image_Gx_norm_ptr, image_Gy_norm_ptr);
    Image *image_grad_mag_norm_ptr = image_normalization(image_grad_mag_ptr);

    Image *edge_map_ptr = create_edge_map(image_grad_mag_norm_ptr, threshold);

    write_image(gray_image_filename, gray_image_ptr);
    write_image(image_Gx_filename, image_Gx_norm_ptr);
    write_image(image_Gy_filename, image_Gy_norm_ptr);
    write_image(image_grad_mag_filename, image_grad_mag_norm_ptr);
    write_image(edge_map_filename, edge_map_ptr);

    free(filename);
    delete_float_matrix(KERNEL_SIZE, KERNEL_SIZE, Gx);
    delete_float_matrix(KERNEL_SIZE, KERNEL_SIZE, Gy);

    delete_image(rgb_image_ptr);
    delete_image(gray_image_ptr);
    delete_image(image_Gx_ptr);
    delete_image(image_Gx_norm_ptr);
    delete_image(image_Gy_ptr);
    delete_image(image_Gy_norm_ptr);
    delete_image(image_grad_mag_ptr);
    delete_image(image_grad_mag_norm_ptr);
    delete_image(edge_map_ptr);

    return 0;
}