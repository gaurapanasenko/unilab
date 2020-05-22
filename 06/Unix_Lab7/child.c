#include <stdio.h>

int main() {
    int number = 0;
    scanf("%i", &number);
    fprintf(stderr, "%i\n", 2 * number);
    return 0;
}
