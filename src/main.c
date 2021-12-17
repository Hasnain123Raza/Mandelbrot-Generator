#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "config.h"
#include "mandelbrot.h"

int main(int argc, char *argv[])
{
    int imageSize = DEFAULT_IMAGE_SIZE;
    float xPosition = DEFAULT_X_POSITION;
    float yPosition = DEFAULT_Y_POSITION;
    float zoom = DEFAULT_ZOOM;
    int iterations = DEFAULT_MAX_ITERATIONS;
    int useDefaultOutput = 1;

    if (argc > 1)
        imageSize = atoi(argv[1]);

    if (argc > 2)
        xPosition = atof(argv[2]);

    if (argc > 3)
        yPosition = atof(argv[3]);

    if (argc > 4)
        zoom = atof(argv[4]);

    if (argc > 5)
        iterations = atoi(argv[5]);

    if (argc > 6)
        useDefaultOutput = 0;

    printf("Using the following parameters to generate the Mandelbrot Fractal:\n");
    printf("Image size: %d\n", imageSize);
    printf("X position: %f\n", xPosition);
    printf("Y position: %f\n", yPosition);
    printf("Zoom: %f\n", zoom);
    printf("Max iterations: %d\n", iterations);

    unsigned char *imageData = (unsigned char *) malloc(imageSize * imageSize * 3);
    if (!imageData)
    {
        printf("Failed to allocate memory for image data\n");
        exit(EXIT_FAILURE);
    }

    mandelbrot(imageSize, xPosition, yPosition, zoom, iterations, imageData);
    double startTime = tick();
    int result = stbi_write_jpg(useDefaultOutput ? DEFAULT_IMAGE_OUTPUT : argv[6], imageSize, imageSize, 3, imageData, 100);
    double endTime = tick();
    if (result == 0)
    {
        free(imageData);
        printf("Failed to write image data to file\n");
        exit(EXIT_FAILURE);
    }
    printf("Time to write Mandelbrot image data to file: %f\n", endTime - startTime);
    
    free(imageData);
    exit(EXIT_SUCCESS);
}