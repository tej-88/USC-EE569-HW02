#include "./fixed.h"


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