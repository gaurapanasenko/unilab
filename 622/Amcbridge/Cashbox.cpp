#include <iostream>
#include <cstdlib>

using namespace std;

typedef long long type;                                    /* array type */
#define MAX 64            /* stack size for max 2^(64/2) array elements  */

void quicksort_iterative(type ** array, unsigned len) {
   unsigned left = 0, stack[MAX], pos = 0, seed = rand();
   for ( ; ; ) {                                           /* outer loop */
      for (; left+1 < len; len++) {                /* sort left to len-1 */
         if (pos == MAX) len = stack[pos = 0];  /* stack overflow, reset */
         type pivot = array[left+seed%(len-left)][0];  /* pick random pivot */
         seed = seed*69069+1;                /* next pseudorandom number */
         stack[pos++] = len;                    /* sort right part later */
         for (unsigned right = left-1; ; ) { /* inner loop: partitioning */
            while (array[++right][0] < pivot);  /* look for greater element */
            while (pivot < array[--len][0]);    /* look for smaller element */
            if (right >= len) break;           /* partition point found? */
            type temp = array[right][0];
            array[right][0] = array[len][0];
            array[len][0] = temp;
            temp = array[right][1];
            array[right][1] = array[len][1];
            array[len][1] = temp;
            temp = array[right][2];
            array[right][2] = array[len][2];
            array[len][2] = temp;
         }                            /* partitioned, continue left part */
      }
      if (pos == 0) break;                               /* stack empty? */
      left = len;                             /* left to right is sorted */
      len = stack[--pos];                      /* get next range to sort */
   }
}

int Search_Binary (type ** arr, type left, type right, type key)
{
	type midd = 0;
	while (1)
	{
		midd = (left + right) / 2;

		if (key < arr[midd][0])       // если искомое меньше значения в ячейке
			right = midd - 1;      // смещаем правую границу поиска
		else if (key > arr[midd][0])  // если искомое больше значения в ячейке
			left = midd + 1;	   // смещаем левую границу поиска
		else                       // иначе (значения равны)
			return midd;           // функция возвращает индекс ячейки

		if (left > right)          // если границы сомкнулись
			return -1;
	}
}

int main() {
	type ** arr, i, j, k, l = 0, n = 0, d = 0, sum;
	arr = (type **) calloc(0,sizeof(type[3]));
	for (cin >> i; i != -1; cin >> i) {
		n++;
		cin >> j >> k;
		arr = (type **) realloc(arr,n*sizeof(type[3]));
		arr[n-1] = (type*) calloc(3,sizeof(type));
		arr[n-1][0] = i;
		arr[n-1][1] = j;
		arr[n-1][2] = k;
	}
	quicksort_iterative(arr,n);
	//~ for (i = 0; i < n; i++) {
		//~ for (j = 0; j < 3; j++)
			//~ cout << arr[i][j] << " ";
		//~ cout << endl;
	//~ }
	cin >> d;
	for (i = 0; i < d; i++) {
		sum = 0;
		for (cin >> j; j != -1; cin >> j) {
			cin >> k;
			l = Search_Binary(arr, 0, n - 1, j);
			if (l != -1) {
				if (k <= arr[l][2]) {
					arr[l][2] -= k;
					sum += k * arr[l][1];
				} else if (arr[l][2] > 0) {
					sum += arr[l][2] * arr[l][1];
					arr[l][2] = 0;
				}
			}
		}
		cout << sum << " ";
	}
	cout << "-1";
	return 0;
}
