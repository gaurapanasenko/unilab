#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
	double ax,ay,az,ux,uy,uz,bx,by,bz,vx,vy,vz,k,D,len,p,l;
	scanf("%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&ax,&ay,&az,&ux,&uy,&uz,&bx,&by,&bz,&vx,&vy,&vz);
	k = ux*vx+uy*vy+uz*vz;
	D = -vx*bx -vy*by -vz*bz;
	p = vx*ax+vy*ay+vz*az+D;
	if (k == 0 && p == 0) {
		if (p == 0) {
			len = sqrt(ux*ux + uy*uy + uz*uz);
			ux /= len; uy /= len; uz /= len;
			printf("5 %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf",ax,ay,az,ux,uy,uz);
		} else
			printf("0");
	} else if ((k < 0 && p > 0) || (k > 0 && p < 0)) {
		l = -p/(vx*ux+vy*uy+vz*uz);
		printf("1 %.8lf %.8lf %.8lf",ax+ux*l,ay+uy*l,az+uz*l);
	} else printf("0");
	return 0;
}
