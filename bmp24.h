#ifndef BMP24_H
#define BMP24_H
#include <stdint.h>

// Structure representing a pixel in BMP 24-bit images
typedef struct {
    uint8_t red;   // Red component
    uint8_t green; // Green component
    uint8_t blue;  // Blue component
} t_pixel;

// Structure representing a BMP 24-bit image
typedef struct {
    int width;       // Width of the image
    int height;      // Height of the image
    int colorDepth;  // Color depth of the image
    t_pixel **data;  // 2D array of pixels
} t_bmp24;

// Function to allocate memory for a 2D pixel array
t_pixel **bmp24_allocateDataPixels(int width, int height);

// Function to free memory allocated for a 2D pixel array
void bmp24_freeDataPixels(t_pixel **pixels, int height);

// Function to free the entire BMP image structure
void bmp24_free(t_bmp24 *img);

// Function to load a BMP image from a file
t_bmp24 *bmp24_loadImage(const char *filename);

// Function to save a BMP image to a file
void bmp24_saveImage(t_bmp24 *img, const char *filename);

// Function to apply a negative effect to the image
void bmp24_negative(t_bmp24 *img);

// Function to convert the image to grayscale
void bmp24_grayscale(t_bmp24 *img);

// Function to adjust the brightness of the image
void bmp24_brightness(t_bmp24 *img, int value);

// Function to apply a convolution filter to the image
void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize);

// Function to apply a box blur filter
void bmp24_boxBlur(t_bmp24 *img);

// Function to apply a Gaussian blur filter
void bmp24_gaussianBlur(t_bmp24 *img);

// Function to apply an outline filter
void bmp24_outline(t_bmp24 *img);

// Function to apply an emboss filter
void bmp24_emboss(t_bmp24 *img);

// Function to apply a sharpen filter
void bmp24_sharpen(t_bmp24 *img);

// Function to compute the histogram for the red channel
unsigned int *bmp24_computeHistogramR(const t_bmp24 *img);

// Function to compute the histogram for the green channel
unsigned int *bmp24_computeHistogramG(const t_bmp24 *img);

// Function to compute the histogram for the blue channel
unsigned int *bmp24_computeHistogramB(const t_bmp24 *img);

// Function to compute the lookup table for histogram equalization
void computeEqualizationLUT(unsigned int *hist, int totalPixels, uint8_t *lut);

// Function to apply histogram equalization to the image
void bmp24_equalize(t_bmp24 *img);

#endif // BMP24_H
