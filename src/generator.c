#include "generator.h"

int generate(Configurations *configurations)
{
    int imageSize = configurations->imageSize;
    float xPosition = configurations->xPosition;
    float yPosition = configurations->yPosition;
    float zoom = configurations->zoom;
    int maxIterations = configurations->maxIterations;

    float *data = malloc(sizeof(float) * imageSize * imageSize);
    if (data == NULL)
    {
        printf("Failed to allocate memory for data.\n");
        return 1;
    }

    unsigned char *image = malloc(sizeof(unsigned char) * imageSize * imageSize * 3);
    if (image == NULL)
    {
        printf("Failed to allocate memory for image.\n");
        free(data);
        return 1;
    }

    calculate(configurations, data);
    convert(configurations, data, image);
    free(data);

    double startTime = tick();
    int result = stbi_write_jpg(
        configurations->imageName != NULL ? configurations->imageName : DEFAULT_IMAGE_OUTPUT, 
        imageSize, imageSize, 3, image, 100);
    double endTime = tick();

    if (result == 0)
    {
        free(image);
        printf("Failed to write image.\n");
        return 1;
    }

    printf("Time to write image to file: %f\n", endTime - startTime);

    return 0;
}