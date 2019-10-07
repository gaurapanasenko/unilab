#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void ign_other(FILE * input) {
	char c = 0;
	while (c!='\n')
		c=fgetc(input);
}

int* merge_sort(int *up, int *down, unsigned int left, unsigned int right)
{
	if (left == right)
	{
		down[left] = up[left];
		return down;
	}

	unsigned int middle = (left + right) / 2;

	int *l_buff = merge_sort(up, down, left, middle);
	int *r_buff = merge_sort(up, down, middle + 1, right);

	int *target = l_buff == up ? down : up;

	unsigned int l_cur = left, r_cur = middle + 1;
	for (unsigned int i = left; i <= right; i++)
	{
		if (l_cur <= middle && r_cur <= right)
		{
			if (l_buff[l_cur] < r_buff[r_cur])
			{
				target[i] = l_buff[l_cur];
				l_cur++;
			}
			else
			{
				target[i] = r_buff[r_cur];
				r_cur++;
			}
		}
		else if (l_cur <= middle)
		{
			target[i] = l_buff[l_cur];
			l_cur++;
		}
		else
		{
			target[i] = r_buff[r_cur];
			r_cur++;
		}
	}
	return target;
}

void bubleSort(int *mas, int size) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size - i - 1; j++) {
			if(mas[j] > mas[j+1]) {
				int tmp = mas[j];
				mas[j] = mas[j+1];
				mas[j+1] = tmp;
			}
		}
	}
}

void bubleSortFlagged(int *mas, int size) {
	char f = 0,tmp;
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size - i - 1; j++) {
			if(mas[j] > mas[j+1]) {
				f = 1;
				tmp = mas[j];
				mas[j] = mas[j+1];
				mas[j+1] = tmp;
			}
		}
		if (f == 0) break;
	}
}

int main(int argc,char *argv[]) {
	srand(time(NULL));
	int * a, * b ,* c, * d, min = 0, max = 0, n = 0, i = 0;
	char first_run = 1;
	clock_t t1,t2;
	double dur[9];
	FILE * src, * dst, * stat;
	if (argc > 3) {
		sscanf(argv[1],"%i",&n);
		sscanf(argv[2],"%i",&min);
		sscanf(argv[3],"%i",&max);
		if (n < 1 || n > INT_MAX-1) return 1;
		if (min >= max) return 1;
	}
	while (n < 1 || n > INT_MAX-1) {
		printf("Введіть довжину масиву:\n");
		scanf("%i",&n);
		ign_other(stdin);
		if (n < 1 || n > INT_MAX-1) {
			system("clear");
			printf("Вы ввели неправильну довжину, дозволено тільки ціле число від 1 до %i\n",INT_MAX-1);
		}
	}
	while (min >= max) {
		printf("Введіть інтервал дозволених значень в масиві:\n");
		scanf("%i%i",&min,&max);
		ign_other(stdin);
		if (min >= max) {
			system("clear");
			printf("Вы ввели неправильний інтервал, дозволено тільки цілі числа, при чому перше число повинно бути менше ніж друге\n");
		}
	}
	src =	fopen("src.txt","w");
	dst =	fopen("dst.txt","w");
	stat =	fopen("statistics.csv","w");
	if (!src || !dst || !stat) {
		if (argc < 4)
			printf("Невозможно создать файл\n");
		return 1;
	}
	for (; n <= 50000; n += 1000) {
		// ######### //
		// Best time //
		// ######### //
		a = (int*) calloc(n, sizeof(int));
		b = (int*) calloc(n, sizeof(int));
		for (i = 0; i < n; i++) {
			a[i] = i;
			if (first_run == 1) fprintf(src,"%i ",i);
		}
		if (first_run == 1) fprintf(src,"\n");

		// Merge sort
		t1 = clock();
		c = merge_sort(a,b,0,n-1);
		t2 = clock();
		dur[0] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (a == c) free(b);
		else {
			free(a);
			a=b;
		}
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",a[i]);
			fprintf(dst,"\n");
		}

		// Bubble sort
		t1 = clock();
		bubleSort(a,n);
		t2 = clock();
		dur[1] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",a[i]);
			fprintf(dst,"\n");
		}

		// Flagged bubble sort
		t1 = clock();
		bubleSortFlagged(a,n);
		t2 = clock();
		dur[2] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",a[i]);
			fprintf(dst,"\n\n");
		}


		// ########### //
		// Common time //
		// ########### //
		b = (int*) calloc(n, sizeof(int));
		c = (int*) calloc(n, sizeof(int));
		for (i = 0; i < n; i++) {
			a[i]=rand()%(max - min + 1) + min;
			b[i]=a[i];
			if (first_run == 1) {
				fprintf(src,"%i ",a[i]);
			}
		}
		if (first_run == 1) {
			fprintf(src,"\n");
		}

		// Merge sort
		t1 = clock();
		d = merge_sort(b,c,0,n-1);
		t2 = clock();
		dur[3] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (b == d) free(c);
		else {
			free(b); b = c;
		}
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",b[i]);
			fprintf(dst,"\n");
		}

		// Bubble sort
		for (i = 0; i < n; i++)
			b[i] = a[i];
		t1 = clock();
		bubleSort(b,n);
		t2 = clock();
		dur[4] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",b[i]);
			fprintf(dst,"\n");
		}

		// Flagged bubble sort
		for (i = 0; i < n; i++)
			b[i] = a[i];
		t1 = clock();
		bubleSortFlagged(b,n);
		t2 = clock();
		dur[5] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",b[i]);
			fprintf(dst,"\n\n");
		}


		// ########### //
		// Worst time //
		// ########### //
		for (i = 0; i < n; i++) {
			a[i] = n - i;
			if (first_run == 1) fprintf(src,"%i ",a[i]);
		}
		if (first_run == 1) fprintf(src,"\n");

		// Merge sort
		t1 = clock();
		c = merge_sort(a,b,0,n-1);
		t2 = clock();
		dur[6] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (a == c) free(b);
		else {
			free(a);
			a=b;
		}
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",a[i]);
			fprintf(dst,"\n");
		}

		// Bubble sort
		for (i = 0; i < n; i++)
			a[i] = n - i;
		t1 = clock();
		bubleSort(a,n);
		t2 = clock();
		dur[7] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",a[i]);
			fprintf(dst,"\n");
		}

		// Flagged bubble sort
		for (i = 0; i < n; i++)
			a[i] = n - i;
		t1 = clock();
		bubleSortFlagged(a,n);
		t2 = clock();
		dur[8] = 1.0 * (t2 - t1) / CLOCKS_PER_SEC;
		if (first_run == 1) {
			for (i = 0; i < n; i++)
				fprintf(dst,"%i ",a[i]);
			fprintf(dst,"\n\n");
		}


		fprintf(stat,"%i,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", n, dur[0], dur[1], dur[2], dur[3], dur[4], dur[5], dur[6], dur[7], dur[8]);
		printf("%i,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", n, dur[0], dur[1], dur[2], dur[3], dur[4], dur[5], dur[6], dur[7], dur[8]);
		if (first_run == 1) {
			fclose(src);
			fclose(dst);
		}
		free(a);
		if (first_run == 1) {
			first_run = 0;
			n = 1000;
		}
	}
	fclose(stat);
}
