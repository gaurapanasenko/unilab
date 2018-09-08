#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void ign_other() {
	char instr[2] = {0,0};
	while (instr[0]!='\n')
		fgets(instr,2,stdin);
}

int main(int argc, char **argv)
{
	int n=0,m=0,i;
	char yn[2]={0,0};
	printf("Введіть довжину a: |a|=");
	while (n<=0) {
		scanf("%i",&n);
		ign_other();
		if (n<=0) printf("Довжина не може бути нульовою або від'ємною, спробуйте ще: |a|=");
	}
	printf("Введіть довжину b: |b|=");
	while (m<=0) {
		scanf("%i[\n]",&m);
		ign_other();
		if (m<=0) printf("Довжина не може бути нульовою або від'ємною, спробуйте ще: |b|=");
	}
	while (yn[0]!='y' && yn[0]!='n') {
		printf("Згенерувати вектори автоматично? (y/n) ");
		fgets(yn,2,stdin);
		ign_other();
		if (yn[0]!='y' && yn[0]!='n')
			printf("Невірна відповідь, спробуйте ще\n");
	}
	int a[n],b[m];
	if (yn[0]=='n') {
		printf("Введіть елементи a через пробіл:\n");
		for (i=0;i<n;i++) scanf("%i",&a[i]);
		ign_other();
		printf("Введіть елементи b через пробіл:\n");
		for (i=0;i<m;i++) scanf("%i",&b[i]);
		ign_other();
	} else {
		srand(time(NULL));
		for (i=0;i<n;i++) a[i]=rand()%2000-1000;
		for (i=0;i<m;i++) b[i]=rand()%2000-1000;
	}
	printf("Вхідні дані:\n|a|=%-5i a={",n);
	for (i=0;i<n-1;i++) printf("%-6i ",a[i]);
	printf("%i}\n|b|=%-5i b={",a[n-1],m);
	for (i=0;i<m-1;i++) printf("%-6i ",b[i]);
	printf("%i}\n",b[m-1]);
	int c[m+n],min=0,max=a[0],sum=0,mn=0;
	for (i=0;i<n || i<m;i++) {
		if (i<n && (a[i]%7==0 || a[i]%11==0)) {
			c[mn]=i+1;mn++;
		}
		if (i<m && (b[i]%7==0 || b[i]%11==0)) {
			c[mn]=i+1;mn++;
		}
		if (i<n && a[i]<a[min])
			min=i;
		if (i<m) {
			sum+=b[i];
			if(b[i]>max) max=b[i];
		}
	}
	double avrg=sqrt(abs(sum/m));
	a[min]=abs(max);
	for (i=0;i<m;i++) b[i]*=avrg;
	printf("Отримані дані:\n|a|=%-5i a={",n);
	for (i=0;i<n-1;i++) printf("%-6i ",a[i]);
	printf("%i}\n|b|=%-5i b={",a[n-1],m);
	for (i=0;i<m-1;i++) printf("%-6i ",b[i]);
	printf("%i}\n",b[m-1]);
	if (mn) {
		printf("|c|=%-5i c={",mn);
		for (i=0;i<mn-1;i++) printf("%-6i ",c[i]);
		printf("%i}\n",c[mn-1]);
	} else printf("|c|=0");
	return 0;
}
