#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

typedef struct element element;
struct element
{
	int value;
	struct element * next;
};

element * add_to_end(element * head, int value) {
	element * node = (element*) malloc(sizeof(element));
	node->value = value;
	node->next = NULL;
	if (head == NULL) {
		return node;
	}
	element * iter = head;
	while (iter->next != NULL) {
		iter=iter->next;
	}
	iter->next = node;
	return head;
}

element * fill_list(FILE * input, element * head, int count) {
	element * h = head, * node = head, * prev = NULL;
	int a = 0;
	while (node != NULL) {
		prev = node;
		node = node->next;
	}
	for (int i=0; (count==0 || i<count) && !feof(input); i++) {
		fscanf(input,"%i",&a);
		element * node = (element*) malloc(sizeof(element));
		node->value = a;
		node->next = NULL;
		if (prev == NULL)
			h = node;
		else
			prev->next = node;
		prev = node;
	}
	return h;
}

char show_list (FILE * output, element * head) {
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

size_t count_list (element * head) {
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

element * find_by_key (element * head, int key) {
	if (head == NULL)
		return NULL;
	element * node = head;
	for (int i = 1; i < key; i++) {
		if (node->next == NULL) return NULL;
		node = node->next;
	}
	return node;
}

int find_by_value (element * head, int value) {
	if (head == NULL)
		return -1;
	element * node = head;
	for (int i = 1; node != NULL; i++) {
		if (node->value == value) return i;
		node = node->next;
	}
	return -1;
}

element * delete_node_by_value (element * head, int value) {
	if (head == NULL)
		return NULL;
	element * node = head, * prev = NULL, * next = NULL;
	while (node != NULL) {
		if (node->value == value) {
			next = node->next;
			free(node);
			if (prev != NULL) {
				prev->next = next;
				return head;
			} else return next;
		}
		prev = node;
		node = node->next;
	}
	return head;
}

element * delete_node_by_key (element * head, int key) {
	if (head == NULL)
		return NULL;
	element * node = head, * prev = NULL, * next = NULL;
	for (int i = 1; i < key; i++) {
		if (node == NULL) return head;
		prev = node;
		node = node->next;
	}
	next = node->next;
	free(node);
	if (prev != NULL) {
		prev->next = next;
		return head;
	} else return next;
}

void delete_even (element * head) {
	if (head == NULL)
		return;
	int len = count_list(head)/2;
	for (int i = 1; i <= len; i++) {
		delete_node_by_key(head,i+1);
	}
}

/*
* function delete_even (list)
* 	if list = null then
* 		return
* 	end if
* 	len = count_list(head) / 2;
* 	for i=1 to len step 1 do
* 		delete_node_by_key(list,i+1)
* 	end for
* end function
*
* Difficulty:
* T(list[1..n]) = 2 + (4+3*n) + 3*(n/2-1) + 9*(n/2-1) + 5*(2+3+...+n/2) =
* = 9*n-6 + 5*(2+n/2)*(n/2-1)/2 =
* = n^2/6+55/4*n-19/4=Θ(n^2)
* M(list[1..n]] = 1+2+4 = 6 = Θ(1)
* Funcion is quadratic
*/

void delete_even_fast (element * head) {
	if (head == NULL)
		return;
	element * node = head, * prev = NULL, * next = NULL;
	for (int i = 1; node != NULL; i=(i+1)%2) {
		next = node->next;
		if (i%2 == 0) {
			free(node);
			if (prev != NULL) {
				prev->next = next;
			}
		}
		prev = node;
		node = next;
	}
}

/*
* function delete_even_fast (list)
* 	if list = null then
* 		return
* 	end if
* 	node = list
* 	i = 1
* 	while node != null do
* 		i = (i+1) % 2
* 		next = node.next
* 		if i % 2 = 0 then
* 			FREE MEMORY (node)
* 			if prev != null then
* 				prev.next = next
* 			end if
* 		end if
* 		prev = node
* 		node = next
* 	end while
* end function
*
* Difficulty: 4*n/2+7*n/2+3 = 11*n/2+3
* 11*n/2+3=Θ(n)
* Function is linear
*/

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
	int answer = 0,x = 0;
	FILE * input = NULL;
	while (answer != 12) {
		printf("Вибeріть дію:\n");
		printf("1. Додати n елементів в кінець списку з клавіатури\n");
		printf("2. Додати n елементів в кінець списку з файлу data.txt\n");
		printf("3. Додати один елемент в кінець списку\n");
		printf("4. Видалити один елемент зі списку за значенням\n");
		printf("5. Видалити один елемент зі списку за номером\n");
		printf("6. Пошук елементу в списку за номером\n");
		printf("7. Пошук елементу в списку за значенням\n");
		printf("8. Вивести список на екран\n");
		printf("9. Вивести на екран поточну кількість елементів в списку\n");
		printf("10. Очищення списку\n");
		printf("11. Видалити елементи з парними номерами\n");
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
				head = add_to_end(head, get_number(stdin,stdout,"Введіть число",INT_MIN+1,INT_MAX-1));
				break;
			case 4:
				show_list(stdout,head);
				head = delete_node_by_value(head, get_number(stdin,stdout,"Введіть число",INT_MIN+1,INT_MAX-1));
				break;
			case 5:
				show_list(stdout,head);
				head = delete_node_by_key(head, get_number(stdin,stdout,"Введіть номер",1,INT_MAX-1));
				break;
			case 6:
				node = find_by_key(head, get_number(stdin,stdout,"Введіть номер",1,INT_MAX-1));
				if (node == NULL)
					printf("Елемент не знайдений\n");
				else
					printf("Елемент має значення: %i\n",(int)node->value);
				break;
			case 7:
				show_list(stdout,head);
				x = find_by_value(head, get_number(stdin,stdout,"Введіть значення",INT_MIN+1,INT_MAX-1));
				if (x == -1)
					printf("Елемент не знайдений\n");
				else
					printf("Елемент знаходиться на %i позиції\n",x);
				break;
			case 8:
				break;
			case 9:
				printf("Список має таку кількість елементів: %u\n", count_list(head));
				break;
			case 10:
				head = delete_list(head);
				break;
			case 11:
				show_list(stdout,head);
				delete_even(head);
				break;
			case 12:
				break;
			default:
				printf("Неправильне значення, треба ввести число від 1 до 12\n");
		}
		if (answer != 12) {
			show_list(stdout,head);
			printf("Натисніть ENTER для продовження\n");
			ign_other(stdin);
			system("clear");
		}
	}
	delete_list(head);
	return 0;
}
