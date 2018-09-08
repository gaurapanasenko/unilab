#include <stdio.h>

int main(int argc, char **argv)
{
	long int n = 0, a = 1, b = 1, c = 0, i = 2;
	printf("N = ");
	scanf("%li",&n);
	if (n >= 0) {
		if (n == 0)
			printf("F(0) = 0");
		else if (n == 1)
			printf("F(1) = 1");
		else {
			while (i < n) {
				c = a + b;a = b;b = c;i++;
				printf("F(%li) = %li\n",i,b);
			}
			printf("F(%li) = %li\n",n,b);
		}
	} else printf("Неправиьный формат, ввод должен быть натуральным числом.");
	return 0;
}

