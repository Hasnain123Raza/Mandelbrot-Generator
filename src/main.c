#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "common.h"

#include "generator.h"

int main(int argc, char *argv[])
{
    Configurations configurations = {
        .imageSize = DEFAULT_IMAGE_SIZE,
        .xPosition = DEFAULT_X_POSITION,
        .yPosition = DEFAULT_Y_POSITION,
        .zoom = DEFAULT_ZOOM,
        .maxIterations = DEFAULT_MAX_ITERATIONS,
        .imageName = NULL
    };

    if (argc > 1)
        configurations.imageSize = atoi(argv[1]);

    if (argc > 2)
        configurations.xPosition = atof(argv[2]);

    if (argc > 3)
        configurations.yPosition = atof(argv[3]);

    if (argc > 4)
        configurations.zoom = atof(argv[4]);

    if (argc > 5)
        configurations.maxIterations = atoi(argv[5]);

    if (argc > 6)
        configurations.imageName = argv[6];

    printf("Using the following parameters to generate the Mandelbrot Fractal:\n");
    printf("Image size: %d\n", configurations.imageSize);
    printf("X position: %f\n", configurations.xPosition);
    printf("Y position: %f\n", configurations.yPosition);
    printf("Zoom: %f\n", configurations.zoom);
    printf("Max iterations: %d\n", configurations.maxIterations);

    int result = generate(&configurations);
    if (result != 0)
    {
        printf("Failed to generate Mandelbrot fractal.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}