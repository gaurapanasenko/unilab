#include <stdio.h>

int main(int argc, char **argv)
{
	char str[256]; int i,x=0;
	printf("N = ");
	scanf("%255s",str);
	for (i=0;str[i];i++)
		if (str[i] >= '0' && str[i] <= '9')
			continue;
		else if (i == 0 && str[i] == '-')
			x=1;
		else if (str[i] == '.')
			x=2;
		else x=3;
	if (x == 1) printf("Error: N must be natural, not negative!");
	else if (x == 2) printf("Error: N must be natural, not real!");
	else if (x == 3) printf("Error: N must be number, not string!");
	return 0;
}

