#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <mpfr.h>

#include "config.h"
#include "common.h"
#include "util.h"

#if MULTITHREADING == 1
#include <pthread.h>

typedef struct {
    float percentageComplete;
    float lastPercentageComplete;
    pthread_mutex_t mutex;
} ThreadProgress;

typedef struct {
    Configurations *configurations;
    MandelbrotData *data;
    int start;
    int end;
    ThreadProgress *progress;
} ThreadData;
#endif

int calculate(Configurations *configurations, MandelbrotData *data);

#endif