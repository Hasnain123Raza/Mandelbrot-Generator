#include "mandelbrot.h"

static int solve(float cx, float cy, int iterations);

void mandelbrot(
    int imageSize,
    float xPosition,
    float yPosition,
    float zoom,
    int maxIterations,
    unsigned char *imageData
)
{
    double startTime = tick();
    for (int y = 0; y < imageSize; y++)
    {
        for (int x = 0; x < imageSize; x++)
        {
            float cx = xPosition + ((float) x - (float) imageSize / 2.0f) * zoom / (float) imageSize;
            float cy = yPosition + ((float) (imageSize - (y + 1)) - (float) imageSize / 2.0f) * zoom / (float) imageSize;

            float color = (float) solve(cx, cy, maxIterations) / (float) maxIterations;

            int index = (y * imageSize + x) * 3;
            imageData[index] = (unsigned char) (color * 255.0f);
            imageData[index + 1] = (unsigned char) (color * 255.0f);
            imageData[index + 2] = (unsigned char) (color * 255.0f);
        }
    }
    double endTime = tick();
    printf("Time to generate Mandelbrot fractal: %f\n", endTime - startTime);
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