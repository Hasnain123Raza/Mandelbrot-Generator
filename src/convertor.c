#include "convertor.h"

static float simpleGrayscaleColor(int maxIterations, MandelbrotData datum);
static float smoothGrayscaleColor(int maxIterations, MandelbrotData datum);

void convert(Configurations *configurations, MandelbrotData *data, unsigned char *image)
{
    int imageSize = configurations->imageSize;
    int maxIterations = configurations->maxIterations;

    double startTime = tick();
    for (int y = 0; y < imageSize; y++)
    {
        for (int x = 0; x < imageSize; x++)
        {
            MandelbrotData datum = data[y * imageSize + x];
            float color = 0.0f;
            switch (configurations->colorMode)
            {
                case SIMPLE_GRAYSCALE:
                    color = simpleGrayscaleColor(maxIterations, datum);
                    break;

                case SMOOTH_GRAYSCALE:
                    color = smoothGrayscaleColor(maxIterations, datum);
                    break;

                default:
                    color = simpleGrayscaleColor(maxIterations, datum);
                    break;
            }
            
            int pixelIndex = (y * imageSize + x) * 3;
            image[pixelIndex] = (unsigned char) (color * 255.0f);
            image[pixelIndex + 1] = (unsigned char) (color * 255.0f);
            image[pixelIndex + 2] = (unsigned char) (color * 255.0f);
        }
    }
    double endTime = tick();
    printf("Time to convert Mandelbrot convergence set to image: %f\n", endTime - startTime);
}

static float simpleGrayscaleColor(int maxIterations, MandelbrotData datum)
{
    return (float) datum.iterations / (float) maxIterations;
}

static float smoothGrayscaleColor(int maxIterations, MandelbrotData datum)
{
    float color = (float) datum.iterations;
    if (datum.iterations < maxIterations)
        color = ((float) datum.iterations + 1.0f) - log(log(datum.xn * datum.xn + datum.yn * datum.yn));
    return color / (float) maxIterations;
}