#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(int argc, char *argv[]) {
	char * input_filename, output_filename[12];
	FILE * input_file = NULL, * output_file = NULL;
	char ** text, ** outtext, ans;
	int i,lines;
	if (argc > 1) {
		input_filename = argv[1];
		input_file = fopen(input_filename, "r");
		if (input_file == NULL) {
			if (menu(stdin,stdout,"Вы задали неверное имя файла. Хотите ввести данные вручную?\n1. Да\n2.Нет",2) == 2)
				return 0;
			return 1;
		}
	}
	if (input_file == NULL)
		input_filename = get_inputfile(stdin,stdout,&input_file);
	text = get_text(input_file,&lines);
	outtext = sort_text(text,lines);
	ans = menu(stdin,stdout,"Вывести:\n1. На экран\n2. В файл\n3. Оба варианта",3);
	if (ans == 1 || ans == 3)
		for (i=0;i<lines;i++)
		printf("%s\n",outtext[i]);
	if (ans == 2 || ans == 3) {
		create_outputfile(stdin,stdout,input_filename,&output_file,output_filename);
		if (output_file == NULL) {
			printf("Не получилось создать файл %s\n",output_filename);
			return 1;
		}
		for (i=0;i<lines;i++) {
			fprintf(output_file,"%s\n",outtext[i]);
		}
		fclose(output_file);
		printf("Выходной файл: %s\n",output_filename);
	}

	// Exit
	fclose(input_file);
	free(input_filename);
	for (i=0; i<lines; i++) {
		free(text[i]);
	}
	free(text);
	return 0;
}
