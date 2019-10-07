#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#define CHUNK 128

/* Copy string from source (second argument) to destanation (first argument) */
char *stringcopy(char *d, const char *s) {
	// Saving begining of destanation string
	char *saved = d;
	// While current character is not zero copy each
	// character from source to destanation string
	while (*s)
		// Same as *d = *s;d++;s++;
		// Copy character from s to d
		// then increase d and s
		*d++ = *s++;
	*d = 0; // Make end of string by zero
	return saved; // Returning destanation string
}

/* Move block of memory from source (second argument)
 * to destanation (first argument) with size of third argument */
void *memorymove(void *dest, const void *src, long c) {
	// Create dynamic pointers
	char * d = (char*) dest;
	const char * s = (const char*) src;
	// Check that memory block not overlaps
	// and if they will overlap copy blocks in reverse direction
	if (d - s <= c) {
		// While count is not zero decrease counter and
		// copy character from source to destanation block
		while (c--)
			*d++ = *s++;
		return dest;
	} else {
		// Add to each pointers count number
		d+=c;s+=c;
		// While count is not zero decrease counter and
		// copy character from source to destanation block
		while (c--)
			*d-- = *s--;
		return dest;
	}
}

void ign_other(FILE * input) {
	char c = 0;
	while ((c = fgetc(input)) != EOF && c != '\n') {}
}

char * get_string (FILE * input, char * status, int * len) {
	char * a = (char *) calloc(CHUNK,sizeof(char)), ch = EOF;
	int length = 0;
	while (ch) {
		ch = fgetc(input);
		if (length >= INT_MAX - 2 * CHUNK || ch == EOF || ch == '\n') {
			if (ch == EOF)
				*status = 2;
			else if (ch == '\n')
				*status = 3;
			else if (length >= INT_MAX - 2 * CHUNK)
				*status = 1;
			ch = 0;
		}
		if (length % CHUNK == CHUNK - 1)
			a = (char *) realloc (a, (length + CHUNK + 1) * sizeof(char));
		a[length++] = ch;
	}
	*len = length;
	return a;
}

int main() {
	// Debbuging clock
	clock_t start, end;
	double cpu_time_used1,cpu_time_used2;
	char answer = 0, *a, status = 0;
	int length = 0;
	// While answer is incorrect make menu
	while (answer < 1 || answer > 2) {
		// Ask menu
		printf("Выберете фукцию:\n1. strcpy()\n2. memmove()\n3. exit()\n");
		// Get answer
		answer = getchar()-'0';
		// Ignore all after answer to avoid incorrect data in future
		ign_other(stdin);
		// Exit if answer is 3
		if (answer == 3) exit(0);
		// Else ask to try again
		else if (answer < 1 || answer > 2) {
			system("clear");
			printf("Вводить нужно только цифры от 1 до 3. Попробуйте ещё\n");
		}
	}
	while (status < 2) {
		printf("Введите строку: ");
		a = get_string(stdin,&status,&length);
		if (status == 1) {
			system("clear");
			printf("Слишком длинная строчка, максимальная длина строчки %i. Попробуйте ещё.\n", INT_MAX - 2 * CHUNK);
			ign_other(stdin);
		}
		if (status < 2) free(a);
	}
	// Make something by answer
	if (answer == 1) {
		// Create strings with certain length, to avoid overflows
		char string1[length],string2[length];
		// Saving starting time
		start = clock();
		printf("Воспользуемся самодельными функциями\n");
		// Copying a to string1
		printf("Копируем введенную строчку в string1\n");
		stringcopy(string1,a);
		// Copying string1 to string2
		printf("Копируем string1 в string2\n");
		stringcopy(string2,string1);
		printf("string1: %s\nstring2: %s\n",string1,string2);
		// Saving ending time
		end = clock();
		// Get execution time
		cpu_time_used1 = ((double) (end - start));
		printf("Операции выполнились за %f милисекунд\n",cpu_time_used1);
		// Saving starting time
		start = clock();
		printf("Воспользуемся стандартными функциями\n");
		// Copying a to string1
		printf("Копируем введенную строчку в string1\n");
		strcpy(string1,a);
		// Copying string1 to string2
		printf("Копируем string1 в string2\n");
		strcpy(string2,string1);
		printf("string1: %s\nstring2: %s\n",string1,string2);
		// Saving ending time
		end = clock();
		// Get execution time
		cpu_time_used2 = ((double) (end - start));
		// Show debug data
		printf("Операции выполнились за %f милисекунд\n",cpu_time_used2);
		printf("Производительность уменьшилась в %f\n",cpu_time_used1/cpu_time_used2);
	} else {
		int x=0,y=0,l=0;
		char b[length];
		strcpy(b,a);
		while (x == y || x+l > length || y+l > length || length == 0) {
			// Input destanation memory block position
			printf("Введите позицию блока которую хотите заменить: ");
			scanf("%i",&x);
			// Input source memory block position
			printf("Введите позицию блока с помощью которой хотите заменить: ");
			scanf("%i",&y);
			// Input length of memory block
			printf("Введите длину блока: ");
			scanf("%i",&l);
			// Warn on incorrect input data
			if (x == y || x+l > length || y+l > length || length == 0) {
				system("clear");
				if (x == y || length == 0)
					printf("Ничего не поменяеться. ");
				else if (x+l > length || y+l > length)
					printf("Блок памяти вылазит за строку");
				printf("Попробуйте ещё\n");
			}
		}
		// Saving starting time
		start = clock();
		printf("Воспользуемся самодельными функциями\n");
		// Move memory block from x to y
		printf("Переместим часть строки\n");
		memorymove(a+x-1,a+y-1,l);
		printf("Получившиеся строка: %s\n",a);
		// Saving ending time
		end = clock();
		// Get execution time
		cpu_time_used1 = ((double) (end - start));
		printf("Операции выполнились за %f милисекунд\n",cpu_time_used1);
		// Saving starting time
		start = clock();
		printf("Воспользуемся самодельными функциями\n");
		// Move memory block from x to y
		printf("Переместим часть строки\n");
		memmove(b+x-1,b+y-1,l);
		printf("Получившиеся строка: %s\n",b);
		// Saving ending time
		end = clock();
		// Get execution time
		cpu_time_used2 = ((double) (end - start));
		// Show debug data
		printf("Операции выполнились за %f милисекунд\n",cpu_time_used2);
		printf("Производительность уменьшилась в %f\n",cpu_time_used1/cpu_time_used2);
	}
	// Exit
	free(a);
	return 0;
}
