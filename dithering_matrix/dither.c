#include "./dither.h"

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

float **get_index_matrix(int dimension){
    if (dimension == 2){
        float **index_matrix = create_float_matrix(dimension, dimension);
        index_matrix[0][0] = 1, index_matrix[0][1] = 2;
        index_matrix[1][0] = 3, index_matrix[1][1] = 0;

        return index_matrix;
    }

    int lower_dimension = dimension / 2;
    float **lower_order_index_matrix = get_index_matrix(lower_dimension);

    float **index_matrix = create_float_matrix(dimension, dimension);

    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            if (i < lower_dimension){
                if (j < lower_dimension){
                    index_matrix[i][j] = lower_order_index_matrix[i][j] * 4 + 1;
                }
                else{
                    index_matrix[i][j] = lower_order_index_matrix[i][j - lower_dimension] * 4 + 2;
                }
            }
            else{
                if (j < lower_dimension){
                    index_matrix[i][j] = lower_order_index_matrix[i - lower_dimension][j] * 4 + 3;
                }
                else{
                    index_matrix[i][j] = lower_order_index_matrix[i - lower_dimension][j - lower_dimension] * 4;
                }
            }
        }
    }

    delete_float_matrix(dimension, dimension, lower_order_index_matrix);   
    return index_matrix;
}

float **get_threshold_matrix(float **index_matrix, int dimension){
    float **threshold_matrix = create_float_matrix(dimension, dimension);

    for (int a = 0; a < dimension; a++){
        for (int b = 0; b < dimension; b++){
            threshold_matrix[a][b] = (index_matrix[a][b] + 0.5) * MAX_INTENSITY / (pow(dimension, 2));
        }
    }

    return threshold_matrix;
}

Image *get_halftoned_image(Image *image_ptr, float **threshold_matrix, int dimension){
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    colors color = image_ptr->color;

    Image *halftone_image_ptr = create_image(rows, cols, color);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            for (int k = 0; k < color; k++) {
                float threshold = threshold_matrix[i % dimension][j % dimension];
                if ((image_ptr->image)[i][j][k] <= threshold){
                    (halftone_image_ptr->image)[i][j][k] = MIN_INTENSITY;
                }
                else{
                    (halftone_image_ptr->image)[i][j][k] = MAX_INTENSITY;
                }
            }
        }
    }

    return halftone_image_ptr;
}