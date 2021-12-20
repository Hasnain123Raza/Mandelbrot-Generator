#include "generator.h"

int generate(Configurations *configurations)
{
    mpfr_prec_t precision = configurations->precision;
    int imageSize = configurations->imageSize;

    MandelbrotData *data = malloc(sizeof(MandelbrotData) * imageSize * imageSize);
    if (data == NULL)
    {
        printf("Failed to allocate memory for data.\n");
        return 1;
    }
    for (int counter = 0; counter < imageSize * imageSize; counter++)
    {
        mpfr_init2(data[counter].xn, precision);
        mpfr_init2(data[counter].yn, precision);
    }

    unsigned char *image = malloc(sizeof(unsigned char) * imageSize * imageSize * 3);
    if (image == NULL)
    {
        printf("Failed to allocate memory for image.\n");
        free(data);
        return 1;
    }

    if (calculate(configurations, data) != 0)
    {
        free(data);
        free(image);
        return 1;
    }
    convert(configurations, data, image);
    for (int counter = 0; counter < imageSize * imageSize; counter++)
    {
        mpfr_clear(data[counter].xn);
        mpfr_clear(data[counter].yn);
    }
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