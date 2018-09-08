#include "stdlib.h"
#include "stdio.h"
#include "limits.h"

void ign_other(FILE * input) {
	char c = 0;
	while (c!='\n')
		c=fgetc(input);
}

int get_number (FILE * input, FILE * output, char message[], int min, int max) {
	int number = min-1, flag = 0;
	if (min == INT_MIN) flag = 1;
	while (number < min || number > max || flag) {
		fprintf(output,"%s:\n",message);
		fscanf(input,"%i",&number);
		ign_other(stdin);
		if (number < min || number > max) {
			system("clear");
			fprintf(output,"Вы ввели неправильне число, дозволено тільки ціле число від %i до %i\n",min,max);
		}
		if (flag == 1) flag = 0;
	}
	return number;
}

typedef struct element element;
struct element
{
	int value;
	struct element * prev;
	struct element * next;
};

size_t list_size (element * head) {
	if (head == NULL)
		return 0;
	size_t x=1;
	element * iter = head;
	while (iter->next != NULL) {
		x++;
		iter=iter->next;
	}
	return x;
}

element * list_add (element * head, int value, int position) {
	element * node = (element*) malloc(sizeof(element));
	node->value = value;
	node->next = NULL;
	node->prev = NULL;
	int size = list_size(head), pos = position % (size + 1) - 1;
	if (size == 0)
		return node;
	if (pos == -1) pos = rand() % size + 1;
	else if (pos < -1) pos += size + 2;
	element * prev = NULL, * next = head, * h = head;
	for (int i = 0; i < pos; i++) {
		prev = next;
		next = next->next;
	}
	node->prev = prev;
	node->next = next;
	if (prev != NULL)
		prev->next = node;
	else h = node;
	if (next != NULL)
		next->prev = node;
	return h;
}

element * fill_list(FILE * input, element * head, int count) {
	element * h = head, * node = NULL, * iter = head;
	int a = 0;
	if (iter != NULL)
		while (iter->next != NULL) iter = iter->next;
	for (int i=0; (count==0 || i<count) && !feof(input); i++) {
		fscanf(input,"%i",&a);
		element * node = (element*) malloc(sizeof(element));
		node->value = a;
		node->prev = iter;
		node->next = NULL;
		if (iter == NULL)
			h = node;
		else
			iter->next = node;
		iter = node;
	}
	return h;
}

char list_print (FILE * output, element * head) {
	if (head == NULL) {
		fprintf(output,"Список пустий\n");
		return 1;
	}
	element * iter = head;
	while (iter->next != NULL) {
		printf("%i ",iter->value);
		iter=iter->next;
	}
	printf("%i\n",iter->value);
	return 0;
}

element * list_find_by_key (element * head, int key) {
	if (head == NULL)
		return NULL;
	element * node = head;
	for (int i = 1; i < key; i++) {
		if (node->next == NULL) return NULL;
		node = node->next;
	}
	return node;
}

int list_find_by_value (element * head, int value) {
	if (head == NULL)
		return -1;
	element * node = head;
	for (int i = 1; node != NULL; i++) {
		if (node->value == value) return i;
		node = node->next;
	}
	return -1;
}

element * list_delete (element * head, element * node) {
	if (node == NULL || head == NULL) return head;
	element * h = head;
	if (node->prev != NULL && node->next != NULL) {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	} else if (node->prev == NULL && node->next != NULL) {
		h = node->next;
		node->next->prev = NULL;
	} else if (node->prev != NULL && node->next == NULL) {
		node->prev->next = NULL;
	} else if (node->prev == NULL && node->next == NULL) {
		h = NULL;
	}
	free(node);
	return h;
}

//~ element * delete_node_by_key (element * head, int key) {
	//~ if (head == NULL)
		//~ return NULL;
	//~ element * node = head, * prev = NULL, * next = NULL;
	//~ for (int i = 1; i < key; i++) {
		//~ if (node == NULL) return head;
		//~ prev = node;
		//~ node = node->next;
	//~ }
	//~ next = node->next;
	//~ free(node);
	//~ if (prev != NULL) {
		//~ prev->next = next;
		//~ return head;
	//~ } else return next;
//~ }

element * list_delete_by_value (element * head, int value) {
	if (head == NULL)
		return NULL;
	element * iter = head;
	while (iter->next != NULL) {
		if (iter->value == value)
			return list_delete(head,iter);
		else iter = iter->next;
	}
	return head;
}

element * delete_list (element * head) {
	if (head == NULL)
		return NULL;
	element * iter = head;
	element * next;
	while (iter != NULL) {
		next=iter->next;
		free(iter);
		iter=next;
	}
	return NULL;
}

int main() {
	element * head = NULL, * node = NULL;
	int answer = 0, x = 0, y = 0;
	FILE * input = NULL;
	while (answer != 12) {
		printf("Вибeріть дію:\n");
		printf("1. Додати n елементів в кінець списку з клавіатури\n");
		printf("2. Додати n елементів в кінець списку з файлу data.txt\n");
		printf("3. Додати елемент в потрібну позицію\n");
		printf("4. Видалити один елемент зі списку за значенням\n");
		printf("5. Видалити один елемент зі списку за номером\n");
		printf("6. Пошук елементу в списку за номером\n");
		printf("7. Пошук елементу в списку за значенням\n");
		printf("8. Вивести список на екран\n");
		printf("9. Вивести на екран поточну кількість елементів в списку\n");
		printf("10. Очищення списку\n");
		printf("12. Вихід\n");
		scanf("%i",&answer);
		ign_other(stdin);
		system("clear");
		switch (answer) {
			case 1:
				head = fill_list(stdin, head, get_number(stdin,stdout,"Введіть кількіть",1,INT_MAX-1));
				ign_other(stdin);
				break;
			case 2:
				input = fopen("data.txt", "r");
				if (input == NULL) {
					printf("Такого файла не існує.\n");
					break;
				}
				head = fill_list(input, head, get_number(stdin,stdout,"Введіть кількість",1,INT_MAX-1));
				break;
			case 3:
				list_print(stdout,head);
				x = get_number(stdin,stdout,"Введіть число",INT_MIN,INT_MAX);
				y = get_number(stdin,stdout,"Введіть позицію (нуль - рандомна позиція, відємне число - відрахвати з останього, позитивнне число - відрахувати з першого)",INT_MIN,INT_MAX);
				head = list_add(head, x, y);
				break;
			case 4:
				list_print(stdout,head);
				head = list_delete_by_value(head, get_number(stdin,stdout,"Введіть число",INT_MIN+1,INT_MAX-1));
				break;
			//~ case 5:
				//~ list_print(stdout,head);
				//~ head = list_delete_by_key(head, get_number(stdin,stdout,"Введіть номер",1,INT_MAX-1));
				//~ break;
			//~ case 6:
				//~ node = find_by_key(head, get_number(stdin,stdout,"Введіть номер",1,INT_MAX-1));
				//~ if (node == NULL)
					//~ printf("Елемент не знайдений\n");
				//~ else
					//~ printf("Елемент має значення: %i\n",(int)node->value);
				//~ break;
			//~ case 7:
				//~ show_list(stdout,head);
				//~ x = find_by_value(head, get_number(stdin,stdout,"Введіть значення",INT_MIN+1,INT_MAX-1));
				//~ if (x == -1)
					//~ printf("Елемент не знайдений\n");
				//~ else
					//~ printf("Елемент знаходиться на %i позиції\n",x);
				//~ break;
			case 8:
				break;
			case 9:
				printf("Список має таку кількість елементів: %lu\n", list_size(head));
				break;
			case 10:
				head = delete_list(head);
				break;
			case 11:
				break;
			default:
				printf("Неправильне значення, треба ввести число від 1 до 12\n");
		}
		if (answer != 11) {
			list_print(stdout,head);
			printf("Натисніть ENTER для продовження\n");
			ign_other(stdin);
			system("clear");
		}
	}
	delete_list(head);
	return 0;
}
