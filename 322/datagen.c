#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void ign_other(FILE * input) {
	char c = 0;
	while (c!='\n')
		c=fgetc(input);
}

int main () {
	srand(time(0));
	int length=0,intermin=0,intermax=0;
	while (length < 1 || length == INT_MAX) {
		printf("Введіть довжину масиву:\n");
		scanf("%i",&length);
		ign_other(stdin);
		if (length < 1 || length == INT_MAX) {
			system("clear");
			printf("Вы ввели неправильну довжину, дозволено тільки ціле число від 1 до %i\n", INT_MAX - 1);
		}
	}
	while (intermin >= intermax) {
		printf("Введіть інтервал дозволених значень в масиві:\n");
		scanf("%i%i",&intermin,&intermax);
		ign_other(stdin);
		if (intermin >= intermax) {
			system("clear");
			printf("Вы ввели неправильний інтервал, дозволено тільки цілі числа, при чому перше число повинно бути менше ніж друге\n");
		}
	}
	FILE * out = fopen("data.txt","w");
	for (int i=0;i<length;i++) {
		fprintf(out,"%i ",rand()%(intermax-intermin+1)+intermin);
	}
	fclose(out);
}
