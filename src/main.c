#include <stdio.h>
#include <stdlib.h>
#include <mpfr.h>

#include "config.h"
#include "common.h"

#include "generator.h"

int main(int argc, char *argv[])
{
    Configurations configurations = {
        .precision = DEFAULT_PRECISION,
        .imageSize = DEFAULT_IMAGE_SIZE,
        .maxIterations = DEFAULT_MAX_ITERATIONS,
        .maxThreads = DEFAULT_MAX_THREADS,
        .colorMode = DEFAULT_COLOR_MODE,
        .imageName = NULL
    };

    if (argc > 1)
        configurations.precision = (mpfr_prec_t) atoi(argv[1]);

    mpfr_init2(configurations.xPosition, configurations.precision);
    mpfr_init2(configurations.yPosition, configurations.precision);
    mpfr_init2(configurations.zoom, configurations.precision);

    mpfr_set_flt(configurations.xPosition, DEFAULT_X_POSITION, MPFR_RNDN);
    mpfr_set_flt(configurations.yPosition, DEFAULT_Y_POSITION, MPFR_RNDN);
    mpfr_set_flt(configurations.zoom, DEFAULT_ZOOM, MPFR_RNDN);


    if (argc > 2)
        configurations.imageSize = atoi(argv[2]);

    if (argc > 3)
        mpfr_strtofr(configurations.xPosition, argv[3], NULL, 10, MPFR_RNDN);

    if (argc > 4)
        mpfr_strtofr(configurations.yPosition, argv[4], NULL, 10, MPFR_RNDN);

    if (argc > 5)
        mpfr_strtofr(configurations.zoom, argv[5], NULL, 10, MPFR_RNDN);

    if (argc > 6)
        configurations.maxIterations = atoi(argv[6]);

    if (argc > 7)
        configurations.colorMode = atoi(argv[7]);

    if (argc > 8)
        configurations.maxThreads = atoi(argv[8]);

    if (argc > 9)
        configurations.imageName = argv[9];

    printf("Using the following parameters to generate the Mandelbrot Fractal:\n");
    printf("Precision: %ld\n", configurations.precision);
    printf("Image size: %d\n", configurations.imageSize);
    printf("X position: %f\n", mpfr_get_flt(configurations.xPosition, MPFR_RNDN));
    printf("Y position: %f\n", mpfr_get_flt(configurations.yPosition, MPFR_RNDN));
    printf("Zoom: %f\n", mpfr_get_flt(configurations.zoom, MPFR_RNDN));
    printf("Max iterations: %d\n", configurations.maxIterations);
    printf("Color mode: %d\n", configurations.colorMode);
#if MULTITHREADING == 1
    printf("Max threads: %d\n", configurations.maxThreads);
#endif

    int result = generate(&configurations);
    if (result != 0)
    {
        printf("Failed to generate Mandelbrot fractal.\n");
        exit(EXIT_FAILURE);
    }

    mpfr_clear(configurations.xPosition);
    mpfr_clear(configurations.yPosition);
    mpfr_clear(configurations.zoom);

    mpfr_free_cache();
    mpfr_free_pool();
    exit(EXIT_SUCCESS);
}