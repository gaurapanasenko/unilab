#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int ox = rand() % 2000 - 1000, oy = rand() % 2000 - 1000, or = rand() % 20 + 50,
	isx = 0, isy = 0, ifm = 0, ism = 0, ord, dx = 0, dy = 0;
	ord = or * or;
	int frun = 1;
	while (1) {
		printf("Введите x, y через пробел и нажмите ENTER.\n");
		scanf("%i%i",&isx,&isy);
		dx = isx - ox;
		dy = isy - oy;
		ism = dx * dx + dy * dy;
		if (ism <= ord) {
			printf("Точка (%i,%i) попала в круг с центром в точке (%i,%i) радиуса %i\n",isx,isy,ox,oy,or);
			break;
		}
		if (frun) {
			printf("Попробуй ещё\n");
			frun = 0;
		} else {
			if (ism < ifm)
				printf("Теплее\n");
			else if (ism == ifm)
				printf("Одинаково\n");
			else if (ism > ifm)
				printf("Холоднее\n");
		}
		ifm = ism;
	}
	return 0;
}

