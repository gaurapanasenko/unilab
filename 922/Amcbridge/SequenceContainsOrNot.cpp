#include <stdio.h>
#include <stdlib.h>

typedef long long type;                                    /* array type */
#define MAX 64            /* stack size for max 2^(64/2) array elements  */

void quicksort_iterative(type array[], unsigned len) {
   unsigned left = 0, stack[MAX], pos = 0, seed = rand();
   for ( ; ; ) {                                           /* outer loop */
      for (; left+1 < len; len++) {                /* sort left to len-1 */
         if (pos == MAX) len = stack[pos = 0];  /* stack overflow, reset */
         type pivot = array[left+seed%(len-left)];  /* pick random pivot */
         seed = seed*69069+1;                /* next pseudorandom number */
         stack[pos++] = len;                    /* sort right part later */
         for (unsigned right = left-1; ; ) { /* inner loop: partitioning */
            while (array[++right] < pivot);  /* look for greater element */
            while (pivot < array[--len]);    /* look for smaller element */
            if (right >= len) break;           /* partition point found? */
            type temp = array[right];
            array[right] = array[len];                  /* the only swap */
            array[len] = temp;
         }                            /* partitioned, continue left part */
      }
      if (pos == 0) break;                               /* stack empty? */
      left = len;                             /* left to right is sorted */
      len = stack[--pos];                      /* get next range to sort */
   }
}

int Search_Binary (long long int arr[], long left, long right, long key)
{
	long long int midd = 0;
	while (1)
	{
		midd = (left + right) / 2;

		if (key < arr[midd])       // если искомое меньше значения в ячейке
			right = midd - 1;      // смещаем правую границу поиска
		else if (key > arr[midd])  // если искомое больше значения в ячейке
			left = midd + 1;	   // смещаем левую границу поиска
		else                       // иначе (значения равны)
			return midd;           // функция возвращает индекс ячейки

		if (left > right)          // если границы сомкнулись
			return -1;
	}
}

int main() {
	long long int *a,*b,x,n=0,m=0;
	a = (long long*) calloc(1,sizeof(long long));
	b = (long long*) calloc(1,sizeof(long long));
	scanf("%lli",&x);
	while (x != -1) {
		n++;
		a[n-1] = x;
		a = (long long*) realloc(a,(n+1)*sizeof(long long));
		scanf("%lli",&x);
	}
	scanf("%lli",&x);
	while (x != -1) {
		m++;
		b = (long long*) realloc(b,(m+1)*sizeof(long long));
		b[m-1] = x;
		scanf("%lli",&x);
	}
	quicksort_iterative(b,m);
	//~ for (int i=0;i<n;i++)
		//~ printf("%lli ",a[i]);
	//~ printf("\n");
	//~ for (int i=0;i<m;i++)
		//~ printf("%lli ",b[i]);
	//~ printf("\n");
	for (int i=0;i<n;i++) {
		if (Search_Binary(b,0,m-1,a[i]) != -1)
			printf("1 ");
		else
			printf("0 ");
	}
	printf("-1");
	free(a);
	free(b);
	return 0;
}
