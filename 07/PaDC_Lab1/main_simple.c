#include <math.h>
#include "common.h"

void func(size_type size, type *a, type *b, type *dot_product) {
    size_type i;
    type dp = 0;
    for (i = 0; i < size; i++)
        dp += a[i] * b[i];
    *dot_product = dp;
}

int main()
{
    return main_template(func);
}
