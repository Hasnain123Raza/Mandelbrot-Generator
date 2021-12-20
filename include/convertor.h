#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <math.h>
#include <stdio.h>
#include <mpfr.h>

#include "common.h"
#include "util.h"

void convert(Configurations *configurations, MandelbrotData *data, unsigned char *image);

#endif