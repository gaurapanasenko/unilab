#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include <ctype.h>

void say (FILE * pFile, char * text) {
	fprintf(pFile,"%s",text);
}

char * get_string (FILE * input) {
	char *a = (char *) calloc(1,sizeof(char)),c = 0;
	int length = 0;
	// While not end of line ask new character
	while ((c = fgetc(input)) != EOF && c != '\n') {
		// Increase length
		length++;
		// Increase length of input string
		a = (char *) realloc (a, (length + 1) * sizeof(char));
		// Copy character to string
		a[length-1] = c;
		// Get new character
	}
	a[length] = 0;
	return a;
}

char * get_inputfile (FILE * input, FILE * output, FILE ** input_file) {
	char * input_filename;
	while (*input_file == NULL) {
		// Ask string
		say(output,"Введите имя файла: ");
		input_filename = get_string(input);
		*input_file = fopen(input_filename, "r");
		if (*input_file == NULL) {
			system("clear");
			say(output,"Такого файла не существует. Попробуйте ещё\n");
		}
	}
	return input_filename;
}

void create_outputfile (FILE * input, FILE * output, char * input_filename, FILE ** output_file, char * output_filename) {
	char i;
	for (i=0; i<3 && input_filename[i] != '.'; i++) {
		output_filename[i] = input_filename[i];
	}
	output_filename[i]=0;
	strcat(output_filename,"_out.dat");
	*output_file = fopen(output_filename, "w");
}

char ** get_text (FILE * input, int * lines) {
	char ** text = (char **) calloc(0,sizeof(char*));
	int length = 0;
	while (!feof(input)) {
		// Increase length
		length++;
		// Increase length of input string
		text = (char **) realloc (text, (length + 1) * sizeof(char*));
		// Copy character to string
		text[length-1] = get_string(input);
	}
	*lines = length;
	return text;
}

char ** sort_text(char ** text, int lines) {
	char ** outtext = (char **) calloc(lines,sizeof(char*));
	int x=0,i;
	for (i=0; i<lines; i++)
		if (check_line(text[i])) {
			outtext[x]=text[i];
			x++;
		}
	for (i=0; i<lines; i++)
		if (!check_line(text[i])) {
			outtext[x]=text[i];
			x++;
		}
	return outtext;
}

char check_line(char * text) {
	if (isalpha(text[0]) && !isalpha(text[1]) && text[1]!='-' && text[1]!='\'')
		return 1;
	else
		for (int i = 1; text[i] && !isalpha(text[i-1]); i++)
			if (isalpha(text[i]) && !isalpha(text[i+1]) && text[i+1]!='-' && text[i+1]!='\'')
				return 1;
	return 0;
}

/* Ignore all data until end of line */
void ign_other(FILE * input) {
	char c = 0;
	while ((c = fgetc(input)) != EOF && c != '\n') {}
}


char menu (FILE * input, FILE * output, char * message, char count) {
	// Main menu
	char answer = 0;
	// While answer is incorrect make menu
	while (answer < 1 || answer > count) {
		// Ask menu
		printf("%s\n",message);
		// Get answer
		answer = getchar()-'0';
		// Ignore all after answer to avoid incorrect data in future
		ign_other(input);
		// ask to try again
		if (answer < 1 || answer > count) {
			system("clear");
			printf("Вводить нужно только цифры от 1 до %hhi. Попробуйте ещё\n",count);
		}
	}
	return answer;
}
