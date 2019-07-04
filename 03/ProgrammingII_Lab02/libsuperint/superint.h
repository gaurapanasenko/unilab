#ifndef SUPERINT_H
#define SUPERINT_H
#include <ostream>
#include <string>
#include <vector>

#define SUPERINT_CELL_MAX 1000000000

using namespace std;

namespace SuperInt {

class interval;
class position;
class const_position;

class SuperInt {
private:
	vector<int> a;
	char sign;

public:
	SuperInt();
	SuperInt(const SuperInt& x);
	SuperInt(const string& str);
	SuperInt(const char * str);
	SuperInt(const int& right);

	SuperInt& operator=(const SuperInt& right);
	SuperInt& operator=(const string& right);
	SuperInt& operator=(const char * str);
	SuperInt& operator=(const int& right);

	position operator[](const size_t& digit);
	const_position operator[](const size_t& digit) const;

	SuperInt& operator()(const SuperInt& right);
	interval  operator()(const size_t& begin, const size_t& length);
	SuperInt& operator()(const SuperInt& right, const size_t& begin,
						 const size_t& length);

	const string get_string();
	int get_digit(const size_t& digit);
	int get_digit(const size_t& digit) const;
	SuperInt& set_digit(const size_t& digit, const int& number);

	friend SuperInt operator+(const SuperInt& left, const SuperInt& right);
	friend SuperInt operator-(const SuperInt& left, const SuperInt& right);
	friend SuperInt operator*(const SuperInt& left, const SuperInt& right);
	friend SuperInt operator/(const SuperInt& left, const int& right);
	friend SuperInt operator/(const SuperInt& left, const SuperInt& right);

	friend bool operator  <(const SuperInt& left, const SuperInt& right);
	friend bool operator  >(const SuperInt& left, const SuperInt& right);
	friend bool operator ==(const SuperInt& left, const SuperInt& right);
	friend bool operator !=(const SuperInt& left, const SuperInt& right);
	friend bool operator <=(const SuperInt& left, const SuperInt& right);
	friend bool operator >=(const SuperInt& left, const SuperInt& right);

	friend ostream& operator<<(ostream& output,const SuperInt& right);
	friend istream& operator>>(istream& input, SuperInt& right);

private:
	char compare(const SuperInt& right) const;
	char compare_module(const SuperInt& right) const;
	bool parse_string(const string& str);
	bool validate(const string& str);
	SuperInt operator_sum_sub(char s, const SuperInt& right) const;
	void summation(const vector<int>& b);
	void subtraction(const vector<int>& b);
	void clear_leading_zeros();
};

class position {
private:
	SuperInt& number;
	size_t digit;
public:
	position(SuperInt& number, const size_t& digit)
		: number(number), digit(digit) {}

	SuperInt& operator=(const SuperInt& right);
	SuperInt& operator=(const position& right);
	SuperInt& operator=(const const_position& right);
	SuperInt& operator=(const int& right)
		{return number.set_digit(digit, right);}

	int operator*() {return number.get_digit(digit);}
	int operator*() const {return number.get_digit(digit);}

	friend const_position;
	friend ostream& operator<<(ostream& output,
		const position& right);
	friend istream& operator>>(istream& input, position& right);
};

class const_position {
private:
	const SuperInt& number;
	size_t digit;
public:
	const_position(const SuperInt& number, const size_t& digit)
		: number(number), digit(digit) {}
	const_position(const position& right)
		: number(right.number), digit(right.digit) {}

	int operator*() {return number.get_digit(digit);}
	int operator*() const {return number.get_digit(digit);}

	friend ostream& operator<<(ostream& output,
		const const_position& right);
};

class interval {
private:
	SuperInt& number;
	size_t begin, length;
public:
	interval(SuperInt& number, const size_t& begin, const size_t& length)
		: number(number), begin(begin), length(length) {};

	SuperInt& operator=(const SuperInt& right);

	SuperInt operator*();
	SuperInt operator*() const;

	friend ostream &operator<<(ostream& output,
		const interval& right);
	friend istream &operator>>(istream& input, interval& right);
};

}

#endif
