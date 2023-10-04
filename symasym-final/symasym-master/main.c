#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int size = 32;
  int thickness = 3;
  int symmetry = 0;
  int direction = 0;
  int nImages = 1;
  int seed = (int) time(NULL);
  const char *filename = "output";
  
  // Process command line arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-a") == 0) {
      symmetry = 1;  // // Asymmetric line
    } else if (strcmp(argv[i], "-h") == 0) {
      direction = 1;  // Horizontal line
    } else if (strcmp(argv[i], "-s") == 0) {
      symmetry = 2;  // Symmetric line
    } else if (strcmp(argv[i], "-v") == 0) {
      direction = 2;  // Vertical line
    } else if (strcmp(argv[i], "-seed") == 0 && i + 1 < argc) {
      seed = atoi(argv[i + 1]);  // Getting a seed
      i++;
    } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      filename = argv[i + 1];  // Getting a filename
      i++;
    } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
      nImages = atoi(argv[i + 1]);  // Getting a number of images
      i++;
    } else if (strcmp(argv[i], "-size") == 0 && i + 1 < argc) {
      size = atoi(argv[i + 1]);  // Getting a size
      i++;
    } else if(strcmp(argv[i], "-t") == 0 && i + 1 < argc){
      thickness = atoi(argv[i + 1]);  // Getting a thickness
      i++;
    }
  }
  
  // Generate points
  for (int i = 0; i < nImages; i++) {
    srand(seed);
    Direction finalDir;
    Symmetry finalSym;
    switch (seed % 4) {
      case 0:
        finalSym = A;
        finalDir = H;
        break;
      case 1:
        finalSym = S;
        finalDir = H;
        break;
      case 2:
        finalSym = A;
        finalDir = V;
        break;
      case 3:
        finalSym = S;
        finalDir = V;
        break;
    }
    
    if(direction != 0) finalDir = direction == 1 ? H : V;
    char dirLetter = finalDir == H ? 'h' : 'v';
    
    if(symmetry != 0) finalSym = symmetry == 1 ? A : S;
    char symString[5];
    if(finalSym == A) strcpy(symString, "asym");
    else if(finalSym == S) strcpy(symString, "symm");
    
    char finalFilename[200];
    sprintf(finalFilename, "%s-%c-%s-%d.bmp", filename, dirLetter, symString, seed);
    
    int *points = generatePoints(size, finalSym, thickness);
    generateImage(size, thickness, finalDir, finalFilename, points);
    free(points);
    seed = rand();
  }
  
  return 0;
}

// Restore the original packing alignment
// This directive pops the most recently pushed packing alignment value
// It ensures that the packing alignment is restored to its previous state
// after specifying a custom packing alignment with #pragma pack(push, ...)
// This helps maintain the proper memory layout and size of subsequent structs or data types
#pragma pack(push, 1)

typedef struct {
  uint16_t type;           // File signature ('BM' for BMP)
  uint32_t fileSize;       // File size in bytes
  uint32_t reserved;       // Reserved fields (always 0)
  uint32_t dataOffset;     // Offset of pixel data from the beginning of the file
  uint32_t headerSize;     // Size of the header (always 40 bytes)
  uint32_t width;          // Image width in pixels
  uint32_t height;         // Image height in pixels
  uint16_t planes;         // Number of planes (always 1)
  uint16_t bitsPerPixel;   // Color depth (number of bits per pixel)
  uint32_t compression;    // Compression method (0 - uncompressed)
  uint32_t dataSize;       // Size of the image data (width x height x color depth in bytes)
  uint32_t hResolution;    // Horizontal resolution (pixels per meter)
  uint32_t vResolution;    // Vertical resolution (pixels per meter)
  uint32_t colors;         // Number of colors in the palette (0 - no palette)
  uint32_t importantColors;// Number of important colors (0 - all colors are important)
} BMPHeader;
#pragma pack(pop)



typedef enum {
  V = 0,
  H = 1
} Direction;

typedef enum {
  S = 0,
  A = 1
} Symmetry;



void generateImage(int size, int thickness, Direction direction, const char *filename, int *points) {
  int width = size;
  int height = size;
  int bytesPerPixel = 3; // 24 bits (8 bits per RGB channel)
  int dataSize = width * height * bytesPerPixel;
  
  // Allocate memory for image data
  unsigned char *imageData = (unsigned char *) malloc(dataSize);
  if (imageData == NULL) {
    printf("Memory Allocation FAIL!.\n");
    return;
  }
  
  // Fill the image with white color
  memset(imageData, 255, dataSize);
  // Draw a polyline
  int pointCount = size;
  for (int i = 0; i < pointCount; i++) {
    for (int j = 0; j < thickness; j++) {
      int x, y;
      if(direction == H){
        x = i, y = points[i] + j;
        if(y >= size) y = size - 1;
      } else {
        x = points[i] + j, y = i;
        if(x >= size) x = size - 1;
      }
      int pixelIndex = y * width * bytesPerPixel + x * bytesPerPixel;
      imageData[pixelIndex] = 0;        // Red channel
      imageData[pixelIndex + 1] = 0;    // Green channel
      imageData[pixelIndex + 2] = 0;    // Blue channel
    }
  }
  
   // Create BMP header
  BMPHeader header;
  memset(&header, 0, sizeof(BMPHeader));
  header.type = 0x4D42;
  header.fileSize = sizeof(BMPHeader) + dataSize;
  header.dataOffset = sizeof(BMPHeader);
  header.headerSize = 40;
  header.width = width;
  header.height = height;
  header.planes = 1;
  header.bitsPerPixel = 24;
  header.compression = 0;
  header.dataSize = dataSize;
  header.hResolution = 2835;  // 72 dpi
  header.vResolution = 2835;  // 72 dpi
  
  // Write the image to a file
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    printf("Failed to open the file for writing.\n");
    free(imageData);
    return;
  }

  // Write the header
  fwrite(&header, sizeof(BMPHeader), 1, file);

  // Write the image data
  fwrite(imageData, dataSize, 1, file);

  // Close the file
  fclose(file);

  // Free memory
  free(imageData);
}

void fillImage(unsigned char *imageData, int width, int height, int stride, int bytesPerPixel, int startY, int thickness) {
  // Fill pixels with black color
  for (int i = 0; i < thickness; i++) {
    for (int j = 0; j < width; j++) {
      int pixelIndex = startY * stride + j * bytesPerPixel;
      imageData[pixelIndex] = 0;        // Red channel
      imageData[pixelIndex + 1] = 0;    // Green channel
      imageData[pixelIndex + 2] = 0;    // Blue channel
    }
    startY++;
  }
}

int *generatePoints(int size, Symmetry symmetry, int thickness){
  int *points = (int *) malloc(size * 2 * sizeof(int));
  if (points == NULL) {
    printf("Memory Allocation Fail\n");
    return NULL;
  }
  int start = rand() % size;
  points[0] = start;
  for(int i = 1; i < size; i++){
    int prev = points[i - 1];
    int next = prev - thickness + rand() % (2 * thickness + 1);
    if (next < 0) {
      next = 0;
    } else if (next >= size) {
      next = size - 1;
    }
    points[i] = next;
  }
  if(symmetry == S){
    for(int i = 0; i < size/2; i++){
      points[size - i - 1] = points[i];
    }
  }
  return points;
}

