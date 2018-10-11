#include "model.h"

Function::Function(const char * str) : mp(str) {}

double Function::operator()(double x) {
	MiniParser::IDMap ids;
	ids["x"] = x;
	return mp.eval(ids);
}

Polynomial::Polynomial() : cof() {}

Polynomial::Polynomial(const double& a) : cof() {cof.push_back(a);}

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
Model::Model() : cp(0) {
	fun_str = "x";
	fun = new Function(fun_str.c_str());
	a = 0, b = 0, min = 0, max = 0, dx = 0, dy = 0;
	height = 0, width = 0, ox = 0, oy = 0;
}

void Model::update(const Glib::ustring& function,
                   const Glib::ustring& in_a,
                   const Glib::ustring& in_b,
                   const Glib::ustring& in_n) {
	char * end;
	double tmp, aa = strtod(in_a.c_str(), &end),
		bb = strtod(in_b.c_str(), &end),
		nn = strtod(in_n.c_str(), &end);
	if (aa == bb) bb += 1;
	else if (aa > bb) {tmp = aa; aa = bb; bb = tmp;}
	if (fun_str != function || a != aa || b != bb || n != nn) {
		fun_str = function; a = aa; b = bb; n = nn;
		delete fun;
		fun = new Function(fun_str.c_str());
		//~ std::stringstream ss;
		//~ cp_str = control_points;
		//~ ss << cp_str;
		//~ cp.clear();
		//~ while (!ss.eof()) {
			//~ ss >> tmp;
			//~ cp.push_back(tmp);
		//~ }
		fx.clear(); lx.clear(); cp.clear();
		render();
	}
}

void Model::resize(const int& h, const int& w) {
	if (height != h || width != w) {
		height = h; width = w;
		render();
	}
}

const std::vector<double>& Model::get_cp() {return cp;}
const double& Model::get_a()   {return a;}
const double& Model::get_b()   {return b;}
const    int& Model::get_ox()  {return ox;}
const    int& Model::get_oy()  {return oy;}
const double& Model::get_min() {return min;}
const double& Model::get_max() {return max;}
const double Model::get_n() {return n;}
const double Model::get_h() {return h;}
const std::map<double, double>& Model::get_fx() {return fx;}
const std::map<double, double>& Model::get_lx() {return lx;}
const double Model::operator()(const char& f, const double& x) {
	std::map<double,double>::iterator it;
	double tmp;
	switch(f) {
		case 'f':
			it = fx.find(x);
			if (it != fx.end())
				return (*it).second;
			tmp = (*fun)(x);
			fx[x] = tmp;
			return tmp;
			break;
		case 'L':
			//~ it = lx.find(x);
			//~ if (it != lx.end())
				//~ return (*it).second;
			tmp = lagrange(x);
			//~ lx[x] = tmp;
			return tmp;
			break;
		case 'X':
			return (x - a) * dx + width * 0.05;
			break;
		case 'Y':
			return (x - max) * dy + height * 0.1;
			break;
	}
	return 0;
}

const double Model::lagrange(const double& x) {
	long double xx = x, l = 0, p;
	for (size_t i = 0; i < cp.size(); i++) {
		p = 1;
		for (size_t j = 0; j < cp.size(); j++)
			if (i != j)
				p = p * (xx - cp[j]) / (cp[i] - cp[j]);
		l = l + (*this)('f', cp[i]) * p;
	}
	return l;
}


Glib::ustring Model::generate_str() {
	std::stringstream ss;
	double tmp;
	ss << "Відрізок [a, b] = [" << a << ", " << b
		<< "]\nКількіть точок та шаг:\nn = "
		<< n << ", h = " << (b - a) / n
		<< "\nf(x) - значення фунцкії\n"
		<< "L(x) - значення інтерполяційного\n"
		<< "       многочлена у формі Лагранжа\n"
		<< "P(x) - значення інтерполяційного\n"
		<< "       многочлена у формі Ньютона\n"
		<< "Ф(x) - значення многочлена найкращого\n"
		<< "       середньоквадратичного наближення\n";
	return ss.str();
	//~ ss << "L(x) = ";
	//~ for (int i = 0; i < (int) cp.size(); i++) {
		//~ ss << "(" << (*this)('f', cp[i]);
		//~ for (int j = 0; j < (int) cp.size(); j++)
			//~ if (i != j)
				//~ ss << "*(x-" << cp[j] << ")/(" << cp[i] << "-" << cp[j] << ")";
		//~ ss << ")";
		//~ if (i + 1 != (int) cp.size()) ss << " + ";
	//~ }
	//~ ss << "\n";
	//~ Polynomial p, p1;
	//~ double t1;
	//~ for (int i = 0; i < (int) cp.size(); i++) {
		//~ p1 = Polynomial(1);
		//~ t1 = 1;
		//~ for (int j = 0; j < (int) cp.size(); j++)
			//~ if (i != j) {
				//~ p1 = p1 * Polynomial(1, -cp[j]);
				//~ t1 *= (cp[i]-cp[j]);
			//~ }
		//~ //p = p + p1;
		//~ p = p + p1 * Polynomial((*this)('f', cp[i]) / t1);
	//~ }
	//~ ss << p;
}

void Model::render() {
	if (fx.size() > 10000) fx.clear();
	if (lx.size() > 10000) lx.clear();

	cp.clear();
	h = (b - a) / n;
	for (int i = 0; i <= n; i++)
		cp.push_back(a + i * h);

	dx = width * 0.90 / (b - a); ii = 1 / dx / 4;
	double tmp;
	min = (*this)('f', a); max = (*this)('f', a);
	for (double i = a; i <= b; i += ii) {
		tmp = (*this)('f', i);
		fx[i] = tmp;
		if (tmp > max) max = tmp;
		if (tmp < min) min = tmp;
	}
	for (double i = a; i <= b; i += ii) {
		tmp = (*this)('L', i);
		lx[i] = tmp;
		//if (tmp > max) max = tmp;
		//if (tmp < min) min = tmp;
	}
	dy = height * 0.80 / (min - max);

	if (0 < a) ox = a;
	else if (0 > b) ox = b;
	else ox = 0;
	if (0 < min) oy = min;
	else if (0 > max) oy = max;
	else oy = 0;
}
