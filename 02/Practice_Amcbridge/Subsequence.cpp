#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
	long int n,m,*a,*b,j=0,k=0;
	scanf("%li",&n);
	a = new long int[n];
	for (int i=0;i<n;i++)
		scanf("%li",&a[i]);
	scanf("%li",&m);
	b = new long int[m];
	for (int i=0;i<m;i++)
		scanf("%li",&b[i]);
	for (int i=0;i<m;i++) {
		while (a[j]!=b[i] && j<=n)
			j++;
		if (j==n) {
			printf("0");
			return 0;
		} else if (a[j]==b[i]) k++;
	}
	if (k == m) printf("1");
	else  printf("0");
	delete a;
	delete b;
	return 0;
}
