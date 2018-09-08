#include <stdio.h>

int main(int argc, char **argv)
{
	printf("Введите последовательность целых чисел. Ноль - признак конца.\n");
	int x = 0, tmp = 0, i = 0;
	while (1) {
		printf("Введите число= ");
		scanf("%i",&tmp);
		if (tmp == 0)
			break;
		else if (tmp % 2 == 1) {
			x++;
		}
		i++;
	}
	printf("Встретилось %i нечетных числа.\n",x);
	return 0;
}

