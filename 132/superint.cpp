#include "superint.h"

SuperInt::SuperInt() {
	a.resize(1,0);
}

SuperInt::SuperInt(const SuperInt& right) {
	*this = right;
}

SuperInt::SuperInt(const std::vector<int>& x) { a = x; }

SuperInt::SuperInt(const std::string& str) {
	parse_string(str);
}

SuperInt& SuperInt::operator=(const SuperInt& right) {
	a = std::vector<int>(right.a);
	return *this;
}

SuperInt& SuperInt::operator=(const std::vector<int>& right)
	{ a = right; return *this; }

SuperInt& SuperInt::operator=(const std::string& right) {
	parse_string(right);
	return *this;
}

SuperInt operator+(const SuperInt& left, const SuperInt& right) {
	std::vector<int> out = left.a;
	int carry = 0;
	for (size_t i = 0; i < std::max(out.size(), right.a.size()) || carry; i++) {
		if (i == out.size())
			out.push_back(0);
		out[i] += carry + (i < right.a.size() ? right.a[i] : 0);
		carry = out[i] >= SUPERINT_CELL_MAX;
		if (carry) out[i] -= SUPERINT_CELL_MAX;
	}
	return SuperInt(out);
}

SuperInt operator-(const SuperInt& left, const SuperInt& right) {
	std::vector<int> out;
	const std::vector<int> * in;
	bool x = left < right;
	if (x) { out = right.a; in = &left.a; }
	else { out = left.a; in = &right.a; }
	int carry = 0;
	for (size_t i = 0; i < in->size() || carry; i++) {
		out[i] -= carry + (i < in->size() ? in->at(i) : 0);
		carry = out[i] < 0;
		if (carry) out[i] += SUPERINT_CELL_MAX;
	}
	while (out.size() > 1 && out.back() == 0) out.pop_back();
	if (x && !out.empty()) out.back() *= -1;
	return SuperInt(out);
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

void SuperInt::parse_string(const std::string& str) {
	a.clear();
	for (int i = (int)str.length(); i > 0; i -= 9)
		if (i < 9) a.push_back(atoi(str.substr(0, i).c_str()));
		else a.push_back(atoi(str.substr(i - 9, 9).c_str()));
	while (a.size() > 1 && a.back() == 0) a.pop_back();
}

char SuperInt::compare(const SuperInt& right) const {
	if (a.size() > right.a.size()) return 1;
	else if (a.size() < right.a.size()) return -1;
	for (size_t i = 0; i < a.size(); i++) {
		if (a[i] > right.a[i]) return 1;
		else if (a[i] < right.a[i]) return -1;
	}
	return 0;
}
