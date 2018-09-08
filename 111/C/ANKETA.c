#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	char fname[256],sname[256],tname[256],gender[16],bday[32],
	address[256],phone[32],school[256],grade[256],bestdoing[256];
	int i;
	printf("Фамилия?= ");
	scanf("%s",sname);
	printf("Имя?= ");
	scanf("%s",fname);
	printf("Отчество?= ");
	scanf("%s",tname);
	printf("Пол?= ");
	scanf("%s",gender);
	printf("Дата рождения?= ");
	scanf("%s",bday);
	printf("Адрес?= ");
	scanf("%s",address);
	printf("Номер телефона?= ");
	scanf("%s",phone);
	printf("Место учебы?= ");
	scanf("%s",school);
	printf("Класс?= ");
	scanf("%s",grade);
	printf("Хобби?= ");
	scanf("%s",bestdoing);
	//~ printf("%u",(unsigned)strlen(sname));
	int fc[] = {7, (int) strlen(sname),3, (int) strlen(grade),11, (int) strlen(school)};
	int sc[] = {3, (int) strlen(fname),13, (int) strlen(bday),5, (int) strlen(grade)};
	int tc[] = {8, (int) strlen(tname),14, (int) strlen(phone)};
	int fcmax = 0;
	int scmax = 0;
	int tcmax = 0;
	for (i=0;i<7;i++) {
		if (fc[i] > fcmax)
			fcmax = fc[i];
	}
	for (i=0;i<5;i++) {
		if (sc[i] > scmax)
			scmax = sc[i];
	}
	for (i=0;i<5;i++) {
		if (tc[i] > tcmax)
			tcmax = tc[i];
	}
	fcmax += 5;scmax += 5;tcmax += 5;
	printf("%*sА Н К Е Т А\n", (fcmax + scmax + tcmax - 11) / 2, " ");
	printf("%s%*s%s%*s%s\n","Фамилия",fcmax-7,"","Имя",scmax-3,"","Отчество");
	printf("%s%*s%s%*s%s\n","-------",fcmax-7,"","---",scmax-3,"","--------");
	printf("%s%*s%s%*s%s\n\n",sname,fcmax-fc[1],"",fname,scmax-sc[1],"",tname);
	printf("%s%*s%s%*s%s\n","Пол",fcmax-3,"","Дата рождения",scmax-13,"","Номер телефона");
	printf("%s%*s%s%*s%s\n","---",fcmax-3,"","-------------",scmax-13,"","--------------");
	printf("%s%*s%s%*s%s\n\n",gender,fcmax-fc[3],"",bday,scmax-sc[3],"",phone);
	printf("%s\n","Адрес");
	printf("%s\n","-----");
	printf("%s\n\n",address);
	printf("%s%*s%s\n","Место учебы",fcmax-11,"","Класс");
	printf("%s%*s%s\n","-----------",fcmax-11,"","-----");
	printf("%s%*s%s\n\n",school,fcmax-fc[5],"",grade);
	printf("%s\n","Хобби");
	printf("%s\n","-----");
	printf("%s\n\n",bestdoing);
	return 0;
}
