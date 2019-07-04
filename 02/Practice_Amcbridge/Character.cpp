#include <stdio.h>
#include <stdlib.h>

int main() {
	long long int MaxHP,HP,MP,HealCost,a = 0,b = 0;
	scanf("%lli%lli%lli%lli",&MaxHP,&HP,&MP,&HealCost);
	if (HP>MaxHP) HP=MaxHP;
	if (HP <= 0) {
		printf("0");
		return 0;
	}
	while (a != -1 || b != -1) {
		scanf("%lli",&a);
		if (a == -1) break;
		scanf("%lli",&b);
		if (b == -1) break;
		if (a == 1) {
			HP-=b;
		} else if (a == 2 && MP>=HealCost) {
			MP-=HealCost;
			HP+=b;
		} else if (a == 3)
			MP+=b;
		if (HP <= 0) {
			printf("0");
			return 0;
		} else if (HP>MaxHP) HP=MaxHP;
	}
	printf("%lli",HP);
	return 0;
}
