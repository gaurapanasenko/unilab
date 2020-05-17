#include <stdio.h>
#include <stdlib.h>
#include "sumy.h"

int main(int argc, char **argv) {
    int arr[argc];
    for (int i = 0; i < argc; i++) {
        arr[i] = atoi(argv[i]);
    }
    printf("Summary: %i\n", sumy_array(arr, argc));
    return 0;
}
