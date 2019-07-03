#include <stdio.h>
#include <math.h>

int main(int argc, char **argv)
{
	long double x,e,sold=1,s5=0,s=0; int i=0,j=0,ep=0;
	printf("Введіть значення x в діапазоні [-1;1]\nx=");
	scanf("%Lf",&x);
	printf("Введіть потрібну точність обчислення сумми ряду:\nE=");
	scanf("%Lf",&e);
	for (;x<-1 || x>1;x/=10) {}
	ep=log10l(e);
	if (ep<0) ep=-ep;
	e=pow(10,-ep);
	printf("x=%Lg e=%Lg\n",x,e);
	for (;floor(sold/e)!=floor(s/e);i++) {
		sold=s;
		s+=powl(-1,i)*powl(x,2*i+1)/(2*i+1);
		if (i==4) s5=s;
	}
	if (i<4) {
		for (j=0;j<4;j++) {
			s5+=powl(-1,j)*powl(x,2*j+1)/(2*j+1);
		}
	}
	printf("%.*Lf - сумма перших п’яти членів ряду становить\n",ep,s5);
	printf("%.*Lf - сумма ряду с точністю %Lg становить\n",ep,s,e);
	printf("%.*Lf - результат обчислень за допомогою стандартних математичних функцій\n",ep,atanl(x));
	printf("%i - кількість виконаних ітерацій \n",i);
	return 0;
}

