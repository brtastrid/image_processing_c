# BAERT Astrid & EZZAHRAOUI - BMP Image Filter
_____________________________________________________________
## Description : 
On this project you can load, display, apply filters and save BMP 8-bit and 24-bit color format. 
The filters available are : 
  → negative, 
  
  → blur (gaussian and box), 
  
  → emboss, 
  
  → sharpen 
  
  → histogram equalization.

  
→ You can also display image infos

→ You can also : 
  → Open image
  → Save it 
  → Apply filters 

## Bugs : 
We can have some issue if the image file is corrupted or if the file path is incorrect. Moreover, if we apply an 24-bit filter on a 8-bit image, we will have some issues. 
  
________________________________________________________________
## To use this project : 
Use `gcc` to compile the project:

```bash
gcc main.c bmp8.c bmp24.c -o bmp_filter
