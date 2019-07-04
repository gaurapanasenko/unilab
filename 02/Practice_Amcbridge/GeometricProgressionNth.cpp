#include <stdio.h>
#include <stdlib.h>

int main() {
	long double b0,q,qn=1;
	int n;
	scanf("%Lf%Lf%i",&b0,&q,&n);
	for (int i=1;i<n;i++)
		qn*=q;
	printf("%.8Lf",b0*qn);
	return 0;
}
