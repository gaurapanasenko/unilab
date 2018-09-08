#include <stdio.h>

int main(int argc, char **argv)
{
	int a = 0;
	while (1) {
		printf("Работает информационная система СШ23\n");
		printf("1. Новости дня\n");
		printf("2. Анекдот недели\n");
		printf("3. Автора!!!\n");
		printf("0. Выход\n");
		printf("Введите соответсвующий номер и нажмите ENTER.\n");
		scanf("%i",&a);
		if (a < 0 || a > 3)
			printf("Попробуйте ещё");
		else if (a == 0)
			break;
		else if (a == 1) {
			printf("Новостей нет\n");
		}
		else if (a == 2) {
			printf("Анекдотов нет\n");
		}
		else if (a == 3) {
			printf("Автор неизвестен\n");
		}
	}
	return 0;
}

