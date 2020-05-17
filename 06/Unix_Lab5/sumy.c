#include "sumy.h"

int sumy_array(int* arr, int size) {
    int value = 0;
    int *i = arr, *last = arr + size;
    for (; i != last; ++i) {
        value += *i;
    }
    return value;
}
