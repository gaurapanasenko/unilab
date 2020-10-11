#include <omp.h>
#include <math.h>
#include "common.h"

void func(size_type size, type *a, type *b, type *dot_product) {
    size_type i;
    type dp = 0;
#pragma omp parallel for shared(a, b, size) private(i) reduction(+:dp)
    for (i = 0; i < size; i++)
        dp += a[i] * b[i];
    *dot_product = dp;
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "%s must have numeric argument, "
                        "that will represent number of threads\n",
                argv[0]);
        return 1;
    }
    omp_set_dynamic(0);
    omp_set_num_threads(atoi(argv[1]));
    return main_template(func);
}
