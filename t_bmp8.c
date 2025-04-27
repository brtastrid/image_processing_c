// bmp8.c
#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

// Functions to read ans write 8-bit grayscale image
t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: check filename %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Error: Check memory allocation \n");
        fclose(file);
        return NULL;
    }

    fread(img->header, sizeof(unsigned char), 54, file);
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];

    if (img->colorDepth != 8) {
        printf("Error: 8-bit grayscale image is necessary \n");
        fclose(file);
        free(img);
        return NULL;
    }

    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    img->data = (unsigned char *)malloc(img->dataSize);
    if (!img->data) {
        printf("Error: Memory allocation for image data failed\n");
        fclose(file);
        free(img);
        return NULL;
    }

    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    return img;
}
// Saving function
void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: %s cant be opened \n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file); // sieze
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
}
// Freeing function
void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}
// Printing function (all infos)
void bmp8_printInfo(t_bmp8 *img) {
    if (img) {
        printf("Image Info:\n");
        printf("Width: %u\n", img->width);
        printf("Height: %u\n", img->height);
        printf("Color Depth: %u\n", img->colorDepth);
        printf("Data Size: %u\n", img->dataSize);
    }
}


// IMAGE PROCESSING Function
// Negative
void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = 255 - img->data[i];
    }
}

// Brightness
void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        int newVal = img->data[i] + value;
        if (newVal > 255) newVal = 255;
        if (newVal < 0) newVal = 0;
        img->data[i] = (unsigned char)newVal;
    }
}

// Threshold
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

// Filter
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    int n = kernelSize / 2;
    unsigned char *newData = (unsigned char *)malloc(img->dataSize);
    if (!newData) {
        printf("Error: check memory allocation \n");
        return;
    }

    for (unsigned int y = n; y < img->height - n; ++y) {
        for (unsigned int x = n; x < img->width - n; ++x) {
            float sum = 0.0f;
            for (int i = -n; i <= n; ++i) {
                for (int j = -n; j <= n; ++j) {
                    int px = x + j;
                    int py = y + i;
                    sum += img->data[py * img->width + px] * kernel[i + n][j + n];
                }
            }
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            newData[y * img->width + x] = (unsigned char)sum;
        }
    }

    for (unsigned int y = n; y < img->height - n; ++y) {
        for (unsigned int x = n; x < img->width - n; ++x) {
            img->data[y * img->width + x] = newData[y * img->width + x];
        }
    }

    free(newData);
}
