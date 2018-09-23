#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define NUM_SIZE 8
#include "stdio.h"
#include "stdlib.h"

void ign_other(FILE * input);

int get_number(FILE * input, int * number);

int menu (FILE * input, FILE * output, char * message, int count);

int rnd(int min, int max);

int * generate_array(int n, int min, int max);

void print_array(FILE * input, int * a, int n);

int summarize(int * a, int n);

void multiply_elem(int x, int * a, int n);

void split(int * a, int n, int *  b, int *bn, int *  c, int * cn);

int * summarize_arrays(int * a, int * b, int n);

#endif
