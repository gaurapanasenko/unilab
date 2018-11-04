#include "superint.h"

using namespace std;

SuperInt::interval::interval(SuperInt& number, const size_t& begin,
	const size_t& length) : number(number), begin(begin), length(length) {}

SuperInt& SuperInt::interval::operator=(const SuperInt& right) {
	size_t a = (begin + 8) / 9, end = begin + length;
	size_t b = end / 9;
	vector<int> ra = right.a;
	if (b >= number.a.size() || b >= ra.size()) {
		if (number.a.size() > ra.size())
			ra.resize(number.a.size());
		if (number.a.size() < ra.size())
			number.a.resize(ra.size());
		if (b >= number.a.size()) {
			b = number.a.size();
			end = 9 * b - 1;
		}
	}
	for (size_t i = a; i < b; i++)
		number.a[i] = ra[i];
	int aa = begin % 9, bb = (end - 1) % 9 + 1,
		ap = pow(10, aa), bp = pow(10, bb);
	if (a == b + 1 && aa != 0 && bb != 9) {
		number.a[b] -= int(int(number.a[b] % bp) / ap) * ap;
		number.a[b] += int(int(ra[b] % bp) / ap) * ap;
	}
	else {
		if (aa != 0) {
			number.a[a - 1] = number.a[a - 1] % ap;
			number.a[a - 1] += int(ra[a - 1] / ap) * ap;
		}
		if (bb != 0) {
			number.a[b] = int(number.a[b] / bp) * bp;
			number.a[b] += ra[b] % bp;
		}
	}
	number.clear_leading_zeros();
	return number;
}

SuperInt::SuperInt() {
	a.resize(1,0); sign = 1;
}

SuperInt::SuperInt(const SuperInt& right) { *this = right; }
SuperInt::SuperInt(const std::string& right) { *this = right; }
SuperInt::SuperInt(const char * right) { *this = right; }
SuperInt::SuperInt(const int& right) { *this = right; }

SuperInt& SuperInt::operator=(const SuperInt& right) {
	a = right.a; sign = right.sign;
	return *this;
}

SuperInt& SuperInt::operator=(const std::string& right) {
	parse_string(right);
	return *this;
}

SuperInt& SuperInt::operator=(const char * right) {
	parse_string(string(right));
	return *this;
}

SuperInt& SuperInt::operator=(const int& right) {
	sign = (right < 0) ? -1 : 1;
	a.clear();
	a.push_back(right);
	return *this;
}

SuperInt& SuperInt::operator()(const SuperInt& right) {
	*this = right;
	return *this;
}

SuperInt::interval SuperInt::operator()(const size_t& begin,
		const size_t& length) {
	//limit = 1; this->begin = begin; this->length = length;
	return SuperInt::interval(*this, begin, length);
}

SuperInt& SuperInt::operator()(const SuperInt& right, const size_t& begin,
							   const size_t& length) {
	(*this)(begin,length) = right;
	return *this;
}

const string SuperInt::get_string() {
	stringstream ss;
	ss << *this;
	return ss.str();
}

SuperInt operator+(const SuperInt& left, const SuperInt& right) {
	return left.operator_sum_sub(1, right);
}

SuperInt operator-(const SuperInt& left, const SuperInt& right) {
	return left.operator_sum_sub(-1, right);
}

SuperInt operator*(const SuperInt& left, const SuperInt& right) {
	SuperInt num;
	num.sign = left.sign * right.sign;
	const std::vector<int> * a = &left.a, * b = &right.a;
	std::vector<int> * c = &num.a;
	long long base = SUPERINT_CELL_MAX;
	c->resize(a->size() + b->size(), 0);
	long long cur;
	for (size_t i = 0; i < a->size(); i++)
		for (int j = 0, carry = 0; j < (int)b->size() || carry; j++) {
			cur = c->at(i+j) + a->at(i) * 1ll
				* (j < (int)b->size() ? b->at(j) : 0) + carry;
			c->at(i+j) = int (cur % base);
			carry = int (cur / base);
		}
	num.clear_leading_zeros();
	return num;
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
	for (int i = (int)a.size() - 1; i >= 0; i--) {
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
	if (sign == -1) x = 1; else x = 0;
	for (int i = (int)str.length(); i > x; i -= 9) {
		if (i - x < 9) a.push_back(atoi(str.substr(x, i - x).c_str()));
		else a.push_back(atoi(str.substr(i - 9, 9).c_str()));
	}
	clear_leading_zeros();
	return true;
}

bool SuperInt::validate(const std::string& str) {
	for (size_t i = 0; i < str.size(); i++)
		if ((i != 0 || str[i] != '-') && (str[i] < '0' || str[i] > '9'))
			return false;
	return true;
}

SuperInt SuperInt::operator_sum_sub(char s, const SuperInt& right) const {
	SuperInt num;
	if (sign * right.sign * s == 1)
		num(*this).summation(right.a);
	else if (compare_module(right) == -1)
		num(right).subtraction(a);
	else num(*this).subtraction(right.a);
	return num;
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
	clear_leading_zeros();
}

void SuperInt::clear_leading_zeros() {
	while (a.size() > 1 && a.back() == 0) a.pop_back();
}
