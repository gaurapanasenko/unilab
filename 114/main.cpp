#include <cstdio>
#include <stdlib.h>

using namespace std;

class Matrix {
	int n,m,**a;
	public:
	Matrix (int height, int width) {
		n=height;m=width;
		a=new int*[n];
		for(int i=0;i<n;i++)
		a[i]=new int[m];
	}
	void set (int i, int j, int x) {
		a[i][j]=x;
	}
	int get (int i, int j) {
		return a[i][j];
	}
	void find_max(int max[2]) {
		max[0] = 0; max[1] = 0;
		for (int i=0;i<n;i++)
			for (int j=0;j<m;j++)
				if (a[i][j]>a[max[0]][max[1]]) {
					max[0]=i;
					max[1]=j;
				}
	}
	void find_min(int min[2]) {
		min[0] = 0; min[1] = 0;
		for (int i=0;i<n;i++)
			for (int j=0;j<m;j++)
				if (a[i][j]<a[min[0]][min[1]]) {
					min[0]=i;
					min[1]=j;
				}
	}
	void sort_col(int col) {
		int tmp=0;
		for (int i=0;i<n-1;i++)
			for (int j=i+1;j<n;j++)
				if (a[j][col]<a[i][col]) {
					tmp=a[j][col];a[j][col]=a[i][col];a[i][col]=tmp;
				}
	}
	void input_matrix() {
		for (int i=0;i<n;i++)
			for (int j=0;j<m;j++) {
				scanf("%i",&a[i][j]);
			}
	};
	void show(char x) {
		printf("  -%*c-\n",m*6,' ');
		for (int i=0;i<n;i++) {
			if (i==n/2) printf("%c=|",x);
			else printf("  |");
			for (int j=0;j<m;j++)
				printf("%-6i",a[i][j]);
			printf("|\n");
		}
		printf("  -%*c-\n",m*6,' ');
	}
	void fill_rand() {
		for (int i=0;i<n;i++)
			for (int j=0;j<m;j++)
				a[i][j]=rand()%2000-1000;
	}
};

void ign_other() {
	char instr[2] = {0,0};
	while (instr[0]!='\n')
		fgets(instr,2,stdin);
}

int main(int argc, char **argv)
{
	char ans1,ans2[2];
	int n=0,m=0,max[2]={0,0},min[2]={0,0},tmp;
	while (n==m || n<6 || m<6) {
		printf("Введіть розмірність NxM через пробіл\n");
		scanf("%i%i",&n,&m);
		if (n==m || n<6 || m<6)
			printf("Невірні вхідні данні\n");
	}
	while (ans1!=1 && ans1!=2 && ans1!=3) {
		printf("Ви можете:\n1. Ввести матриці з файлу input.txt\n2. Ввести матриці вручну\n3. Сгенерувати автоматично\nЩо вам до вподоби? (1/2/3) ");
		scanf("%hhi",&ans1);
		ign_other();
		if (ans1!=1 && ans1!=2 && ans1!=3)
			printf("Невірна відповідь, спробуйте ще\n");
	}
	while (ans2[0]!='y' && ans2[0]!='n') {
		printf("Вивести данні у файл output.txt? (y/n) ");
		fgets(ans2,2,stdin);
		ign_other();
		if (ans2[0]!='y' && ans2[0]!='n')
			printf("Невірна буква, спробуйте ще\n");
	}
	if (ans1==1)
		freopen("input.txt","r",stdin);
	Matrix a(n,m), b(n,m), c(n,m);
	if (ans1==3) {
		a.fill_rand();
		b.fill_rand();
	} else {
		if (ans1==2)
			printf("Введіть матрицю A розмірністю NxM\n");
		a.input_matrix();
		if (ans1==2)
			printf("Введіть матрицю B розмірністю NxM\n");
		b.input_matrix();
	}
	if (ans2[0]=='y')
		freopen("output.txt","w",stdout);
	printf("Вхідні матриці:\n");
	a.show('A');
	b.show('B');
	b.find_max(max);a.find_min(min);
	tmp=a.get(min[0],min[1]);
	a.set(min[0],min[1],b.get(max[0],max[1]));
	b.set(max[0],max[1],tmp);
	printf("Матриці A і B після першого пункту:\n");
	a.show('A');
	b.show('B');
	for (int i=0;i<n;i++)
		for (int j=0;j<m;j++)
			c.set(i,j,a.get(i,j)+b.get(i,j));
	printf("Матриця C після другого пункту:\n");
	c.show('C');
	a.sort_col(max[1]);
	printf("Матриця A після третього пункту:\n");
	a.show('A');
	return 0;
}
