#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define size_type int
#define type double

const int SIZE_OF_SIZE = sizeof(size_type);
const int SIZE_OF_ITEM = sizeof(type);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "%s must have numeric argument, "
                        "that will represent number of threads\n",
                argv[0]);
        return 1;
    }
    size_type size, i, x;
    type *a, *b, dot_product = 0;
    clock_t begin, end;

    omp_set_num_threads(atoi(argv[1]));

    fread(&size, SIZE_OF_SIZE, 1, stdin);

    a = malloc(size * SIZE_OF_ITEM);
    b = malloc(size * SIZE_OF_ITEM);

    fread(a, SIZE_OF_ITEM, size, stdin);

    fread(&i, SIZE_OF_SIZE, 1, stdin);
    if (i != size) {
        fprintf(stderr, "Vectors has different sizes, first have %i, second %i\n",
                size, i);
        free(a);
        free(b);
        return 1;
    }

    fread(b, SIZE_OF_ITEM, size, stdin);

    x = (8 > size) ? size : 8;
    fprintf(stderr, "Size is %i, first %i elements of first vector:\n", size, x);
    for (i = 0; i < x; i++) {
        fprintf(stderr, "%lf ", a[i]);
    }
    fprintf(stderr, "\nFirst %i elements of second vector:\n", x);
    for (i = 0; i < x; i++) {
        fprintf(stderr, "%lf ", b[i]);
    }

    begin = clock();
#pragma omp parallel for reduction (+:dot_product)
    for (int j = 0; j < size; j++) {
        dot_product += a[j] * b[j];
    }
    end = clock();
    fprintf(stderr, "\nFirst 8 elements of second vector:\n");

    printf("Dot product of vectors equals %lf, execution time is %lf seconds\n",
           dot_product, (double)(end - begin) / CLOCKS_PER_SEC);

    free(a);
    free(b);
    return 0;
}
