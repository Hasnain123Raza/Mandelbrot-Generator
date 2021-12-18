#include "calculator.h"

#if MULTITHREADING == 1
static void multithreadedCalculate(ThreadData *threadDatum);
#endif
static MandelbrotData solve(float cx, float cy, int iterations);

int calculate(Configurations *configurations, MandelbrotData *data)
{
    int imageSize = configurations->imageSize;
    float xPosition = configurations->xPosition;
    float yPosition = configurations->yPosition;
    float zoom = configurations->zoom;
    int maxIterations = configurations->maxIterations;
    int maxThreads = configurations->maxThreads;
    float lastPercentComplete = 0.0f;

    double startTime = tick();
    if (maxThreads == 1)
    {
        for (int y = 0; y < imageSize; y++)
        {
            float percentComplete = (float) y / (float) imageSize;
            if (percentComplete - lastPercentComplete >= 0.1f)
            {
                printf("%.f%% complete\n", percentComplete * 100.0f);
                lastPercentComplete = percentComplete;
            }

            for (int x = 0; x < imageSize; x++)
            {
                float cx = xPosition + ((float) x - (float) imageSize / 2.0f) * zoom / (float) imageSize;
                float cy = yPosition + ((float) (imageSize - (y + 1)) - (float) imageSize / 2.0f) * zoom / (float) imageSize;

                MandelbrotData solution = solve(cx, cy, maxIterations);

                int index = y * imageSize + x;
                data[index].iterations = solution.iterations;
                data[index].xn = solution.xn;
                data[index].yn = solution.yn;
            }
        }

        printf("100%% complete\n");
    }
#if MULTITHREADING == 1
    else
    {
        int actualThreads = maxThreads > imageSize ? imageSize : maxThreads;
        int threadSize = (imageSize + actualThreads - 1) / actualThreads;
        ThreadProgress threadProgress = {0.0f, 0.0f};

        pthread_t *threads = malloc(sizeof(pthread_t) * actualThreads);
        if (!threads)
        {
            printf("Failed to allocate memory for threads\n");
            return 1;
        }

        ThreadData *threadData = malloc(sizeof(ThreadData) * actualThreads);
        if (!threadData)
        {
            printf("Failed to allocate memory for thread data\n");
            free(threads);
            return 1;
        }

        for (int index = 0; index < actualThreads; index++)
        {
            int start = index * threadSize;
            int end = start + threadSize;
            if (end > imageSize)
                end = imageSize;
            if (index == actualThreads - 1)
                end = imageSize;

            ThreadData *threadDatum = threadData + index;
            threadDatum->configurations = configurations;
            threadDatum->data = data;
            threadDatum->start = start;
            threadDatum->end = end;
            threadDatum->progress = &threadProgress;

            if (pthread_create(threads + index, NULL, (void *(*)(void *))multithreadedCalculate, threadDatum) != 0)
            {
                for (int counter = 0; counter < index; counter++)
                    pthread_cancel(threads[counter]);
                free(threads);
                free(threadData);
                printf("Failed to create thread\n");
                return 1;
            }

            if (end == imageSize)
            {
                actualThreads = index + 1;
                break;
            }
        }

        for (int index = 0; index < actualThreads; index++)
            pthread_join(threads[index], NULL);

        printf("100%% complete\n");
        printf("Actual threads: %d\n", actualThreads);
        printf("Thread size: %d\n", threadSize);

        free(threads);
        free(threadData);
    }
#endif
    double endTime = tick();
    printf("Time to generate Mandelbrot convergence set: %f\n", endTime - startTime);

    return 0;
}

#if MULTITHREADING == 1
static void multithreadedCalculate(ThreadData *threadDatum)
{
    Configurations *configurations = threadDatum->configurations;
    int imageSize = configurations->imageSize;
    float xPosition = configurations->xPosition;
    float yPosition = configurations->yPosition;
    float zoom = configurations->zoom;
    int maxIterations = configurations->maxIterations;

    for (int y = threadDatum->start; y < threadDatum->end; y++)
    {
        float percentComplete = 1.0f / (float) imageSize;
        pthread_mutex_lock(&threadDatum->progress->mutex);
        threadDatum->progress->percentageComplete += percentComplete;
        if (threadDatum->progress->percentageComplete - threadDatum->progress->lastPercentageComplete >= 0.1f)
        {
            printf("%.f%% complete\n", threadDatum->progress->percentageComplete * 100.0f);
            threadDatum->progress->lastPercentageComplete = threadDatum->progress->percentageComplete;
        }
        pthread_mutex_unlock(&threadDatum->progress->mutex);

        for (int x = 0; x < imageSize; x++)
        {
            float cx = xPosition + ((float) x - (float) imageSize / 2.0f) * zoom / (float) imageSize;
            float cy = yPosition + ((float) (imageSize - (y + 1)) - (float) imageSize / 2.0f) * zoom / (float) imageSize;

            MandelbrotData solution = solve(cx, cy, maxIterations);

            int index = y * imageSize + x;
            if (index >= imageSize * imageSize)
            {
                printf("%d, %d\n", threadDatum->end, imageSize);
            }
            threadDatum->data[index].iterations = solution.iterations;
            threadDatum->data[index].xn = solution.xn;
            threadDatum->data[index].yn = solution.yn;
        }
    }
}
#endif

static MandelbrotData solve(float cx, float cy, int iterations)
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

    return (MandelbrotData) {i, x, y};
}