#include <stdio.h>

int main(int argc, char **argv)
{

	printf("Введите последовательность целых чисел. Ноль - признак конца.\n");
	int x = 0, tmp = 0, i = 0;
	while (1) {
		printf("a[%i]=",(i + 1));
		scanf("%i",&tmp);
		if (tmp == 0)
			break;
		else if (tmp > x) {
			x = tmp;
		}
		i++;
	}
	printf("MAX=%i\n",x);
	return 0;
}

