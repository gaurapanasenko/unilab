#include <iostream>
#include <locale>
#include "superint.h"
#include "tests.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	return run_tests();
}
