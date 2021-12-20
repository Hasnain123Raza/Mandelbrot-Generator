#ifndef COMMON_H
#define COMMON_H

#include <mpfr.h>

typedef enum {
    SIMPLE_GRAYSCALE,
    SMOOTH_GRAYSCALE
} ColorMode;

typedef struct {
    mpfr_prec_t precision;
    int imageSize;
    mpfr_t xPosition;
    mpfr_t yPosition;
    mpfr_t zoom;
    int maxIterations;
    ColorMode colorMode;
    int maxThreads;
    char *imageName;
} Configurations;

typedef struct {
    int iterations;
    mpfr_t xn;
    mpfr_t yn;
} MandelbrotData;

#endif