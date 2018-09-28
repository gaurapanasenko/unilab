#include "superint.h"

using namespace std;

SuperInt::SuperInt() {
	a.resize(1,0); sign = 1;
}

SuperInt::SuperInt(const SuperInt& right) {
	*this = right;
}

SuperInt::SuperInt(const std::vector<int>& x) { a = x; }

SuperInt::SuperInt(const std::string& str) { *this = str; }

SuperInt::SuperInt(const char * str) { *this = str; }

SuperInt& SuperInt::operator=(const SuperInt& right) {
	a = right.a; sign = right.sign;
	return *this;
}

SuperInt& SuperInt::operator=(const std::vector<int>& right)
	{ a = right; return *this; }

SuperInt& SuperInt::operator=(const std::string& right) {
	parse_string(right);
	return *this;
}

SuperInt& SuperInt::operator=(const char * right) {
	parse_string(string(right));
	return *this;
}

SuperInt& SuperInt::operator()(const SuperInt& right) {
	*this = right;
	return *this;
}

SuperInt operator+(const SuperInt& left, const SuperInt& right) {
	SuperInt num;
	if (left.sign == 1 && right.sign == 1)
		num(left).summation(right.a);
	else if (left.sign == -1 && right.sign == 1
			&& left.compare_module(right) == 1)
		num(left).subtraction(right.a);
	else if (left.sign == -1 && right.sign == 1
			&& left.compare_module(right) == -1)
		num(right).subtraction(left.a);
	else if (left.sign == 1 && right.sign == -1
			&& left.compare_module(right) == 1)
		num(left).subtraction(right.a);
	else if (left.sign == 1 && right.sign == -1
			&& left.compare_module(right) == -1)
		num(right).subtraction(left.a);
	else if (left.sign == -1 && right.sign == -1)
		num(left).summation(right.a);
	//~ std::cout << (int)left.compare_module(right) << endl;
	return num;
}

SuperInt operator-(const SuperInt& left, const SuperInt& right) {
	return left + SuperInt("-1") * right;
	//~ std::vector<int> out;
	//~ const std::vector<int> * in;
	//~ bool x = left < right;
	//~ if (x) { out = right.a; in = &left.a; }
	//~ else { out = left.a; in = &right.a; }
	//~ return SuperInt(out);
}

SuperInt operator*(const SuperInt& left, const SuperInt& right) {
	const std::vector<int> * a = &left.a, * b = &right.a;
	long long base = SUPERINT_CELL_MAX;
	std::vector<int> c(a->size() + b->size(), 0);
	long long cur;
	for (size_t i = 0; i < a->size(); i++)
		for (int j = 0, carry = 0; j < (int)b->size() || carry; j++) {
			cur = c[i+j] + a->at(i) * 1ll * (j < (int)b->size() ? b->at(j) : 0)
				+ carry;
			c[i+j] = int (cur % base);
			carry = int (cur / base);
		}
	while (c.size() > 1 && c.back() == 0)
		c.pop_back();
	return SuperInt(c);
}

bool operator  <(const SuperInt& left, const SuperInt& right) {
	if (left.compare(right) == -1) return true;
	else return false;
}

bool operator  >(const SuperInt& left, const SuperInt& right) {
	if (left.compare(right) == 1) return true;
	else return false;
}

bool operator ==(const SuperInt& left, const SuperInt& right) {
	if (left.compare(right) == 0) return true;
	else return false;
}

bool operator <=(const SuperInt& left, const SuperInt& right) {
	char x = left.compare(right);
	if (x == -1 || x == 0) return true;
	else return false;
}

bool operator >=(const SuperInt& left, const SuperInt& right) {
	char x = left.compare(right);
	if (x == 1 || x == 0) return true;
	else return false;
}

std::ostream &operator<<(std::ostream &output, const SuperInt &right) {
	if (right.sign == -1) output << '-';
	output << (right.a.empty() ? 0 : right.a.back());
	for (int i = right.a.size() - 2; i >= 0; i--)
		output << std::setfill('0') << std::setw(9) << right.a[i];
	return output;
}

std::istream &operator>>(std::istream  &input, SuperInt &right) {
	std::string str;
	input >> str;
	right.parse_string(str);
	return input;
}

char SuperInt::compare(const SuperInt& right) const {
	if (sign == 1 && right.sign == -1) return 1;
	else if (sign == -1 && right.sign == 1) return -1;
	return compare_module(right) * sign;
}

char SuperInt::compare_module(const SuperInt& right) const {
	if (a.size() > right.a.size()) return 1;
	else if (a.size() < right.a.size()) return -1;
	for (size_t i = 0; i < a.size(); i++) {
		if (a[i] > right.a[i]) return 1;
		else if (a[i] < right.a[i]) return -1;
	}
	return 0;
}

bool SuperInt::parse_string(const std::string& str) {
	if (!validate(str)) return false;
	a.clear();
	if (str[0] == '-') sign = -1; else sign = 1;
	int x;
	if (sign == 1) x = 0; else x = 1;
	for (int i = (int)str.length(); i > x; i -= 9) {
		if (i < 9) a.push_back(atoi(str.substr(x, i - x).c_str()));
		else a.push_back(atoi(str.substr(i - 9, 9).c_str()));
	}
	while (a.size() > 1 && a.back() == 0) a.pop_back();
	return true;
}

bool SuperInt::validate(const std::string& str) {
	for (size_t i = 0; i < str.size(); i++)
		if ((i != 0 || str[i] != '-') && (str[i] < '0' || str[i] > '9'))
			return false;
	return true;
}

void SuperInt::summation(const std::vector<int>& b) {
	int carry = 0;
	for (size_t i = 0; i < std::max(a.size(), b.size()) || carry; i++) {
		if (i == a.size())
			a.push_back(0);
		a[i] += carry + (i < b.size() ? b[i] : 0);
		carry = a[i] >= SUPERINT_CELL_MAX;
		if (carry) a[i] -= SUPERINT_CELL_MAX;
	}
}
void SuperInt::subtraction(const std::vector<int>& b) {
	int carry = 0;
	for (size_t i = 0; i < b.size() || carry; i++) {
		a[i] -= carry + (i < b.size() ? b[i] : 0);
		carry = a[i] < 0;
		if (carry) a[i] += SUPERINT_CELL_MAX;
	}
	while (a.size() > 1 && a.back() == 0) a.pop_back();
}
