#include "args.h"
#include "functions.h"

using namespace std;

int main(int argc, char *argv[]) {
	Args args(argc, argv);
	args.handle();
	std::vector<std::vector<float>> arr, vbuf(1);
	float buf;
	char ch;
	for (int i = 0; i < args.count_commands(); i++) {
		args.get_new_command(&ch, arr);
		switch(ch) {
			case 'h':
				printf("Usage: main [-h] [-f NUMBERS] [-l NUMBERS] [-g NUMBERS]\
 [-a NUMBERS]  [-s NUMBER NUMBERS] [-m NUMBERS] [-p NUMBERS] [-o NUMBERS]\
 [-k NUMBERS -- NUMBERS ...]\n");
				printf("  -h  print help\n");
				printf("  -f  fold array\n");
				printf("  -l  find least\n");
				printf("  -g  find greatest\n");
				printf("  -a  find the arithmetic mean\n");
				printf("  -s  add to items\n");
				printf("  -m  multiply items\n");
				printf("  -p  same as -f -l -g -a -m\n");
				printf("  -o  separate on positive and negative\n");
				printf("  -k  add two array separated by --\n");
				printf("  --  new array\n");
				printf("  NUMBER is one number\n");
				printf("  NUMBERS is array of number separated by space\n");
				break;
			case 'f':
				buf = fold_an_array(arr[0]);
				show_array(arr);
				printf("Sum of array: %f\n\n", buf);
				break;
			case 'l':
				buf = least(arr[0]);
				show_array(arr);
				printf("Minimal item of array is %g\n\n", buf);
				break;
			case 'g':
				buf = greatest(arr[0]);
				show_array(arr);
				printf("Maximal item of array is %g\n\n", buf);
				break;
			case 'a':
				buf = arithmetic_mean(arr[0]);
				show_array(arr);
				printf("Arithmetic mean of array: %g\n\n", buf);
				break;
			case 's':
				vbuf[0] = append_to_items(arr[0]);
				show_array(arr);
				printf("Appending first to next items: ");
				for (int i = 1; i < vbuf[0].size(); i++)
					printf("%g ", vbuf[0][i]);
				printf("\n\n");
				break;
			case 'm':
				buf = multiply(arr[0]);
				show_array(arr);
				printf("Multiply items of array: %f\n\n", buf);
				break;
			case 'p':
				vbuf[0] = super_function(arr[0]);
				show_array(arr);
				printf("Sum of array: %f\n", vbuf[0]);
				printf("Minimal item of array is %g\n", vbuf[0][1]);
				printf("Maximal item of array is %g\n", vbuf[0][2]);
				printf("Arithmetic mean of array: %g\n", vbuf[0][3]);
				printf("Multiply items of array: %f\n\n", vbuf[0][4]);
				break;
			case 'o':
				vbuf = separate_numbers(arr[0]);
				show_array(arr);
				printf("Positive: \n");
				for (int i = 0; i < vbuf[0].size(); i++)
					printf("%g ", vbuf[0][i]);
				printf("\n");
				printf("Negative: \n");
				for (int i = 0; i < vbuf[1].size(); i++)
					printf("%g ", vbuf[1][i]);
				printf("\n\n");
				break;
			case 'k':
				vbuf[0] = add_arrays(arr);
				show_array(arr);
				printf("Adding arrays: \n");
				for (int i = 0; i < vbuf[0].size(); i++)
					printf("%g ", vbuf[0][i]);
				printf("\n\n");
				break;
		}
	}
}
