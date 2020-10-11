#include "common.h"

int main_template(void func(size_type, type*, type*, type*))
{
    size_type size, i, x;
    type *a, *b, dot_product = 0;
    clock_t begin, end;

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
    fprintf(stderr, "\n");

    begin = clock();
    func(size, a, b, &dot_product);
    end = clock();

    fprintf(stderr, "Dot product of vectors equals %lf, execution time is %lf seconds\n",
            dot_product, (double)(end - begin) / CLOCKS_PER_SEC);

    free(a);
    free(b);
    return 0;
}
