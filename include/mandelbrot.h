#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stdio.h>

#include "util.h"

void mandelbrot(
    int imageSize,
    float xPosition,
    float yPosition,
    float zoom,
    int maxIterations,
    unsigned char *imageData
);

#endif