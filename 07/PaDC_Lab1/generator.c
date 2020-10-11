#include "common.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "%s must have numeric argument, "
                        "that will represent size of output data is MiB\n",
                argv[0]);
        return 1;
    }
    srand(time(NULL));

    // First int value will be size of array
    size_type size = (atoi(argv[1]) * UNIT - SIZE_OF_SIZE) / SIZE_OF_ITEM, i;
    type a;

    fprintf(stderr, "Size is %i, size of item is %i bytes, "
                    "size of size is %i bytes\nFirst 8 elements:\n",
            size, SIZE_OF_ITEM, SIZE_OF_SIZE);

    fwrite(&size, SIZE_OF_SIZE, 1, stdout);
    for (i = 0; i < size; i++) {
        a = ((type)rand()) / DIVIDER;
        if (i < 8)
            fprintf(stderr, "%lf ", a);
        fwrite(&a, SIZE_OF_ITEM, 1, stdout);
    }
    fprintf(stderr, "\n");

    return 0;
}
