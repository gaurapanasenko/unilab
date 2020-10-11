#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define size_type int
#define type double

static const int SIZE_OF_SIZE = sizeof(size_type);
static const int SIZE_OF_ITEM = sizeof(type);

static const int UNIT = 1024 * 1024; // MiB
static const int MAX_VALUE = 16;
static const int DIVIDER = RAND_MAX / MAX_VALUE;

int main_template(void func(size_type, type*, type*, type*));

#endif // COMMON_H
