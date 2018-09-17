#include <iostream>
#include "superint.h"

using namespace std;

int main() {
	SuperInt one, two;
	string str = "12345";
	SuperInt num(str);
	cout << str << endl;
	cout << num << endl;
	str = "1234567890123456789";
	num = str;
	cout << str << endl;
	cout << num << endl;
	//~ cin >> num;
	//~ cout << num << endl;
	one = string("7777777777777777777");
	two = string("8888888888888888888");
	cout << one << endl;
	cout << two << endl;
	cout << one + two << endl;
	cout << two - one << endl;
	cout << one - two << endl;
	cout << (two - one) * (two - one) << endl;
}
