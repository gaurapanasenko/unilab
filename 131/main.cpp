#include "args.h"

using namespace std;

int main(int argc, char *argv[]) {
	Args args(argc, argv);
	args.handle();
	vector<float> arr, vbuf;
	float buf;
	char ch;
	for (int i = 0; i < args.count_commands(); i++) {
		args.get_new_command(&ch, arr);
		switch(ch) {
			case 'h':
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
			case 'f':
				buf = fold_an_array(arr);
				printf("Sum of array: %f\n", buf);
				break;
			case 'l':
				buf = least(arr);
				printf("Minimal item of array is %g\n", buf);
				break;
			case 'g':
				buf = greatest(arr);
				printf("Maximal item of array is %g\n", buf);
				break;
			case 'a':
				buf = arithmetic_mean(arr);
				printf("Arithmetic mean of array: %g\n", buf);
				break;
			case 's':
				vbuf = append_to_items(arr);
				printf("Appending to each item: ");
				for (int i = 1; i < vbuf.size(); i++)
					printf("%g ", vbuf[i]);
				printf("\n");
				break;
			case 'm':
				buf = multiply(arr);
				printf("Multiply items of array: %f\n", buf);
				break;
		}
	}
}
