#include "convertor.h"

void convert(Configurations *configurations, float *data, unsigned char *image)
{
    int imageSize = configurations->imageSize;
    int maxIterations = configurations->maxIterations;

    double startTime = tick();
    for (int y = 0; y < imageSize; y++)
    {
        for (int x = 0; x < imageSize; x++)
        {
            float convergenceScore = data[y * imageSize + x];

            int pixelIndex = (y * imageSize + x) * 3;
            image[pixelIndex] = (unsigned char) (convergenceScore * 255.0f);
            image[pixelIndex + 1] = (unsigned char) (convergenceScore * 255.0f);
            image[pixelIndex + 2] = (unsigned char) (convergenceScore * 255.0f);
        }
    }
    double endTime = tick();
    printf("Time to convert Mandelbrot convergence set to image: %f\n", endTime - startTime);
}