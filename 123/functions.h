// This is start of the header guard
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void say(FILE * output, char * text);

char * get_string (FILE * input);

char * get_inputfile (FILE * input, FILE * output, FILE ** input_file);

void create_outputfile (FILE * input, FILE * output, char * input_filename, FILE ** output_file, char * output_filename);

char ** get_text (FILE * input, int * lines);

char ** sort_text(char ** text, int lines);

char check_line(char * text);

void ign_other(FILE * input);

char menu (FILE * input, FILE * output, char * message, char count);

// This is the end of the header guard
#endif
