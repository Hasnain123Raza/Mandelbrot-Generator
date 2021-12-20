#include "calculator.h"

#if MULTITHREADING == 1
static void multithreadedCalculate(ThreadData *threadDatum);
#endif
static MandelbrotData solve(mpfr_t cx, mpfr_t cy, int precision, int maxIterations);

int calculate(Configurations *configurations, MandelbrotData *data)
{
    mpfr_prec_t precision = configurations->precision;
    int imageSize = configurations->imageSize;
    int maxIterations = configurations->maxIterations;
    int maxThreads = configurations->maxThreads;
    float lastPercentComplete = 0.0f;

    double startTime = tick();
    if (maxThreads == 1)
    {
        mpfr_t temp;
        mpfr_init2(temp, precision);

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
                int index = y * imageSize + x;
                mpfr_t *cx = &data[index].xn;
                mpfr_t *cy = &data[index].yn;
                
                mpfr_div_ui(*cx, configurations->zoom, imageSize, MPFR_RNDN);
                mpfr_set_ui(temp, imageSize, MPFR_RNDN);
                mpfr_div_ui(temp, temp, 2, MPFR_RNDN);
                mpfr_neg(temp, temp, MPFR_RNDN);
                mpfr_add_ui(temp, temp, x, MPFR_RNDN);
                mpfr_mul(*cx, *cx, temp, MPFR_RNDN);
                mpfr_add(*cx, *cx, configurations->xPosition, MPFR_RNDN);

                mpfr_div_ui(*cy, configurations->zoom, imageSize, MPFR_RNDN);
                mpfr_set_ui(temp, imageSize, MPFR_RNDN);
                mpfr_div_ui(temp, temp, 2, MPFR_RNDN);
                mpfr_neg(temp, temp, MPFR_RNDN);
                mpfr_add_ui(temp, temp, imageSize - (y + 1), MPFR_RNDN);
                mpfr_mul(*cy, *cy, temp, MPFR_RNDN);
                mpfr_add(*cy, *cy, configurations->yPosition, MPFR_RNDN);

                MandelbrotData solution = solve(*cx, *cy, precision, maxIterations);

                data[index].iterations = solution.iterations;
                mpfr_swap(data[index].xn, solution.xn);
                mpfr_swap(data[index].yn, solution.yn);

                mpfr_clear(solution.xn);
                mpfr_clear(solution.yn);
            }
        }

        mpfr_clear(temp);
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
    mpfr_prec_t precision = configurations->precision;
    int imageSize = configurations->imageSize;
    int maxIterations = configurations->maxIterations;

    mpfr_t temp;
    mpfr_init2(temp, precision);

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
            int index = y * imageSize + x;
            mpfr_t *cx = &threadDatum->data[index].xn;
            mpfr_t *cy = &threadDatum->data[index].yn;
            
            mpfr_div_ui(*cx, configurations->zoom, imageSize, MPFR_RNDN);
            mpfr_set_ui(temp, imageSize, MPFR_RNDN);
            mpfr_div_ui(temp, temp, 2, MPFR_RNDN);
            mpfr_neg(temp, temp, MPFR_RNDN);
            mpfr_add_ui(temp, temp, x, MPFR_RNDN);
            mpfr_mul(*cx, *cx, temp, MPFR_RNDN);
            mpfr_add(*cx, *cx, configurations->xPosition, MPFR_RNDN);

            mpfr_div_ui(*cy, configurations->zoom, imageSize, MPFR_RNDN);
            mpfr_set_ui(temp, imageSize, MPFR_RNDN);
            mpfr_div_ui(temp, temp, 2, MPFR_RNDN);
            mpfr_neg(temp, temp, MPFR_RNDN);
            mpfr_add_ui(temp, temp, imageSize - (y + 1), MPFR_RNDN);
            mpfr_mul(*cy, *cy, temp, MPFR_RNDN);
            mpfr_add(*cy, *cy, configurations->yPosition, MPFR_RNDN);

            MandelbrotData solution = solve(*cx, *cy, precision, maxIterations);

            threadDatum->data[index].iterations = solution.iterations;
            mpfr_swap(threadDatum->data[index].xn, solution.xn);
            mpfr_swap(threadDatum->data[index].yn, solution.yn);

            mpfr_clear(solution.xn);
            mpfr_clear(solution.yn);
        }
    }

    mpfr_clear(temp);

    mpfr_free_cache();
    mpfr_free_pool();
}
#endif

static MandelbrotData solve(mpfr_t cx, mpfr_t cy, int precision, int maxIterations)
{
    MandelbrotData solution;
    mpfr_t temp1;
    mpfr_t temp2;

    mpfr_init2(solution.xn, precision);
    mpfr_init2(solution.yn, precision);
    mpfr_init2(temp1, precision);
    mpfr_init2(temp2, precision);
    mpfr_set_flt(solution.xn, 0.0f, MPFR_RNDN);
    mpfr_set_flt(solution.yn, 0.0f, MPFR_RNDN);

    int iterations = 0;
    for (iterations = 0; iterations < maxIterations; iterations++)
    {
        mpfr_sqr(temp1, solution.xn, MPFR_RNDN);
        mpfr_sqr(temp2, solution.yn, MPFR_RNDN);

        mpfr_mul(solution.yn, solution.yn, solution.xn, MPFR_RNDN);
        mpfr_mul_ui(solution.yn, solution.yn, 2, MPFR_RNDN);
        mpfr_add(solution.yn, solution.yn, cy, MPFR_RNDN);
    
        mpfr_sub(solution.xn, temp1, temp2, MPFR_RNDN);
        mpfr_add(solution.xn, solution.xn, cx, MPFR_RNDN);

        mpfr_add(temp1, temp1, temp2, MPFR_RNDN);
        if (mpfr_cmp_ui(temp1, 4) > 0)
            break;
    }

    mpfr_clear(temp1);
    mpfr_clear(temp2);

    solution.iterations = iterations;
    return solution;
}