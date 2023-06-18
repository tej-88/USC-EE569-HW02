#include "./jjn.h"

char *get_image_filename(char *full_path){
    int backslash_idx = 0;
    int period_idx = 0;

    for (int i = strlen(full_path); i >= 0; i--){
        if (full_path[i] == '.'){
            period_idx = i;
        }
        else if (full_path[i] == '/'){
            backslash_idx = i;
            break;
        }
    }
    int file_len = (period_idx - backslash_idx);

    char *filename = (char *)malloc(sizeof(char) * file_len);
    for (int i = 0; i < file_len - 1; i++){
        filename[i] = full_path[backslash_idx + 1 + i];
    }
    filename[file_len - 1] = '\0';
    return filename;
}

Image *read_image(char *filename, int rows, int cols, colors color) {
    Image *image_ptr = create_image(rows, cols, color);

    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL){
        printf("%s could not be opened.\n", filename);
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fread((image_ptr->image)[i][j], sizeof(byte), color, fptr);
        }
    }

    fclose(fptr);
    return image_ptr;
}

void write_image(char *filename, Image *image_ptr) {
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    colors color = image_ptr->color;

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL){
        printf("%s could not be opened.\n", filename);
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fwrite((image_ptr->image)[i][j], sizeof(byte), color, fptr);
        }
    }

    fclose(fptr);
    return;
}

Image *create_image(int rows, int cols, colors color) {
    Image *image_ptr = (Image *)malloc(sizeof(Image));

    image_ptr->rows = rows;
    image_ptr->cols = cols;
    image_ptr->color = color;

    image_ptr->image = (byte ***)malloc(sizeof(byte **) * rows);
    for (int i = 0; i < rows; i++) {
        (image_ptr->image)[i] = (byte **)malloc(sizeof(byte *) * cols);
        for (int j = 0; j < cols; j++) {
            (image_ptr->image)[i][j] = (byte *)malloc(sizeof(byte) * color);
        }
    }

    return image_ptr;
}

void delete_image(Image *image_ptr) {
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    colors color = image_ptr->color;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            free((image_ptr->image)[i][j]);
        }
        free((image_ptr->image)[i]);
    }
    free(image_ptr->image);

    free(image_ptr);
    return;
}

Image *image_padding(Image *image_ptr, int kernel_size){
    int original_rows = image_ptr->rows;
    int original_cols = image_ptr->cols;
    colors original_color = image_ptr->color;

    int offset = (kernel_size - 1) / 2;

    int padded_rows = original_rows + offset * 2;
    int padded_cols = original_cols + offset * 2;
    int padded_color = original_color;

    Image *padded_image_ptr = create_image(padded_rows, padded_cols, padded_color);
    
    for (int i = 0; i < original_rows; i++){
        for (int j = 0; j < original_cols; j++){
            for (int k = 0; k < original_color; k++) {
                (padded_image_ptr->image)[i + offset][j + offset][k] = (image_ptr->image)[i][j][k];
            }
        }
    }

    for (int i = 0; i < padded_rows; i++){
        for (int j = 0; j < offset; j++){
            for (int k = 0; k < padded_color; k++) {
                (padded_image_ptr->image)[i][j][k] = (padded_image_ptr->image)[i][offset * 2 - j][k];
                (padded_image_ptr->image)[i][padded_cols - 1 - j][k] = (padded_image_ptr->image)[i][padded_cols - 1 - offset * 2 + j][k];
            }
        }
    }

    for (int a = 0; a < offset; a++){
        for (int b = 0; b < padded_cols; b++){
            for (int c = 0; c < padded_color; c++) {
                (padded_image_ptr->image)[a][b][c] = (padded_image_ptr->image)[offset * 2 - a][b][c];
                (padded_image_ptr->image)[padded_rows - 1 - a][b][c] = (padded_image_ptr->image)[padded_rows - 1 - offset * 2 + a][b][c];
            }
        }
    }

    return padded_image_ptr;
}

float ***image_to_float(Image *image_ptr) {
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    colors color = image_ptr->color;

    float ***matrix = create_float_image(rows, cols, color);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < color; k++) {
                matrix[i][j][k] = (float)(image_ptr->image)[i][j][k];
            }
        }
    }

    return matrix;
}

float **get_mirror_kernel(float **kernel, int kernel_size){
    float **mirror_kernel = create_float_matrix(kernel_size, kernel_size);

    for (int i = 0; i < kernel_size; i++){
        for (int j = 0; j < kernel_size / 2; j++){
            mirror_kernel[i][j] = kernel[i][kernel_size - 1 - j];
        }
    }

    return mirror_kernel;
}

float **create_float_matrix(int rows, int cols) {
    float **matrix = (float **)malloc(sizeof(float *) * rows);
    for (int i = 0; i < rows; i++) {
        matrix[i] = (float *)malloc(sizeof(float) * cols);
    }

    return matrix;
}

void delete_float_matrix(int rows, int cols, float **matrix) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }

    free(matrix);
    return;
}

Image *serpentine_error_diffusion(Image *image_ptr, float **kernel, int kernel_size) {
    byte threshold = 128;

    int original_rows = image_ptr->rows;
    int original_cols = image_ptr->cols;
    colors original_color = image_ptr->color;
    
    Image *halftoned_image_ptr = create_image(original_rows, original_cols, original_color);

    Image *padded_image_ptr = image_padding(image_ptr, kernel_size);

    float ***float_image = image_to_float(padded_image_ptr);

    float **m_kernel = get_mirror_kernel(kernel, kernel_size);

    int offset = (kernel_size - 1) / 2;
    int padded_rows = padded_image_ptr->rows;
    int padded_cols = padded_image_ptr->cols;
    colors padded_color = padded_image_ptr->color;
    Image *binary_image_ptr = create_image(padded_rows, padded_cols, padded_color);

    for (int i = 0; i < original_rows; i++){
        if ((i % 2) == 0){
            for (int j = 0; j < original_cols; j++){
                for (int k = 0; k < original_color; k++){
                    if (float_image[i + offset][j + offset][k] > threshold){
                        (binary_image_ptr->image)[i +offset][j + offset][k] = MAX_INTENSITY;
                    }
                    else {
                        (binary_image_ptr->image)[i +offset][j + offset][k] = MIN_INTENSITY;
                    }
                    float error = float_image[i + offset][j + offset][k] - (binary_image_ptr->image)[i +offset][j + offset][k];
                    for (int a = -offset; a<= offset; a++){
                        for (int b = -offset; b <= offset; b++){
                            float_image[i + offset + a][j + offset + b][k] += error * kernel[a + offset][b + offset];
                        }
                    }
                }
            }
        }
        else {
            for (int j = original_cols - 1; j >= 0; j--){
                for (int k = 0; k < original_color; k++){
                    if (float_image[i + offset][j + offset][k] > threshold){
                        (binary_image_ptr->image)[i +offset][j + offset][k] = MAX_INTENSITY;
                    }
                    else {
                        (binary_image_ptr->image)[i +offset][j + offset][k] = MIN_INTENSITY;
                    }
                    float error = float_image[i + offset][j + offset][k] - (binary_image_ptr->image)[i +offset][j + offset][k];
                    for (int a = -offset; a<= offset; a++){
                        for (int b = -offset; b <= offset; b++){
                            float_image[i + offset + a][j + offset + b][k] += error * m_kernel[a + offset][b + offset];
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < original_rows; i++){
            for (int j = 0; j < original_cols; j++){
                for (int k = 0; k < original_color; k++){
                    (halftoned_image_ptr->image)[i][j][k] = (binary_image_ptr->image)[i + offset][j + offset][k];
                }
            }
    }

    delete_float_matrix(kernel_size, kernel_size, m_kernel);
    delete_float_image(float_image, padded_rows, padded_cols, padded_color);
    delete_image(padded_image_ptr);
    delete_image(binary_image_ptr);

    return halftoned_image_ptr;
}

float ***create_float_image(int rows, int cols, colors color) {
    float ***image = (float ***)malloc(sizeof(float **) * rows);

    for (int i = 0; i < rows; i++) {
        image[i] = (float **)malloc(sizeof(float *) * cols);
        for (int j = 0; j < cols; j++) {
            image[i][j] = (float *)malloc(sizeof(float) * color);
        }
    }

    return image;
}

void delete_float_image(float ***image, int rows, int cols, colors color) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return;
}

float **get_jjn_kernel(void){
    float **kernel = create_float_matrix(KERNEL_SIZE, KERNEL_SIZE);

    kernel[0][0] = 0.0, kernel[0][1] = 0.0, kernel[0][2] = 0.0, kernel[0][3] = 0.0, kernel[0][4] = 0.0;
    kernel[1][0] = 0.0, kernel[1][1] = 0.0, kernel[1][2] = 0.0, kernel[1][3] = 0.0, kernel[1][4] = 0.0;
    kernel[2][0] = 0.0, kernel[2][1] = 0.0, kernel[2][2] = 0.0, kernel[2][3] = 7.0 / 48, kernel[2][4] = 5.0 / 48;
    kernel[3][0] = 0.0, kernel[3][1] = 5.0 / 48, kernel[3][2] = 7.0 / 48, kernel[3][3] = 5.0 / 48, kernel[3][4] = 0.0;
    kernel[4][0] = 1.0 / 48, kernel[4][1] = 3.0 / 48, kernel[4][2] = 5.0 / 48, kernel[4][3] = 3.0 / 48, kernel[4][4] = 1.0 / 48;

    return kernel;
}
