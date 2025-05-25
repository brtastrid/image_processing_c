#ifndef BMP8_H
#define BMP8_H

// Structure representing an 8-bit BMP image
typedef struct {
    unsigned char header[54];      // BMP file header
    unsigned char colorTable[1024]; // Color table for 8-bit images
    unsigned char *data;           // Pixel data
    unsigned int width;            // Width of the image
    unsigned int height;           // Height of the image
    unsigned short colorDepth;     // Color depth of the image
    unsigned int dataSize;         // Size of the pixel data
} t_bmp8;

// Function to load an 8-bit BMP image from a file
t_bmp8 *bmp8_loadImage(const char *filename);

// Function to save an 8-bit BMP image to a file
void bmp8_saveImage(const char *filename, t_bmp8 *img);

// Function to free memory allocated for an 8-bit BMP image
void bmp8_free(t_bmp8 *img);

// Function to print information about an 8-bit BMP image
void bmp8_printInfo(const t_bmp8 *img);

// Function to apply a negative effect to an 8-bit BMP image
void bmp8_negative(t_bmp8 *img);

// Function to adjust the brightness of an 8-bit BMP image
void bmp8_brightness(t_bmp8 *img, int value);

// Function to apply a threshold effect to an 8-bit BMP image
void bmp8_threshold(t_bmp8 *img, int threshold);

// Function to apply a convolution filter to an 8-bit BMP image
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);

// Function to apply a box blur filter to an 8-bit BMP image
void bmp8_boxBlur(t_bmp8 *img);

// Function to apply a Gaussian blur filter to an 8-bit BMP image
void bmp8_gaussianBlur(t_bmp8 *img);

// Function to apply an outline filter to an 8-bit BMP image
void bmp8_outline(t_bmp8 *img);

// Function to apply an emboss filter to an 8-bit BMP image
void bmp8_emboss(t_bmp8 *img);

// Function to apply a sharpen filter to an 8-bit BMP image
void bmp8_sharpen(t_bmp8 *img);

// Function to compute the histogram of an 8-bit BMP image
unsigned int *bmp8_computeHistogram(t_bmp8 *img);

// Function to compute the Cumulative Distribution Function (CDF) from a histogram
unsigned int *bmp8_computeCDF(unsigned int *hist);

// Function to apply histogram equalization to an 8-bit BMP image
void bmp8_equalize(t_bmp8 *img, unsigned int *cdf);

#endif // BMP8_H
