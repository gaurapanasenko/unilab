#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//~ using namespace std;

int main(int argc,char *argv[]) {
	unsigned long n,i,j,k;
	long double ** a, * b, * c;
	clock_t t1,t2;
	double dur;
	if (argc > 1) {
		sscanf(argv[1],"%lu",&n);
	} else scanf("%lu",&n);
	t1 = clock();
	a = (long double**) calloc(n,sizeof(long double*));
	b = (long double*) calloc(n,sizeof(long double));
	c = (long double*) calloc(n,sizeof(long double));
	for (i=0;i<n;i++) {
		a[i] = (long double*) calloc(n,sizeof(long double));
		b[i] = rand()%51-25;
		c[i]=0;
		for(j=0;j<n;j++)
			a[i][j]=rand()%51-25;
	}
	for (i=0;i<n;i++) {
		//~ c[i]=0;
		for (k=0;k<n;k++) {
			c[i]+=a[i][k]*b[k];
		}
	}
	//~ for (i=0;i<n;i++) {
		//~ for (j=0;j<n;j++)
			//~ printf("%li ",a[i][j]);
		//~ printf("\n");
	//~ }
	//~ printf("\n\n");
	//~ for (i=0;i<n;i++) {
		//~ printf("%li ",b[i]);
	//~ }
	//~ printf("\n\n");
	//~ for (i=0;i<n;i++) {
		//~ printf("%li ",c[i]);
	//~ }
	//~ printf("\n\n");
	t2 = clock();
	dur = 1.0*(t2-t1)/CLOCKS_PER_SEC;
	printf("%lu,%lf\n",n,dur);
	for (i=0;i<n;i++)
		free(a[i]);
	free(a);
	return 0;
}
