#include "functions.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char * argv[]) {
	if (argc < 4) printf("Слишком мало аргументов\n");
	if (argc > 4) printf("Слишком много аргументов\n");
	if (argc != 4) return 1;
	int n, bn, cn, min, max, *a = NULL, *b, *c, tmp;
	n = atoi(argv[1]); min = atoi(argv[2]); max = atoi(argv[3]);
	if (n <= 0) {
		printf
			("Размер массива не может быть нулевым или отрицательным");
		return 1;
	}
	if (min > max) {
		tmp = min; max = min; min = tmp;
	}
	int answer = -1;
	while (answer) {
		answer = menu(stdin, stdout, "Выберете функцию:\n\
1. Сложить массив в одно число.\n\
2. Умножить какое-то число на каждый элемент массива.\n\
3. Разделить массив на два массива, где в первом положительные, \n\
   а во втором отрицательные.\n\
4. Соединить все массивы в один сложением.\n\
0. Выход\n\
>>> ", 5);
		//~ system("clear");
		switch(answer) {
			case 1:
				a = generate_array(n, min, max);
				if (a == NULL) {
					printf("Не удалось выделить память\n");
					break;
				}
				printf("Входящий массив:\n");
				print_array(stdin, a, n);
				printf("Сумма массива: %i\n", summarize(a, n));
				free(a);
				break;
			case 2:
				tmp = rnd(min, max);
				a = generate_array(n, min, max);
				if (a == NULL) {printf("Не удалось выделить память\n");
					break;
				}
				printf("Входящее цисло: %i\n", tmp);
				printf("Входящий массив:\n");
				print_array(stdin, a, n);
				multiply_elem(tmp, a, n);
				printf("Выходящий массив:\n");
				print_array(stdin, a, n);
				free(a);
				break;
			case 3:
				a = generate_array(n, min, max);
				b = malloc(n * sizeof(int));
				c = malloc(n * sizeof(int));
				if (a == NULL || b == NULL || c == NULL) {
					printf("Не удалось выделить память\n");
					free(a);free(b);free(c);break;
				}
				printf("Входящий массив:\n");
				print_array(stdin, a, n);
				split(a, n, b, &bn, c, &cn);
				printf("Массив массив отрицательных чисел:\n");
				print_array(stdin, b, bn);
				printf("Массив массив положительных чисел:\n");
				print_array(stdin, c, cn);
				if (!a) free(a);
				if (!b) free(a);
				break;
			case 4:
				a = generate_array(n, min, max);
				b = generate_array(n, min, max);
				if (a == NULL || b == NULL) {
					printf("Не удалось выделить память\n");
					free(a);free(b);break;
				}
				printf("Входящий первый массив:\n");
				print_array(stdin, a, n);
				printf("Входящий второй массив:\n");
				print_array(stdin, b, n);
				c = summarize_arrays(a, b, n);
				if (c == NULL) {
					printf("Не удалось выделить память\n");
					break;
				}
				printf("Выходящий массив:\n");
				print_array(stdin, c, n);
				free(a);free(b);free(c);
				break;
		};
		if (answer) ign_other(stdin);
		//~ system("clear");
	}
}
