#include "args.h"

int main(int argc, char *argv[]) {
	Args args(argc, argv);
	args.handle();
}
