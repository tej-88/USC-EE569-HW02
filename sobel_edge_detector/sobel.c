#include "./sobel.h"

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

Image *image_convolution(Image *image_ptr, float **kernel, int kernel_size){
    int original_rows = image_ptr->rows;
    int original_cols = image_ptr->cols;
    colors original_color = image_ptr->color;
    
    Image *padded_image_ptr = image_padding(image_ptr, kernel_size);

    int offset = (kernel_size - 1) / 2;
    int padded_rows = padded_image_ptr->rows;
    int padded_cols = padded_image_ptr->cols;
    colors padded_color = padded_image_ptr->color;

    Image *convolved_image_ptr = create_image(original_rows, original_cols, original_color);

    float *convolution_sum = (float *)malloc(sizeof(float *) * original_color);

    for (int i = 0; i < original_rows; i++){
        for (int j = 0; j < original_cols; j++){
            for (int k = 0; k < original_color; k++){
                convolution_sum[k] = 0;
                for (int a = -offset; a<= offset; a++){
                    for (int b = -offset; b <= offset; b++){
                        convolution_sum[k] += (padded_image_ptr->image)[i + offset + a][j + offset + b][k] * kernel[a + offset][b + offset];
                    }
                }

                (convolved_image_ptr->image)[i][j][k] = float_to_byte(convolution_sum[k]);
            }
        }
    }

    free(convolution_sum);
    delete_image(padded_image_ptr);

    return convolved_image_ptr;
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

unsigned int **get_histogram(Image *image_ptr){
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    colors color = image_ptr->color;

    unsigned int **histogram = (unsigned int **)malloc((MAX_INTENSITY + 1) * sizeof(unsigned int*));
    for (int i = 0; i <= MAX_INTENSITY; i++) {
        histogram[i] = (unsigned int*)malloc(sizeof(unsigned int) * color);
    }

    for (int i = 0; i <= MAX_INTENSITY; i++){
        for (int j = 0; j < color; j++) {
            histogram[i][j] = 0;
        }
    }

    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            for (int c = 0; c < color; c++) {
                histogram[(image_ptr->image)[a][b][c]][c]++;
            }
        }
    }

    return histogram;
}

float **get_PDF(Image *image_ptr){
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    colors color = image_ptr->color;

    unsigned int **histogram = get_histogram(image_ptr);

    float **PDF = create_float_matrix(MAX_INTENSITY + 1, color);
    
    for (int i = 0; i <= MAX_INTENSITY; i++){
        for (int j = 0; j < color; j++) {
            PDF[i][j] = histogram[i][j] / (float) (rows * cols);
        }
    }

    for (int a = 0; a <= MAX_INTENSITY; a++) {
        free(histogram[a]);
    }
    free(histogram);

    return PDF;
}

float **get_CDF(Image *image_ptr){
    colors color = image_ptr->color;

    float **PDF = get_PDF(image_ptr);
    float **CDF = create_float_matrix(MAX_INTENSITY + 1, color);
    
    for (int i = 0; i < color; i++){
        CDF[0][i] = PDF[0][i];
    }

    for (int i = 1; i <= MAX_INTENSITY; i++){
        for (int j = 0; j < color; j++) {
            CDF[i][j] = PDF[i][j] + CDF[i - 1][j];
        }
    }

    delete_float_matrix(MAX_INTENSITY + 1, color, PDF);

    return CDF;
}

byte float_to_byte(float value) {
    if (value >= MAX_INTENSITY) {
        return MAX_INTENSITY;
    }
    else if (value <= MIN_INTENSITY) {
        return MIN_INTENSITY;
    }
    else {
        return round(value);
    }
}

float **get_Gx(void) {
    float **Gx = create_float_matrix(KERNEL_SIZE, KERNEL_SIZE);

    Gx[0][0] = -1.0, Gx[0][1] = 0.0, Gx[0][2] = 1.0;
    Gx[1][0] = 0.0, Gx[1][1] = 0.0, Gx[1][2] = 0.0;
    Gx[2][0] = -1.0, Gx[2][1] = 0.0, Gx[2][2] = 1.0;

    return Gx;
}

float **get_Gy(void) {
    float **Gy = create_float_matrix(KERNEL_SIZE, KERNEL_SIZE);

    Gy[0][0] = 1.0, Gy[0][1] = 2.0, Gy[0][2] = 1.0;
    Gy[1][0] = 0.0, Gy[1][1] = 0.0, Gy[1][2] = 0.0;
    Gy[2][0] = -1.0, Gy[2][1] = -2.0, Gy[2][2] = -1.0;
    
    return Gy;
}

Image *rgb_to_gray(Image *rgb_image_ptr) {
    int rows = rgb_image_ptr->rows;
    int cols = rgb_image_ptr->cols;

    Image *gray_image_ptr = create_image(rows, cols, gray);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            (gray_image_ptr->image)[i][j][0] = float_to_byte(0.2989 * (rgb_image_ptr->image)[i][j][0] + 0.5870 * (rgb_image_ptr->image)[i][j][1] + 0.1140 * (rgb_image_ptr->image)[i][j][2]);
        }
    }

    return gray_image_ptr;
}

Image *image_normalization(Image *image_ptr) {
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    colors color = image_ptr->color;

    int max_value = (image_ptr->image)[0][0][0];
    int min_value = (image_ptr->image)[0][0][0];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((image_ptr->image)[i][j][0] > max_value) {
                max_value = (image_ptr->image)[i][j][0];
            }

            if ((image_ptr->image)[i][j][0] < min_value) {
                min_value = (image_ptr->image)[i][j][0];
            }
        }
    }

    Image *normalized_image_ptr = create_image(rows, cols, color);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            (normalized_image_ptr->image)[i][j][0] = float_to_byte(MAX_INTENSITY * ((image_ptr->image)[i][j][0] - min_value) / (float)(max_value - min_value));
        }
    }

    return normalized_image_ptr;
}

Image *create_gradient_magnitude_map(Image *image_Gx_ptr, Image *image_Gy_ptr) {
    int rows = image_Gx_ptr->rows;
    int cols = image_Gx_ptr->cols;
    colors color = image_Gx_ptr->color;

    Image *gradient_magnitude_map_ptr = create_image(rows, cols, color);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            for (int k = 0; k < color; k++){
                (gradient_magnitude_map_ptr->image)[i][j][k] = float_to_byte(sqrt(pow((image_Gx_ptr->image)[i][j][k], 2) + pow((image_Gy_ptr->image)[i][j][k], 2)));
            }
        }
    }

    return gradient_magnitude_map_ptr;
}

Image *create_edge_map(Image *image_ptr, float threshold) {
    int rows = image_ptr->rows;
    int cols = image_ptr->cols;
    int color = image_ptr->color;

    float **CDF = get_CDF(image_ptr);

    int *pixel_threshold = (int *)malloc(sizeof(int) * color);
    for (int a = 0; a < color; a++) {
        pixel_threshold[a] = 0;
    }


    for (int i = 0; i < color; i++) {
        while (CDF[pixel_threshold[i]][i] < (threshold * CDF[MAX_INTENSITY][i] / 100)){
            pixel_threshold[i]++;
        }
    }

    Image *edge_map_ptr = create_image(rows, cols, color);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            for (int k = 0; k < color; k++){
                if ((image_ptr->image)[i][j][k] > pixel_threshold[k]){
                    (edge_map_ptr->image)[i][j][k] = MAX_INTENSITY;
                }
                else{
                    (edge_map_ptr->image)[i][j][k] = MIN_INTENSITY;
                }
            }
        }
    }

    delete_float_matrix(MAX_INTENSITY + 1, color, CDF);
    free(pixel_threshold);

    return edge_map_ptr;    
}
