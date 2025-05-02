#include <stdio.h>
#include "t_bmp8.h"

int main() {
    t_bmp8 *img = bmp8_loadImage("lena_gray.bmp");
    if (!img) {
        printf("Failed to load image\n");
        return 1;
    }

    bmp8_printInfo(img);

    bmp8_negative(img);
    bmp8_saveImage("lena_negative.bmp", img);

    bmp8_brightness(img, 50);
    bmp8_saveImage("lena_bright.bmp", img);

    bmp8_threshold(img, 128);
    bmp8_saveImage("lena_threshold.bmp", img);

    int hist[256];
    bmp8_computeHistogram(img, hist);
    for (int i = 0; i < 256; i++) {
        printf("Pixel %d: %d times\n", i, hist[i]);
    }

    bmp8_free(img);
    return 0;
}
