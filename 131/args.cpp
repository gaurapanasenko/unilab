#include "args.h"

Args::Args(int argc_in, char **argv_in) {
	argc = argc_in;
	argv = argv_in;
	states = (char*)"hflgasm";
	set_state(0);
}

void Args::handle() {
	char * s;
	int len = strlen(states);
	for (int i = 0; i < argc; i++) {
		s = argv[i];
		if (!strcmp(s, "-h")) {
			printf("Usage: main [-h] [-f NUMBERS] [-l NUMBERS] [-g NUMBERS]\
 [-m NUMBERS] [-a NUMBER NUMBERS]\n");
			printf("  -h  print help\n");
			printf("  -f  fold array\n");
			printf("  -l  find least\n");
			printf("  -g  find greatest\n");
			printf("  -a  find the arithmetic mean\n");
			printf("  -s  add to items\n");
			printf("  -m  multiply items\n");
			printf("  --  stop iteration\n");
			printf("  NUMBER is one number\n");
			printf("  NUMBERS is array of number separated by space\n");
		}
		else if (!strcmp(s, "--")) set_state(0);
		else if (s[0] == '-' && s[2] == 0) {
			for (int i = 0; i < len; i++)
				if (s[1] == states[i]) {
					set_state(states[i]);
					break;
					continue;
				}
		} else if (state) {
			sscanf(s, "%f", &buf);
			arr.push_back(buf);
		}
	}
}

Args::~Args() {end_state();}

void Args::set_state(char st) {
	end_state();
	state = st;
}

void Args::end_state() {
	switch(state) {
		case 'f': fold_an_array(); break;
		case 'l': least(); break;
		case 'g': greatest(); break;
		case 'a': arithmetic_mean(); break;
		case 's': append_to_items(); break;
		case 'm': multiply(); break;
	}
	state = 0;
	arr.resize(0);
	buf = 0.0;
}

void Args::fold_an_array() {
	float sum = 0;
	for (int i = 0; i < arr.size(); i++)
		sum += arr[i];
	printf("Sum of array: %f\n", sum);
}

void Args::least() {
	if (arr.size() <= 0) return;
	float min = arr[0];
	for (int i = 1; i < arr.size(); i++)
		if (min > arr[i]) min = arr[i];
	printf("Minimal item of array is %g\n", min);
}

void Args::greatest() {
	if (arr.size() <= 0) return;
	float max = arr[0];
	for (int i = 1; i < arr.size(); i++)
		if (max < arr[i]) max = arr[i];
	printf("Maximal item of array is %g\n", max);
}

void Args::arithmetic_mean() {
	float sum = 0, avrg = 0;
	for (int i = 0; i < arr.size(); i++)
		sum += arr[i];
	avrg = sum/((float)arr.size());
	printf("Arithmetic mean of array: %g\n", avrg);
}

void Args::append_to_items() {
	if (arr.size() <= 0) return;
	float num = arr[0];
	printf("Appending to each item: ");
	for (int i = 1; i < arr.size(); i++)
		printf("%g ", arr[i] + num);
	printf("\n");
}

void Args::multiply() {
	if (arr.size() <= 0) return;
	float mul = arr[0];
	for (int i = 1; i < arr.size(); i++)
		mul *= arr[i];
	printf("Multiply items of array: %f\n", mul);
}
