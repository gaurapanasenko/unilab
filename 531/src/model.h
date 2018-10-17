#ifndef MODEL_H
#define MODEL_H
#include <gtkmm.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iomanip>
#include <map>
//~ #include <gmpxx.h>
#include "MiniParser.hpp"

typedef long double real;
//~ typedef mpf_class big_real;

extern long double eps;

class Function {
public:
	Function(const char * str);
	real operator()(const real& x);
private:
	MiniParser mp;
};

class Polynomial {
public:
	Polynomial();
	Polynomial(const std::vector<real>& cof);
	Polynomial(const real& a);
	Polynomial(const real& a, const real& b);
	//~ Polynomial(const big_real& a);
	//~ Polynomial(const big_real& a, const big_real& b);
	Polynomial(const Polynomial& right);
	const std::vector<real>& get_coefficients() const;
	real operator()(const real& a);
	Polynomial& operator=(const Polynomial& right);
	Polynomial& operator=(const std::vector<real>& cof);
	friend Polynomial operator+(const Polynomial& left,
	                            const Polynomial& right);
	friend Polynomial operator-(const Polynomial& left,
	                            const Polynomial& right);
	friend Polynomial operator*(const Polynomial& left,
	                            const Polynomial& right);
	friend Polynomial operator/(const Polynomial& left,
	                            const real& right);
	friend std::ostream& operator<<(std::ostream& output,
	                                const Polynomial& right);
private:
	std::vector<real> cof;
};

class Model {
public:
	Model();
	~Model();
	bool update(const Glib::ustring& function,
	            const Glib::ustring& in_a,
	            const Glib::ustring& in_b,
	            const Glib::ustring& in_n);
	void resize(const int& h, const int& w);
	const std::vector<real>& get_cp();
	const real& get_a();
	const real& get_b();
	const  int& get_ox();
	const  int& get_oy();
	const real& get_min();
	const real& get_max();
	const real get_n();
	const real get_h();
	const std::map<real, real>& get_fx();
	const std::map<real, real>& get_Lx();
	const std::map<real, real>& get_Px();
	const std::map<real, real>& get_Fx();
	const real operator()(const char& f, const real& x);
	const real lagrange(const real& x);
	const real divided_differences(const size_t& i, const size_t& k);
	const real newton(const real& x);
	Glib::ustring generate_str();

private:
	void render();
	real a, b, min, max, dx, dy, ii, h;
	int height, width, ox, oy, n;
	std::vector<real> cp, Pfx;
	std::map<real, real> fx, Lx, Px, Fx;
	Glib::ustring fun_str;
	Function * fun;
	Polynomial lag, newt, fi_pol;
};

int gauss(std::vector< std::vector<real> > a,
		  std::vector<real> y, std::vector<real>& x);

#endif //MODEL_H
