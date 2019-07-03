#include <stdio.h>

int main (int argc, char *argv[])
{
	char name[256],school[256];
	int age;
	printf("Как Вас зовут? = ");
	scanf("%255s",name);
	printf("Сколько Вам лет? = ");
	scanf("%i",&age);
	printf("Где Вы учитесь? = "); // хотя наверное правильно было сказать учились)
	scanf("%255s",school); // todo: сделать FirstUppercase
	printf("\nЗдравствуйте, %s!\nПоздравляем Вас, Вам всего %i ",name,age);
	if ((age % 100 >= 5 && age % 100 <= 20) || (age % 10 >= 5 && age % 10 <= 9) || age % 10 == 0)
		printf("лет");
	else if (age % 10 == 1) printf("год");
	else printf("года");
	printf(", а Вы уже ведёте диалог с компьютером!\n");
	if (school[0] >= '0' && school[0] <= '9')
		printf("Школа №"); // на случай ввода только цыфр
	printf("%s будет гордиться Вами!\n",school);
	return 0;
}
