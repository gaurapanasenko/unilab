#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void ign_other(FILE * input) {
	char c = 0;
	while (c!='\n')
		c=fgetc(input);
}

char menu (FILE * input, FILE * output, const char * message, char count) {
	char answer = 0;
	while (answer < 1 || answer > count) {
		printf("%s\n",message);
		answer = getchar()-'0';
		ign_other(input);
		if (answer < 1 || answer > count) {
			system("clear");
			printf("Вводити потрібно тільки цифри від 1 до %hhi. Спробуйте ще\n", count);
		}
	}
	return answer;
}

int * create_array(int length) {
	int * a = new int[length];
	return a;
}

void fill_array(int * array,int length,int min,int max) {
	for (int i=0;i < length;i++) {
		array[i]=rand()%(max-min+1)+min;
	}
}

void show_array(FILE * output,int * array,int length) {
	if (length == 0) {
		fprintf(output,"Масив порожній\n");
		return;
	}
	fprintf(output,"Масив має такі значення:\n");
	for (int i=0;i < length;i++) {
		fprintf(output,"%i ",array[i]);
	}
	fprintf(output,"\n\n");
}

int find_max(int * array, int length) {
	int max=0,i;
	for(i=1;i<length;i++) {
		if(array[max]<array[i])
		max=i;
	}
	return max;
}

int * task1(int * array, int length,int * length_out, int max_pos) {
	if (max_pos>=length) {
		*length_out = 0;
		return new int [0];
	}
	int lengthb=0, *b = new int[length-max_pos],i;
	for (i=max_pos+1;i<length;i++) {
		if (array[i]%2==0 && array[i]>=0) {
			b[lengthb++] = array[i];
		}
	}
	*length_out = lengthb;
	return b;
}

int * task2(int * array, int * length) {
	int *b,l = *length/2,i;
	b = new int[l];
	for (i=0;i<l;i++)
		b[i]=array[i*2+1];
	delete array;
	*length = l;
	return b;
}

int main() {
	srand(time(0));
	int * a,length=0,max=0,intermin=0,intermax=0,answer=0;
	while (length < 1 || length > 1000) {
		printf("Введіть довжину масиву:\n");
		scanf("%i",&length);
		ign_other(stdin);
		if (length < 1 || length > 1000) {
			system("clear");
			printf("Ви ввели неправильну довжину, дозволено тільки ціле число від 1 до 1000\n");
		}
	}
	while (intermin >= intermax) {
		printf("Введіть інтервал дозволених значень в масиві:\n");
		scanf("%i%i",&intermin,&intermax);
		ign_other(stdin);
		if (intermin >= intermax) {
			system("clear");
			printf("Ви ввели неправильний інтервал, дозволено тільки цілі числа, при чому перше число повинно бути менше ніж друге\n");
		}
	}
	a=create_array(length);
	fill_array(a,length,intermin,intermax);
	show_array(stdout,a,length);
	do {
		answer = menu(stdin,stdout,"Виберіть дії:\n1. Сформувати новий масив з додатних парних елементів даного масиву, що розташовані після максимального елемента масиву.\n2. Видалити всі елементи з непарними індексами.\n3. Вихід",3);
		if (answer == 1) {
			max=find_max(a,length);
			printf("Максимальний елемент знаходиться на %i позиції.\n",max);
			if (max == length) printf("Максимальний елемент знаходиться у кінці масиву, тому вихідний масив порожній\n");
			else {
				int * b,lengthb;
				b=task1(a,length,&lengthb,max);
				show_array(stdout,b,lengthb);
				delete b;
			}
		} else if (answer == 2) {
			a=task2(a,&length);
			show_array(stdout,a,length);
		}
	} while (answer != 3);
	delete a;
	return 0;
}
