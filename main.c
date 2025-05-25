#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp8.h"
#include "bmp24.h"

// Detects if bmp bit is 8 or 24 bit depth
int detectBitDepth(const char *filename) {
    FILE *f = fopen(filename, "rb"); // Opening file
    if (!f) return -1; // if file can't be opened returns error
    fseek(f, 28, SEEK_SET);
    uint16_t bits;
    fread(&bits, sizeof(uint16_t), 1, f);
    printf("DEBUG bits = %d\n", bits);// Reading BMP bit depth
    fclose(f); //closing file
    return (bits == 8 || bits == 24) ? bits : -1;
}

// Filter menu for 8 bit image
void applyFilters8(t_bmp8 *img) {
    int choice;
    while (1) { //loops until user choose
        // Menu of actions on image
        printf("\n Please select a filter. (Remember to save file & reload to see changes):\n");
        printf("1. Negative\n");
        printf("2. Brightness\n");
        printf("3. Threshold (Black and white)\n");
        printf("4. Box Blur\n");
        printf("5. Gaussian Blur\n");
        printf("6. Outline\n");
        printf("7. Emboss\n");
        printf("8. Sharpen\n");
        printf("9. Histogram \n");
        printf("10. Return to menu\n");
        printf(">>> Enter the number next to the filter you want");
        scanf("%d", &choice); // Read the user's choice
        getchar();

        switch (choice) {
            case 1: bmp8_negative(img); printf("Negative applied.\n"); break;
            case 2: {
                int value;
                printf("Brightness value (-255 to 255): "); //Scale of brightness
                scanf("%d", &value); getchar();  // Read the user's choice
                bmp8_brightness(img, value); // Apply filter
                printf("Brightness adjusted.\n");
                break;
            }
            case 3: {
                int threshold;
                printf("Threshold value (0 to 255): "); //level of treshold
                scanf("%d", &threshold); getchar();// Read the user's choice
                bmp8_threshold(img, threshold); // Apply filter
                printf("Threshold applied.\n");
                break;
            }
            case 4: bmp8_boxBlur(img); printf("Box Blur applied\n"); break; // Apply box blur
            case 5: bmp8_gaussianBlur(img); printf("Gaussian Blur applied\n"); break; // Apply gaussian blur
            case 6: bmp8_outline(img); printf("Outline filter applied\n"); break; // Apply outline
            case 7: bmp8_emboss(img); printf("Emboss filter applied\n"); break; // Apply emboss
            case 8: bmp8_sharpen(img); printf("Sharpen filter applied\n"); break; // Apply sharpen
            case 9: {
                unsigned int *hist = bmp8_computeHistogram(img);
                unsigned int *cdf = bmp8_computeCDF(hist);
                bmp8_equalize(img, cdf); // Make histogram
                free(hist); // reset
                free(cdf);
                printf("Histogram Equalization applied\n");
                break;
            }
            case 10: return; // exit menu
            default: printf("Invalid option\n");
        }
    }
}

// Same Filter menu for 24 bit images
void applyFilters24(t_bmp24 *img) {
    int choice;
    while (1) {
        printf("\n Please select a filter.\n");
        printf("1. Negative\n");
        printf("2. Grayscale\n");
        printf("3. Brightness\n");
        printf("4. Box Blur\n");
        printf("5. Gaussian Blur\n");
        printf("6. Outline\n");
        printf("7. Emboss\n");
        printf("8. Sharpen\n");
        printf("9. Histogram Equalization\n");
        printf("10. Return to  menu\n");
        printf("Enter the number next to the filter you want");
        scanf("%d", &choice); // Read user choice
        getchar();

        switch (choice) {
            case 1: bmp24_negative(img); printf("Negative applied.\n"); break; // Apply negative filter 
            case 2: bmp24_grayscale(img); printf("Grayscale applied.\n"); break; // Apply grayscale
            case 3: {
                int value;
                printf("Brightness value (-255 to 255): ");
                scanf("%d", &value); getchar(); // Read the brightness that the user want
                bmp24_brightness(img, value); // Apply brightness
                printf("Brightness applyied.\n");
                break;
            }
            case 4: bmp24_boxBlur(img); printf("Box Blur applied.\n"); break; // Apply box blur 
            case 5: bmp24_gaussianBlur(img); printf("Gaussian Blur applied.\n"); break; // Apply gaussian blur
            case 6: bmp24_outline(img); printf("Outline filter applied.\n"); break; // Apply outline
            case 7: bmp24_emboss(img); printf("Emboss filter applied.\n"); break; // Apply emboss
            case 8: bmp24_sharpen(img); printf("Sharpen filter applied.\n"); break; // Apply sharpen
            case 9: {
                bmp24_equalize(img);
                printf("Histogram Equalization applied.\n");
                break;
            }
            case 10: return;
            default: printf("Looks like there is a problem here... Please choose 1 to 10. \n");
        }
    }
}


// main
int main(void) {
    char filepath[256]; // file path buffer
    int choice ;
    int bits = -1; // byte depth storing
    // def pointers
    t_bmp8 *img8 = NULL;
    t_bmp24 *img24 = NULL;

    while (1) {
        // Menu of application on files 
        printf("\n Whaat are we doing now ? \n");
        printf("1. Open image\n");
        printf("2. Save image\n");
        printf("3. Apply  filter\n");
        printf("4. Image info\n");
        printf("5. Return\n");
        printf("Enter the number next to the action you want ");

        // Reading user inputs
        char input[10];
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Looks like there is a problem here... Please choose 1 to 5.\n");  // Control input issues
            continue;
        }

        switch (choice) {
            case 1: {
                // opening image
                printf("Enter file path: ");
                scanf("%255s", filepath); getchar();

                // detecting bmp depth
                bits = detectBitDepth(filepath);
                printf("Looks like there is a problem here : DEBUG detected bits = %d\n", bits);

                // reset previous loaded images
                if (img8) { bmp8_free(img8); img8 = NULL; }
                if (img24) { bmp24_free(img24); img24 = NULL; }

                // detects type of image and returns error if doesn't match
                if (bits == 8) {
                    img8 = bmp8_loadImage(filepath);
                    if (!img8) {
                        printf("Is is not an 8 bit image\n");
                        bits = -1;
                    } else {
                        printf("8 bit image loaded\n");
                    }
                } else if (bits == 24) {
                    img24 = bmp24_loadImage(filepath);
                    if (!img24) {
                        printf("It is not a 24 bit image \n");
                        bits = -1;
                    } else {
                        printf("24 bit image loaded\n");
                        // Debug first pixel
                        printf("DEBUG – First pixel R=%d G=%d B=%d\n",
                        img24->data[0][0].red,
                        img24->data[0][0].green,
                        img24->data[0][0].blue);
                    }
                } else {
                    printf("Looks like there is a problem here : wrong format. Please only send 8 or 24 bit images.\n");
                }
                break;
            }

            case 2: {
                // save current image
                if (bits == 8 && img8) {
                    printf("Please find a name for the outpout image : ");
                    scanf("%255s", filepath); getchar();
                    bmp8_saveImage(filepath, img8); // Apply save image
                } else if (bits == 24 && img24) {
                    printf("Please find a name for the outpout image :  ");
                    scanf("%255s", filepath); getchar();
                    bmp24_saveImage(img24, filepath); // Apply save image
                } else {
                    printf("We need to load an image first.\n");
                }
                break;
            }

            case 3:
                // apply filter based on image type
                if (bits == 8 && img8) applyFilters8(img8); // Apply filters on 8
                else if (bits == 24 && img24) applyFilters24(img24);
                else printf("We need to load an image first.\n"); // Apply filter on 24 
                break;

            case 4:
                // display image info
                if (bits == 8 && img8) bmp8_printInfo(img8);
                else if (bits == 24 && img24) {
                    printf("Image information:\n");
                    printf("Width       : %d px\n", img24->width); // Print width of the image
                    printf("Height      : %d px\n", img24->height); // Print height of the image
                    printf("Color depth : %d bits\n", img24->colorDepth); // Print color dpeth of the image
                } else printf("We need to load an image first.\n\n");
                break;

            case 5:
                // exit
                if (img8) bmp8_free(img8);
                if (img24) bmp24_free(img24);
                printf("See you ! BAERT Astrid & Taha EZZAHRAOUI\n");
                return 0;

            default:
                printf("Looks like there is a problem here... Please choose 1 to 5. \n");
        }
    }
}
