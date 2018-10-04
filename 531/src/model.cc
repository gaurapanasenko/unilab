#include "model.h"

Function::Function(const char * str) : mp(str) {}

double Function::operator()(double x) {
	MiniParser::IDMap ids;
	ids["x"] = x;
	return mp.eval(ids);
}

Polynomial::Polynomial() : cof() {}

Polynomial::Polynomial(double a) : cof() {cof.push_back(a);}

Polynomial::Polynomial(double a, double b) : cof()
	{cof.push_back(a); cof.push_back(b);}

Polynomial::Polynomial(const Polynomial& right) {(*this) = right;}

const std::vector<double>& Polynomial::get_coefficients() const {return cof;}

//Polynomial& Polynomial::operator()(double a);

Polynomial& Polynomial::operator=(const Polynomial& right) {
	cof = right.cof; return *this;
}

Polynomial operator+(const Polynomial& left, const Polynomial& right) {
	Polynomial out;
	const Polynomial * in;
	if (left.cof.size() > right.cof.size()) {out = left; in = &right;}
	else {out = right; in = &left;}
	for (size_t i = 1; i <= in->cof.size(); i++) {
		out.cof[out.cof.size() - i] += in->cof[in->cof.size() - i];
	}
	return out;
}

//Polynomial operator-(const Polynomial& left,const Polynomial& right);

const Polynomial operator*(const Polynomial& left,
                                  const Polynomial& right) {
	Polynomial out;
	const Polynomial * l, * r;
	if (left.cof.size() > right.cof.size()) {l = &left; r = &right;}
	else {l = &right; r = &left;}
	out.cof.resize(left.cof.size() + right.cof.size() - 1, 0);
	for (size_t i = 0; i < l->cof.size(); i++)
		for (size_t j = 0; j < r->cof.size(); j++)
			out.cof[i + j] += l->cof[i] * r->cof[j];
	return out;
}


std::ostream &operator<<(std::ostream &output, const Polynomial &right) {
	size_t sz = right.cof.size();
	for (size_t i = 0; i < sz; i++) {
		output << right.cof[i];
		if (i + 2 == sz) output << "*x + ";
		else if (i + 1 != sz) output << "*x^" << sz - i - 1 << " + ";
	}
	return output;
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
		ss << "(" << (*this)('f', cp[i]);
		for (int j = 0; j < (int) cp.size(); j++)
			if (i != j)
				ss << "*(x-" << cp[j] << ")/(" << cp[i] << "-" << cp[j] << ")";
		ss << ")";
		if (i + 1 != (int) cp.size()) ss << " + ";
	}
	ss << "\n";
	Polynomial p, p1;
	double t1;
	for (int i = 0; i < (int) cp.size(); i++) {
		p1 = Polynomial(1);
		t1 = 1;
		for (int j = 0; j < (int) cp.size(); j++)
			if (i != j) {
				p1 = p1 * Polynomial(1, cp[j]);
				t1 *= (cp[i]-cp[j]);
			}
		//p = p + p1;
		p = p + p1 * Polynomial((*this)('f', cp[i]) / t1);
	}
	ss << p;
	str = ss.str();
	return str;
}
