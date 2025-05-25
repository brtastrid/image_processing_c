#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Compute the histogram of an 8-bit BMP image
unsigned int *bmp8_computeHistogram(t_bmp8 *img) {
    unsigned int *hist = calloc(256, sizeof(unsigned int));
    if (!hist) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        hist[img->data[i]]++;
    }

    return hist;
}

// Compute the Cumulative Distribution Function (CDF) from a histogram
unsigned int *bmp8_computeCDF(unsigned int *hist) {
    unsigned int *cdf = malloc(256 * sizeof(unsigned int));
    if (!cdf) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    return cdf;
}

// Apply histogram equalization to an 8-bit BMP image
void bmp8_equalize(t_bmp8 *img, unsigned int *cdf) {
    unsigned char map[256];
    unsigned int totalPixels = img->width * img->height;

    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    printf("\nCDF Preview\n");
    for (int i = 0; i < 256; i += 32) {
        printf("cdf[%3d] = %u\n", i, cdf[i]);
    }

    for (int i = 0; i < 256; i++) {
        map[i] = (unsigned char) roundf(((float)(cdf[i] - cdf_min) / (totalPixels - cdf_min)) * 255.0f);
    }

    printf("\nLUT Mapping\n");
    for (int i = 0; i < 256; i += 32) {
        printf("map[%3d] = %d\n", i, map[i]);
    }

    printf("\nSample pixel values\n");
    for (int i = 0; i < 10; i++) {
        unsigned char old = img->data[i];
        unsigned char new = map[old];
        printf("Pixel[%d]: %d -> %d\n", i, old, new);
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = map[img->data[i]];
    }
}

// Load an 8-bit BMP image from a file
t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("Unable to open file %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = malloc(sizeof(t_bmp8));
    if (!img) {
        fclose(f);
        printf("Memory allocation failed\n");
        return NULL;
    }

    if (fread(img->header, sizeof(unsigned char), 54, f) != 54) {
        printf("Couldn't read BMP header.\n");
        free(img);
        fclose(f);
        return NULL;
    }

    img->width       = *(unsigned int *)&img->header[18];
    img->height      = *(unsigned int *)&img->header[22];
    img->colorDepth  = *(unsigned short *)&img->header[28];
    img->dataSize    = *(unsigned int *)&img->header[34];

    if (img->colorDepth != 8) {
        printf("Only 8-bit grayscale images are supported.\n");
        free(img);
        fclose(f);
        return NULL;
    }

    if (fread(img->colorTable, sizeof(unsigned char), 1024, f) != 1024) {
        printf("Color palette read error.\n");
        free(img);
        fclose(f);
        return NULL;
    }

    if (img->dataSize == 0) {
        int rowSize = ((img->width + 3) / 4) * 4;
        img->dataSize = rowSize * img->height;
    }

    img->data = malloc(img->dataSize);
    if (!img->data) {
        printf("Memory allocation failed.\n");
        free(img);
        fclose(f);
        return NULL;
    }

    if (fread(img->data, sizeof(unsigned char), img->dataSize, f) != img->dataSize) {
        printf("Pixel read failed.\n");
        free(img->data);
        free(img);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return img;
}

// Save an 8-bit BMP image to a file
void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Save error (no spaces in name): %s\n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, f);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, f);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, f);

    printf("Image saved in %s\n", filename);
    fclose(f);
}

// Free memory allocated for an 8-bit BMP image
void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

// Print information about an 8-bit BMP image
void bmp8_printInfo(const t_bmp8 *img) {
    printf("\nImage information:\n");
    printf("Width        : %u pixels\n", img->width);
    printf("Height       : %u pixels\n", img->height);
    printf("Color Depth  : %u bits\n", img->colorDepth);
    printf("Image Size   : %u bytes\n", img->dataSize);
}

// Apply a negative effect to an 8-bit BMP image
void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

// Adjust the brightness of an 8-bit BMP image
void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int temp = img->data[i] + value;
        img->data[i] = (temp > 255) ? 255 : (temp < 0 ? 0 : (unsigned char)temp);
    }
}

// Apply a threshold effect to an 8-bit BMP image
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

// Apply a convolution filter to an 8-bit BMP image
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    int n = kernelSize / 2;
    unsigned char *newData = malloc(img->dataSize);
    if (!newData) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            float pixel = 0.0f;
            for (int ky = -n; ky <= n; ky++) {
                for (int kx = -n; kx <= n; kx++) {
                    int ix = x + kx;
                    int iy = y + ky;
                    if (ix >= 0 && ix < (int)img->width && iy >= 0 && iy < (int)img->height) {
                        pixel += img->data[iy * img->width + ix] * kernel[ky + n][kx + n];
                    }
                }
            }
            if (pixel < 0) pixel = 0;
            if (pixel > 255) pixel = 255;
            newData[y * img->width + x] = (unsigned char)roundf(pixel);
        }
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = newData[i];
    }
    free(newData);
}

// Apply a box blur filter to an 8-bit BMP image
void bmp8_boxBlur(t_bmp8 *img) {
    float box[3][3] = {
        {1/9.f, 1/9.f, 1/9.f},
        {1/9.f, 1/9.f, 1/9.f},
        {1/9.f, 1/9.f, 1/9.f}
    };
    float* kernel[3] = { box[0], box[1], box[2] };
    bmp8_applyFilter(img, kernel, 3);
}

// Apply a Gaussian blur filter to an 8-bit BMP image
void bmp8_gaussianBlur(t_bmp8 *img) {
    float gauss[3][3] = {
        {1/16.f, 2/16.f, 1/16.f},
        {2/16.f, 4/16.f, 2/16.f},
        {1/16.f, 2/16.f, 1/16.f}
    };
    float* kernel[3] = { gauss[0], gauss[1], gauss[2] };
    bmp8_applyFilter(img, kernel, 3);
}

// Apply an outline filter to an 8-bit BMP image
void bmp8_outline(t_bmp8 *img) {
    float outline[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    float* kernel[3] = { outline[0], outline[1], outline[2] };
    bmp8_applyFilter(img, kernel, 3);
}

// Apply an emboss filter to an 8-bit BMP image
void bmp8_emboss(t_bmp8 *img) {
    float emboss[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };
    float* kernel[3] = { emboss[0], emboss[1], emboss[2] };
    bmp8_applyFilter(img, kernel, 3);
}

// Apply a sharpen filter to an 8-bit BMP image
void bmp8_sharpen(t_bmp8 *img) {
    float sharpen[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    float* kernel[3] = { sharpen[0], sharpen[1], sharpen[2] };
    bmp8_applyFilter(img, kernel, 3);
}
