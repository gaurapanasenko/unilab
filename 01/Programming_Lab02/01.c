#include <stdio.h>
#include <math.h>
#include <complex.h>

int main(int argc, char **argv)
{
	int a,b,tmp;double h,i;
	printf("Введите интервал [a,b] и шаг h\na=");
	scanf("%i",&a);
	printf("b=");
	scanf("%i",&b);
	printf("h=");
	scanf("%lf",&h);
	if (a>b) {
		tmp=a;a=b;b=tmp;
	}
	if (h==0) h=1;
	else if (h<0) h=-h;
	printf("[%i,%i] step %f\n",a,b,h);
	for (i=a;i<=b;i+=h) {
		if (-7<i && i<-5)
			printf("x=%-11g f(x)=%Lg\n",i,sqrtl(cabsl(sinl(i)))*cbrtl(expl(0.12*i))/3);
		else if (-5<i && i<-3)
			printf("x=%-11g f(x)=%g\n",i,pow(i,20));
		else printf("x=%-11g f(x)=%g\n",i,pow(i,-20));
	}
	return 0;
}
