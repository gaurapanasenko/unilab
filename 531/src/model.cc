#include "model.h"

Function::Function(const char * str) : mp(str) {}

real Function::operator()(const real& x) {
	MiniParser::IDMap ids;
	ids["x"] = x;
	return mp.eval(ids);
}

Polynomial::Polynomial() : cof() {}

Polynomial::Polynomial(const real& a) : cof() {cof.push_back(a);}

Polynomial::Polynomial(const real& a,
                       const real& b) : cof() {
	cof.push_back(a); cof.push_back(b);
}

Polynomial::Polynomial(const Polynomial& right) {(*this) = right;}

const std::vector<real>& Polynomial::get_coefficients() const {
	return cof;
}

real Polynomial::operator()(const real& a) {
	if (cof.size() == 0) return 0;
	real out = 0;
	size_t sz = cof.size();
	for (size_t i = 0; i < sz; i++)
		out += cof[i] * pow(a, sz - i - 1);
	out += cof.back();
	return out;
}

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

Polynomial operator*(const Polynomial& left, const Polynomial& right) {
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

Polynomial operator/(const Polynomial& left, const real& right) {
	Polynomial out = left;
	for (size_t i = 0; i < out.cof.size(); i++)
		out.cof[i] /= right;
	return out;
}

std::ostream& operator<<(std::ostream& output,const Polynomial& right) {
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

Model::~Model() {delete fun;}

bool Model::update(const Glib::ustring& function,
                   const Glib::ustring& in_a,
                   const Glib::ustring& in_b,
                   const Glib::ustring& in_n) {
	char * end;
	real tmp, aa = strtod(in_a.c_str(), &end),
		bb = strtod(in_b.c_str(), &end),
		nn = strtod(in_n.c_str(), &end);
	if (aa == bb) bb += 1;
	else if (aa > bb) {tmp = aa; aa = bb; bb = tmp;}
	if (fun_str != function || a != aa || b != bb || n != nn) {
		fun_str = function; a = aa; b = bb; n = nn;
		delete fun;
		fun = new Function(fun_str.c_str());
		cp.clear(); fx.clear(); Lx.clear(); Px.clear(); Fx.clear();
		h = (b - a) / n;
		for (int i = 0; i <= n; i++)
			cp.push_back(a + i * h);
		render();
		return true;
	}
	return false;
}

void Model::resize(const int& h, const int& w) {
	if (height != h || width != w) {
		height = h; width = w;
		render();
	}
}

const std::vector<real>& Model::get_cp() {return cp;}
const real& Model::get_a()   {return a;}
const real& Model::get_b()   {return b;}
const    int& Model::get_ox()  {return ox;}
const    int& Model::get_oy()  {return oy;}
const real& Model::get_min() {return min;}
const real& Model::get_max() {return max;}
const real Model::get_n() {return n;}
const real Model::get_h() {return h;}
const std::map<real, real>& Model::get_fx() {return fx;}
const std::map<real, real>& Model::get_Lx() {return Lx;}
const std::map<real, real>& Model::get_Px() {return Px;}
const std::map<real, real>& Model::get_Fx() {return Fx;}
const real Model::operator()(const char& f, const real x) {
	std::map<real,real>::iterator it;
	real tmp;
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
			it = Lx.find(x);
			if (it != Lx.end())
				return (*it).second;
			tmp = lagrange(x);
			//tmp = lag(x);
			Lx[x] = tmp;
			return tmp;
			break;
		case 'P':
			it = Px.find(x);
			if (it != Px.end())
				return (*it).second;
			tmp = 0;
			//tmp = lag(x);
			Px[x] = tmp;
			return tmp;
			break;
		case 'F':
			it = Fx.find(x);
			if (it != Fx.end())
				return (*it).second;
			tmp = 0;
			//tmp = lag(x);
			Fx[x] = tmp;
			return tmp;
			break;
		case 'X':
			return (x - a) * dx + width * 0.05;
			break;
		case 'Y':
			return (x - max) * dy + height * 0.15;
			break;
	}
	return 0;
}

const real Model::lagrange(const real& x) {
	real xx = x, l = 0, p;
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
	//real tmp;
	ss << "Відрізок [a, b] = [" << a << ", " << b
		<< "]\nКількіть точок та шаг:\nn = "
		<< n << ", h = " << (b - a) / n
		<< "\nf(x) - значення фунцкії\n"
		<< "L(x) - значення інтерполяційного\n"
		<< "       многочлена у формі Лагранжа\n"
		<< "P(x) - значення інтерполяційного\n"
		<< "       многочлена у формі Ньютона\n"
		<< "Ф(x) - значення многочлена найкращого\n"
		<< "       середньоквадратичного наближення\n"
		<< "R1(x) = f(x) - L(x) (похибка інтерполяції)\n"
		<< "R2(x) = f(x) - P(x) (похибка інтерполяції)\n"
		<< "R3(x) = f(x) - Ф(x)\n"
		<< "    (похибка найкращого середньоквадратичного наближення)";
	//ss << "\n" << lag;
	return ss.str();
}

void Model::render() {

	//~ Polynomial p, p1;
	//~ for (int i = 0; i < (int) cp.size(); i++) {
		//~ p1 = Polynomial(1);
		//~ for (int j = 0; j < (int) cp.size(); j++)
			//~ if (i != j) {
				//~ p1 = p1 * Polynomial(1, -cp[j]) / (cp[i] - cp[j]);
			//~ }
		//~ p = p + p1 * Polynomial((*this)('f', cp[i]));
	//~ }
	//~ lag = p;

	dx = width * 0.8 / (b - a); ii = 1 / dx / 4;
	real tmp;
	min = (*this)('f', a); max = (*this)('f', a);
	for (real i = a; i <= b; i += ii) {
		tmp = (*this)('f', i);
		if (tmp > max) max = tmp;
		if (tmp < min) min = tmp;
		(*this)('L', i); (*this)('P', i); (*this)('F', i);
	}
	dy = height * 0.8 / (min - max);

	if (0 < a) ox = a;
	else if (0 > b) ox = b;
	else ox = 0;
	if (0 < min) oy = min;
	else if (0 > max) oy = max;
	else oy = 0;
}
