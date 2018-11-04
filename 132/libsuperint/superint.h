#ifndef SUPERINT_H
#define SUPERINT_H
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <limits.h>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <math.h>

#define SUPERINT_CELL_MAX 1000000000

using namespace std;

class SuperInt {
private:
	std::vector<int> a;
	char sign;

public:
	class interval {
		private:
			SuperInt& number;
			size_t begin, length;
		public:
			interval(SuperInt& number, const size_t& begin,
				const size_t& length);
			SuperInt& operator=(const SuperInt& right);
	};
	SuperInt();
	SuperInt(const SuperInt& x);
	SuperInt(const std::string& str);
	SuperInt(const char * str);
	SuperInt(const int& right);
	SuperInt& operator=(const SuperInt& right);
	SuperInt& operator=(const std::string& right);
	SuperInt& operator=(const char * str);
	SuperInt& operator=(const int& right);
	SuperInt& operator()(const SuperInt& right);
	interval operator()(const size_t& begin, const size_t& length);
	SuperInt& operator()(const SuperInt& right, const size_t& begin,
						 const size_t& length);
	const string get_string();
	friend SuperInt operator+(const SuperInt& left, const SuperInt& right);
	friend SuperInt operator-(const SuperInt& left, const SuperInt& right);
	friend SuperInt operator*(const SuperInt& left, const SuperInt& right);
	//~ friend SuperInt operator/(const SuperInt& left, const int& right);
	//~ friend SuperInt operator/(const SuperInt& left, const SuperInt& right);
	friend bool operator  <(const SuperInt& left, const SuperInt& right);
	friend bool operator  >(const SuperInt& left, const SuperInt& right);
	friend bool operator ==(const SuperInt& left, const SuperInt& right);
	friend bool operator <=(const SuperInt& left, const SuperInt& right);
	friend bool operator >=(const SuperInt& left, const SuperInt& right);
	friend std::ostream &operator<<(std::ostream &output,const SuperInt &right);
	friend std::istream &operator>>(std::istream  &input, SuperInt &right);

private:
	char compare(const SuperInt& right) const;
	char compare_module(const SuperInt& right) const;
	bool parse_string(const std::string& str);
	bool validate(const std::string& str);
	SuperInt operator_sum_sub(char s, const SuperInt& right) const;
	void summation(const std::vector<int>& b);
	void subtraction(const std::vector<int>& b);
	void clear_leading_zeros();
};


SuperInt operator+(const SuperInt& left, const SuperInt& right);
SuperInt operator-(const SuperInt& left, const SuperInt& right);
SuperInt operator*(const SuperInt& left, const SuperInt& right);
bool operator  <(const SuperInt& left, const SuperInt& right);
bool operator  >(const SuperInt& left, const SuperInt& right);
bool operator ==(const SuperInt& left, const SuperInt& right);
bool operator <=(const SuperInt& left, const SuperInt& right);
bool operator >=(const SuperInt& left, const SuperInt& right);
std::ostream &operator<<(std::ostream &output,const SuperInt &right);
std::istream &operator>>(std::istream  &input, SuperInt &right);

#endif
