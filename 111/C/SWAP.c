#include <stdio.h>

int main(int argc, char **argv)
{
	char a[256],b[256],c[256],d[256];
	scanf("%s%s%s%s",a,b,c,d);
	printf("A=%s B=%s C=%s D=%s\n",a,b,c,d);
	printf("A=%s B=%s C=%s D=%s\n",d,a,b,c);
	return 0;
}
