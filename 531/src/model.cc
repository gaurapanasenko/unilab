#include "model.h"

Function::Function(const char * str) : mp(str) {}

double Function::operator()(double x) {
	MiniParser::IDMap ids;
	ids["x"] = x;
	return mp.eval(ids);
}

Model::Model() : cp(0) {fun = new Function("");
	a = 0; b = 0;}

void Model::update(Glib::ustring function, Glib::ustring in_a,
                   Glib::ustring in_b, Glib::ustring control_points) {
	char * end;
	double tmp;
	a = strtod(in_a.c_str(), &end); b = strtod(in_b.c_str(), &end);
	delete fun;
	fun = new Function(function.c_str());
	std::stringstream ss;
	ss << control_points;
	cp.clear();
	while (!ss.eof()) {
		ss >> tmp;
		cp.push_back(tmp);
	}
}

const std::vector<double>& Model::get_cp() {return cp;}
double Model::get_a() {return a;}
double Model::get_b() {return b;}
double Model::operator()(char f, double x) {
	switch(f) {
	case 'f':
		return (*fun)(x);
		break;
	case 'L':
		return lagrange(x);
		break;
	}
	return 0;
}

double Model::lagrange(double x) {
	long double xx = x, l = 0, p;
	for (int i = 0; i < (int) cp.size(); i++) {
		p = 1;
		for (int j = 0; j < (int) cp.size(); j++)
			if (i != j)
				p = p * (xx - cp[j]) / (cp[i] - cp[j]);
		l = l + (*this)('f', cp[i]) * p;
	}
	return l;
}


Glib::ustring Model::lagrange_str() {
	std::stringstream ss;
	Glib::ustring str;
	ss << "L(x) = ";
	for (int i = 0; i < (int) cp.size(); i++) {
		ss << (*this)('f', cp[i]) << "*";
		for (int j = 0; j < (int) cp.size(); j++)
			if (i != j)
				ss << "(x-" << cp[j] << ")/(" << cp[i] << "-" << cp[j] << ")";
		if (i + 1 != (int) cp.size()) ss << " + ";
	}
	str = ss.str();
	return str;
}
