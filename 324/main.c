#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK 128

void ign_other(FILE * input) {
	char c = 0;
	while (c!='\n')
		c=fgetc(input);
}

int get_number (FILE * input, FILE * output, char message[], int min, int max) {
	int number = min-1;
	while (number < min || number > max) {
		fprintf(output,"%s:\n",message);
		fscanf(input,"%i",&number);
		ign_other(stdin);
		if (number < min || number > max) {
			system("clear");
			fprintf(output,"Вы ввели неправильне число, дозволено тільки ціле число від %i до %i\n",min,max);
		}
	}
	return number;
}

typedef struct stack stack;
struct stack
{
	int * a;
	int size;
	int chunks;
};

stack * stack_init () {
	stack * st = (stack*) malloc(sizeof(stack));
	st->chunks = 1;
	st->size = 0;
	st->a = (int *) calloc(st->chunks*CHUNK,sizeof(int));
	return st;
}

void stack_push (stack * st, int x) {
	if (st->chunks*CHUNK == st->size) {
		st->chunks++;
		st->a = (int *) realloc (st->a, st->chunks * CHUNK * sizeof(int));
	}
	st->a[st->size++]=x;

}

char stack_empty (stack * st) {
	if (st->size == 0) return 1;
	else return 0;
}

int stack_size (stack * st) {
	return st->size;
}

int stack_pop (stack * st) {
	if (stack_empty(st)) return 0;
	int x = st->a[st->size-1];
	st->size--;
	return x;
}

int stack_top (stack * st) {
	if (stack_empty(st)) return 0;
	return st->a[st->size-1];
}

void stack_print (FILE * output, stack * st) {
	if (stack_empty(st)) {
		fprintf(output,"Stack is empty\n");
		return;
	}
	for (int i=0;i<stack_size(st)-1;i++)
		fprintf(output,"%i ",st->a[i]);
	fprintf(output,"%i\n",st->a[stack_size(st)-1]);
}

void stack_clean (stack * st) {
	st->chunks = 1;
	st->size = 0;
	st->a = (int *) realloc (st->a, st->chunks * CHUNK * sizeof(int));
}

void stack_print_info (FILE * output, stack * st) {
	if (stack_empty(st)) fprintf(output,"Stack is empty\n");
	fprintf(output,"Size - %i, first index - %i, last index - %i\n",st->size,0,st->size-1);
}

char * convert_to_binary (stack * st, int x) {
	stack_clean(st);
	int tmp = x;
	while (tmp > 1) {
		stack_push(st,tmp%2);
		tmp/=2;
	}
	if (tmp%2)
	stack_push(st,1);
	char * str = (char*) calloc(stack_size(st)+1,sizeof(char));
	for (int i=0;stack_size(st) > 0;i++) {
		str[i]=stack_pop(st)+'0';
	}
	return str;
}

stack * stack_delete (stack * st) {
	free(st->a);
	free(st);
	return NULL;
}

int main () {
	stack * st = stack_init();
	int answer = 0;
	while (answer != 7) {
		printf("Choose action:\n");
		printf("1. Push to stack\n");
		printf("2. Pop from stack\n");
		printf("3. Print stack\n");
		printf("4. Clean stack\n");
		printf("5. Print info about stack\n");
		printf("6. Convert number to binary\n");
		printf("7. Exit\n");
		scanf("%i",&answer);
		ign_other(stdin);
		system("clear");
		switch (answer) {
			case 1:
				stack_push(st, get_number(stdin,stdout,"Enter number",INT_MIN+1,INT_MAX-1));
				break;
			case 2:
				printf("Last element: %i\n",stack_pop(st));
				break;
			case 3:
				//~ stack_print(stdout,st);
				break;
			case 4:
				stack_clean(st);
				break;
			case 5:
				stack_print_info(stdout,st);
				break;
			case 6:
				printf("%s\n",convert_to_binary(st,get_number(stdin,stdout,"Enter number",INT_MIN+1,INT_MAX-1)));
				break;
			case 7:
				break;
			default:
				printf("Wrong input, you need to type number from 1 to 8\n");
		}
		if (answer != 7) {
			stack_print(stdout,st);
			printf("Click ENTER to exit.\n");
			ign_other(stdin);
			system("clear");
		}
	}
	stack_delete(st);
	return 0;
}
