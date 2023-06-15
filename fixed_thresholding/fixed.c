#include "./fixed.h"

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

Image *fixed_thresholding(Image *image_ptr, byte threshold){
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    int color = image_ptr->color;

    Image *halftone_image_ptr = create_image(rows, cols, color);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            for (int k = 0; k < color; k++) {
                if ((image_ptr->image)[i][j][k] >= threshold) {
                    (halftone_image_ptr->image)[i][j][k] = MAX_INTENSITY;
                }
                else {
                    (halftone_image_ptr->image)[i][j][k] = MIN_INTENSITY;
                }
            }
        }
    }

    return halftone_image_ptr;
}