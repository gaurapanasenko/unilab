#include <stdio.h>

int main(int argc, char **argv)
{

	char str[256];
	while (1) {
		printf("Введите строку и нажмите ENTER или нажмите просто ENTER для выхода\n");
		fgets(str,255,stdin);
		if (str[0] == 10)
			break;
		//~ printf("%i\n",str[0]);
		printf("%s\n",str);
	}
	return 0;
}

