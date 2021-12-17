#include "calculator.h"

static int solve(float cx, float cy, int iterations);

void calculate(Configurations *configurations, float *data)
{
    int imageSize = configurations->imageSize;
    float xPosition = configurations->xPosition;
    float yPosition = configurations->yPosition;
    float zoom = configurations->zoom;
    int maxIterations = configurations->maxIterations;

    double startTime = tick();
    for (int y = 0; y < imageSize; y++)
    {
        for (int x = 0; x < imageSize; x++)
        {
            float cx = xPosition + ((float) x - (float) imageSize / 2.0f) * zoom / (float) imageSize;
            float cy = yPosition + ((float) (imageSize - (y + 1)) - (float) imageSize / 2.0f) * zoom / (float) imageSize;

            float convergenceScore = (float) solve(cx, cy, maxIterations) / (float) maxIterations;

            int index = y * imageSize + x;
            data[index] = convergenceScore;
        }
    }
    double endTime = tick();
    printf("Time to generate Mandelbrot convergence set: %f\n", endTime - startTime);
}

static int solve(float cx, float cy, int iterations)
{
    float x = 0.0f;
    float y = 0.0f;
    int i = 0;
    for (i = 0; i < iterations; i++)
    {
        float x2 = x * x;
        float y2 = y * y;
        if (x2 + y2 > 4.0f)
            break;
        y = 2.0f * x * y + cy;
        x = x2 - y2 + cx;
    }
    return i;
}