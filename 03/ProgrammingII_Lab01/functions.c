#include "functions.h"

/* Ignore all data until end of line */
void ign_other(FILE * input) {
	char c = 0;
	while ((c = fgetc(input)) != EOF && c != '\n') {}
}

int get_number(FILE * input, int * number) {
	char c, i = 0, neg = 1, ret = -1; int x = 0;
	while ((c = fgetc(input)) != '\n') {
		if (ret == -1) {
			if (i == 0 && c == '-') neg = -1;
			else if ((i == 0 && c == ' ')
				|| (neg == -1 && i == 1 && c == ' ')
				|| (c < '0' && c > '9')) ret = 1;
			else if (c == ' ') ret = 0;
			else if (c >= '0' && c <= '9' && i < NUM_SIZE) {
				x = x * 10 + c - '0'; i++;
			}
			else ret = 1;
		}
	}
	if (i == 0 || (neg == -1 && i == 1)) ret = 1;
	else if (ret == -1) ret = 0;
	if (ret == 0) *number = neg * x;
	return ret;
}

int menu (FILE * input, FILE * output, char * message, int count) {
	// Main menu
	int answer = -1;
	// While answer is incorrect make menu
	while (answer < 0 || answer >= count) {
		// Ask menu
		printf("%s", message);
		// Get answer
		if (get_number(input, &answer) != 0)
			answer = -2;
		// Ask to try again
		if (answer < 0 || answer >= count) {
			system("clear");
			printf(
			"Вводить нужно только цифры от 0 до %hhi. Попробуйте ещё\n",
			count - 1);
		}
	}
	return answer;
}

int rnd(int min, int max) {
	return rand() % (max - min) + min;
}

int * generate_array(int n, int min, int max) {
	int * a = malloc(n * sizeof(int)), i;
	if (a == NULL) return NULL;
	for (i = 0; i < n; i++)
		a[i] = rnd(min, max);
	return a;
}

void print_array(FILE * input, int * a, int n) {
	if (n == 0) {
		printf("Массив пустой");
		return;
	}
	for (int i = 0; i < n; i++)
		printf("%i ", a[i]);
	printf("\n");
}

int summarize(int * a, int n) {
	int num = 0, i;
	for (i = 0; i < n; i++)
		num += a[i];
	return num;
}

void multiply_elem(int x, int * a, int n) {
	for (int i = 0; i < n; i++) a[i] *= x;
}

void split(int * a, int n, int *  b, int *bn, int *  c, int * cn) {
	*bn = 0; *cn = 0;
	for (int i = 0; i < n; i++)
		if (a[i] < 0) b[(*bn)++] = a[i];
		else c[(*cn)++] = a[i];
}

int * summarize_arrays(int * a, int * b, int n) {
	int * c = malloc(n * sizeof(int)), i;
	if (a == NULL) return NULL;
	for (i = 0; i < n; i++) c[i] = a[i] + b[i];
	return c;
}
