#ifndef SUPERINT_H
#define SUPERINT_H
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <limits.h>
#include <iomanip>
#include <algorithm>

#define SUPERINT_CELL_MAX 1000000000

using namespace std;

class SuperInt {
private:
	std::vector<int> a;
	char sign;

public:
	SuperInt();
	SuperInt(const SuperInt& x);
	SuperInt(const std::vector<int>& x);
	SuperInt(const std::string& str);
	SuperInt(const char * str);
	SuperInt& operator=(const SuperInt& right);
	SuperInt& operator=(const std::vector<int>& right);
	SuperInt& operator=(const std::string& right);
	SuperInt& operator=(const char * str);
	SuperInt& operator()(const SuperInt& right);
	friend SuperInt operator+(const SuperInt& left, const SuperInt& right);
	friend SuperInt operator-(const SuperInt& left, const SuperInt& right);
	friend SuperInt operator*(const SuperInt& left, const SuperInt& right);
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
	void summation(const std::vector<int>& b);
	void subtraction(const std::vector<int>& b);
};

#endif
