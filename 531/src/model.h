#ifndef MODEL_H
#define MODEL_H
#include <gtkmm.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <stdlib.h>
#include "MiniParser.hpp"

class Function {
public:
    Function(const char * str);
    double operator()(double x);
private:
    MiniParser mp;
};

class Model {
public:
	Model();
	void update(Glib::ustring function, Glib::ustring in_a, Glib::ustring in_b,
	            Glib::ustring control_points);
	const std::vector<double>& get_cp();
	double get_a();
	double get_b();
	double operator()(char f, double x);
	double lagrange(double x);
	Glib::ustring lagrange_str();
	
private:
	double a, b;
	std::vector<double> cp;
	Function * fun;
};

#endif //MODEL_H