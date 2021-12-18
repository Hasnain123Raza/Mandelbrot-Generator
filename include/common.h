#ifndef COMMON_H
#define COMMON_H

typedef enum {
    SIMPLE_GRAYSCALE,
    SMOOTH_GRAYSCALE
} ColorMode;

typedef struct {
    int imageSize;
    float xPosition;
    float yPosition;
    float zoom;
    int maxIterations;
    ColorMode colorMode;
    int maxThreads;
    char *imageName;
} Configurations;

typedef struct {
    int iterations;
    float xn;
    float yn;
} MandelbrotData;

#endif