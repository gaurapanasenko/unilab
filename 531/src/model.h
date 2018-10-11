#ifndef MODEL_H
#define MODEL_H
#include <gtkmm.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include "MiniParser.hpp"

class Function {
public:
    Function(const char * str);
    double operator()(double x);
private:
    MiniParser mp;
};

class Polynomial {
public:
	Polynomial();
	Polynomial(const double& a);
	Polynomial(double a, double b);
	Polynomial(const Polynomial& right);
	const std::vector<double>& get_coefficients() const;
	Polynomial& operator()(double a);
	Polynomial& operator=(const Polynomial& right);
	friend Polynomial operator+(const Polynomial& left,const Polynomial& right);
	friend Polynomial operator-(const Polynomial& left,const Polynomial& right);
	friend const Polynomial operator*(const Polynomial& left,
	                                  const Polynomial& right);
	friend std::ostream &operator<<(std::ostream &output,
	                                const Polynomial &right);
private:
	std::vector<double> cof;
};

class Model {
public:
	Model();
	void update(const Glib::ustring& function,
	            const Glib::ustring& in_a,
	            const Glib::ustring& in_b,
	            const Glib::ustring& in_n);
	void resize(const int& h, const int& w);
	const std::vector<double>& get_cp();
	const double& get_a();
	const double& get_b();
	const    int& get_ox();
	const    int& get_oy();
	const double& get_min();
	const double& get_max();
	const double get_n();
	const double get_h();
	const std::map<double, double>& get_fx();
	const std::map<double, double>& get_lx();
	const double operator()(const char& f, const double& x);
	const double lagrange(const double& x);
	Glib::ustring generate_str();

private:
	void render();
	double a, b, min, max, dx, dy, ii, h;
	int height, width, ox, oy, n;
	std::vector<double> cp;
	std::map<double, double> fx, lx;
	Glib::ustring fun_str;
	Function * fun;
};

#endif //MODEL_H
