#include "classes.h"

Point::Point() {memset(cord, 0, 3*sizeof(GLfloat));}

Point::Point(GLfloat x, GLfloat y) { (*this)(x,y); }

Point::Point(GLfloat x, GLfloat y, GLfloat z) { (*this)(x,y,z); }

Point::Point(GLfloat cords[3]) {*this = cords;}

GLfloat * Point::get_cords() const {return (GLfloat *) cord;}

void Point::vertex() {glVertex3fv(cord);}

void Point::put_on_line(const Point& a, const Point& b, GLfloat t) {
	GLfloat * ac = a.get_cords(), * bc = b.get_cords(), cords[3] = {0, 0, 0};
	for (int i = 0; i < 3; i++)
		cords[i] = (bc[i] - ac[i]) * t + ac[i];
	*this = cords;
}

Point& Point::operator=(const Point& right) {
	if (this == &right) {return *this;}
	memcpy(cord, right.cord, sizeof(cord));
	return *this;
}

Point& Point::operator=(const GLfloat right[3]) {
	memcpy(cord, right, sizeof(cord));
	return *this;
}

GLfloat& Point::operator[](const int index) {return cord[index];}

const GLfloat& Point::operator[](const int index) const {return cord[index];}

Point& Point::operator()(GLfloat x, GLfloat y)
	{cord[0] = x; cord[1] = y; cord[2] = 0; return *this;}

Point& Point::operator()(GLfloat x, GLfloat y, GLfloat z)
	{cord[0] = x; cord[1] = y; cord[2] = z; return *this;}

bool operator==(const Point& left, const Point& right) {
	for (int i = 0; i < 3; i++)
		if (left.cord[i] != right.cord[i]) return false;
	return true;
}

bool operator!=(const Point& left, const Point& right) {
	for (int i = 0; i < 3; i++)
		if (left.cord[i] != right.cord[i]) return true;
	return false;
}

Point operator+(const Point& left, const Point& right) {
	Point p(left);
	for (int i = 0; i < 3; i++) p[i] += right.cord[i];
	return p;
}

Point operator-(const Point& left, const Point& right) {
	Point p(left);
	for (int i = 0; i < 3; i++) p[i] -= right.cord[i];
	return p;
}

Point& operator+=(Point& left, const Point& right) {
	for (int i = 0; i < 3; i++)
		left.cord[i] += right.cord[i];
	return left;
}

const GLfloat operator*(const Point& left, const Point& right) {
	GLfloat x = 0;
	for (int i = 0; i < 3; i++)
		x += left.cord[i] * right.cord[i];
	return x;
}


Color::Color() {memset(rgb, 0, 3*sizeof(GLfloat));}

Color::Color(const Color& c) {*this = c;}

Color::Color(GLfloat r, GLfloat g, GLfloat b) {(*this)(r,g,b);}

Color::Color(const GLfloat in_rgb[3]) { *this = in_rgb; }

Color& Color::operator=(const Color& right) {
	*this = right.rgb;
	return *this;
}

Color& Color::operator=(const GLfloat right[3]) {
	memcpy(rgb, right, sizeof(rgb));
	return *this;
}

Color& Color::operator()(GLfloat r, GLfloat g, GLfloat b) {
	rgb[0] = r; rgb[1] = g; rgb[2] = b;
	return *this;

}
GLfloat& Color::operator[](const int index) { return rgb[index]; }

void Color::set_color() {glColor3fv(rgb);}

Canvas::Canvas() {
	height = 0; width = 0;
	for (int i = 0; i < 4; i++) ortho[i] = 0;
}

void Canvas::set_ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t) {
	ortho[0] = l; ortho[1] = r; ortho[2] = b; ortho[3] = t;
	resize();
}

void Canvas::set_window_size(GLint w, GLint h) {
	width = w; height = h; resize();
}

void Canvas::resize() {
	GLfloat r1 = (ortho[1] - ortho[0]) / (ortho[3] - ortho[2]),
			r2 = width * 1.0 / height,
			o[4] = {ortho[0], ortho[1], ortho[2], ortho[3]};
	if (r1 < r2) {
		o[0] = o[2] * r2; o[1] = o[3] * r2;
	} else  {
		o[2] = o[0] / r2; o[3] = o[1] / r2;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(o[0], o[1], o[2], o[3]);
	glViewport(0, 0, width, height);
}

PointArray::PointArray() : arr(0) {}

PointArray::PointArray(size_t rows) : arr(rows) {}

PointArray::PointArray(const PointArray& right) {*this = right;}

PointArray::PointArray(const std::vector<Point>& right) {*this = right;}

void PointArray::clear() {arr.clear();}

const std::vector<Point>& PointArray::get_arr() const { return arr; }

void PointArray::move_to(const Point& p) {
	for (size_t i = 0; i < arr.size(); i++)
		arr[i] += p;
}

int PointArray::min(int cord) {
	if (arr.size() == 0) return 0;
	int min = 0;
	for (size_t i = 1; i < arr.size(); i++)
		if (arr[min][cord] > arr[i][cord]) min = i;
	return min;
}

int PointArray::max(int cord) {
	if (arr.size() == 0) return 0;
	int max = 0;
	for (size_t i = 1; i < arr.size(); i++)
		if (arr[max][cord] < arr[i][cord]) max = i;
	return max;
}

void PointArray::render() {
	glLineWidth(10);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINE_LOOP);
		glVertex2fv(arr[0].get_cords());
		for (size_t i = 1; i < arr.size(); i++)
			if (arr[i] != arr[i-1])
				glVertex2fv(arr[i].get_cords());
	glEnd();
	glDisable(GL_BLEND);
	glBlendFunc(GL_NONE, GL_NONE);
	glDisable(GL_LINE_SMOOTH);
}

void PointArray::render_triangles() {
	glBegin(GL_TRIANGLES);
		for (size_t i = 0; i < arr.size(); i++)
			arr[i].vertex();
	glEnd();
}

Point& PointArray::operator[](const int index) {return arr[index];}

const Point& PointArray::operator[](const int index) const {return arr[index];}

PointArray& PointArray::operator=(const PointArray& right) {
	if (this == &right) {return *this;}
	arr = right.arr;
	return *this;
}

PointArray& PointArray::operator=(const std::vector<Point>& right) {
	arr = right;
	return *this;
}

PointArray& PointArray::operator<<(const Point& right) {
	arr.push_back(right);
	return *this;
}

const PointArray PointArray::operator*(const PointArray& right) {
	return static_cast<const PointArray&>(*this).operator*(right);
}

const PointArray PointArray::operator*(const PointArray& right) const {
	if (arr.size() != 3) return right;
	PointArray pa(right.arr.size());
	for (size_t i = 0; i < right.arr.size(); i++)
		pa[i] = (*this) * right.arr[i];
	return pa;
}

const Point PointArray::operator*(const Point& right) {
	if (arr.size() != 3) return right;
	return Point(arr[0]*right,arr[1]*right,arr[2]*right);
}

const Point PointArray::operator*(const Point& right) const {
	if (arr.size() != 3) return right;
	return Point(arr[0]*right,arr[1]*right,arr[2]*right);
}

Basis::Basis() : PointArray(0) { CA = 0; CZ = 1; reset(); }

void Basis::rotate_to(GLfloat angle) { CA = angle; reset(); }

void Basis::rotate_rel(GLfloat angle) { CA += angle; rotate(angle); }

void Basis::zoom_to(GLfloat x) { CZ = x; reset(); }

void Basis::zoom_rel(GLfloat x) { CZ *= x; zoom(x); }

Basis& Basis::operator=(const PointArray& right) {
	if (this == &right) {return *this;}
	arr = right.get_arr(); return *this;
}

void Basis::reset() {
	clear();
	*this << Point(1, 0, 0);
	*this << Point(0, 1, 0);
	*this << Point(0, 0, 1);
	rotate(CA);
	zoom(CZ);
}

void Basis::rotate(GLfloat angle) {
	PointArray pa, th = *this;
	GLfloat a = angle * M_PI / 180;
	pa << Point(cos(a), -sin(a), 0);
	pa << Point(sin(a), cos(a), 0);
	pa << Point(0, 0, 1);
	*this = pa * th;
}

void Basis::zoom(GLfloat x) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			arr[i][j] *= x;
}

Path::Path() : PointArray(0), CP(0, 0), commands("") {}

Path::Path(const Path& right) : PointArray(0), CP(0, 0), commands("")
	{*this = right;}

void Path::clear() {
	arr.clear(); CP(0, 0, 0); commands.clear();
}

bool Path::empty() {return arr.size() == 0 && commands.size() == 0;}

void Path::push_command(char com, const Point& p1) {
	char * all_com = (char *)"MmLlVvHh";
	int len = strlen(all_com);
	for (int i = 0; i < len; i++)
		if (com == all_com[i]) {
			*this << p1;
			commands += com;
			return;
		}
}

void Path::push_command(char com, const Point& p1, const Point& p2) {
	char * all_com = (char *)"Qq";
	int len = strlen(all_com);
	for (int i = 0; i < len; i++)
		if (com == all_com[i]) {
			*this << p1; *this << p2;
			commands += com;
			return;
		}
}

void Path::push_command(char com, const Point& p1, const Point& p2,
		const Point& p3) {
	char * all_com = (char *)"Cc";
	int len = strlen(all_com);
	for (int i = 0; i < len; i++)
		if (com == all_com[i]) {
			*this << p1; *this << p2; *this << p3;
			commands += com;
			return;
		}
}

void Path::generate(PointArray& dest) {
	dest.clear();
	Point p1, p2, p3;
	int x = 0;
	for(size_t i = 0; i < commands.size(); i++) {
		switch ((char)commands[i]) {
			case 'M': CP = arr[x]; dest << CP; x++; break;
			case 'm': CP = CP + arr[x]; dest << CP; x++; break;
			case 'L': CP = arr[x]; dest << CP; x++; break;
			case 'l': CP = CP + arr[x]; dest << CP; x++; break;
			case 'Q':
				p1 = arr[x]; p2 = arr[x+1];
				q_bezier(dest, CP, p1, p2);
				CP = p2; x+=2;
				break;
			case 'q':
				p1 = arr[x]; p2 = arr[x+1];
				p1 = p1 + CP; p2 = p2 + CP;
				q_bezier(dest, CP, p1, p2);
				CP = p2; x+=2;
				break;
			case 'h':
				CP(CP[0] + arr[x][0], CP[1]);
				dest << CP;
				x++;
				break;
			case 'H':
				CP(arr[x][0], CP[1]);
				dest << CP;
				x++;
				break;
			case 'v':
				CP(CP[0], CP[1] + arr[x][0]);
				dest << CP;
				x++;
				break;
			case 'V':
				CP(CP[0], arr[x][0]);
				dest << CP;
				x++;
				break;
		}
	}
	dest << CP;
}

void Path::q_bezier(PointArray& dest, const Point& p1, const Point& p2,
		const Point& p3) {
	Point b1, b2, b3;
	for (GLfloat t = 0; t < 1; t+=0.1) {
		b1.put_on_line(p1, p2, t);
		b2.put_on_line(p2, p3, t);
		b3.put_on_line(b1, b2, t);
		dest << b3;
	}
}

const Path operator*(const PointArray& left, const Path& right) {
	Path p;
	PointArray pa;
	pa = left.operator*(right);
	p.arr = pa.get_arr();
	p.commands = right.commands;
	return p;
}

Object::Object() : bc(), fc(), path(), points_orig(), points(), points_triag(),
	basis(), pos() {}

Object::Object(const Path& pth, const Color& b, const Color& f) : bc(b), fc(f),
		fc_orig(f), path(pth), points_orig(), points(), points_triag(),
		basis(), pos() 
	{generate();}

void Object::move_to(const Point& p) {pos = p;}

void Object::move_rel(const Point& p) {pos += p;}

void Object::rotate_to(GLfloat angle) {basis.rotate_to(angle);}

void Object::rotate_rel(GLfloat angle) {basis.rotate_rel(angle);}

void Object::fill(const Color& c) { fc = c; }

void Object::refill() { fc = fc_orig; }

void Object::generate() {
	path.generate(points_orig);
	int min_x = points_orig.min(0), min_y = points_orig.min(1),
		min_z = points_orig.min(2), max_x = points_orig.max(0),
		max_y = points_orig.max(1), max_z = points_orig.max(1);
	Point center((points_orig[min_x][0] - points_orig[max_x][0]) / 2
				 - points_orig[min_x][0],
			(points_orig[min_y][1] - points_orig[max_y][1]) / 2
				 - points_orig[min_y][1],
			(points_orig[min_z][2] - points_orig[max_z][2]) / 2
				 - points_orig[min_z][2]);
	points_orig.move_to(center);
	triangulate(points_orig, points_triag);
}

void Object::triangulate(const PointArray& in_pa, PointArray& out_pa) {
	std::list<Point> d1;
	std::copy(in_pa.get_arr().begin(),
	      	  in_pa.get_arr().end(), std::back_inserter(d1));
	std::list<Point>::iterator it_prev, it = d1.begin(), it_next, i;
	bool x;
	int tl = 0, tr = 0, turn;
	GLfloat svp;
	tl = 0; tr = 0;
	for (i = d1.begin(); i != d1.end(); i++) {
		svp = semi_vector_product(*prev_uniq(d1, i), *i, *next_uniq(d1, i));
		if (svp > 0) tr++;
		else if (svp < 0) tl++;
	}
	if (tr > tl) turn = 1; else turn = -1;
	while (d1.size() > 3) {
		it_prev = prev_uniq(d1, it); it_next = next_uniq(d1, it);
		if (semi_vector_product(*it_prev, *it, *it_next) * turn >= 0) {
			x = false;
			for (i = d1.begin(); i != d1.end() && !x; i++)
				if (is_d_in_abc(*it_prev, *it, *it_next, *i)) x = true;
			if (!x) {
				out_pa << *it_prev;
				out_pa << *it;
				out_pa << *it_next;
				d1.erase(it);
			}
		}
		it = it_next;
	}
	it_prev = prev_uniq(d1, it); it_next = next_uniq(d1, it);
	out_pa << *it_prev;
	out_pa << *it;
	out_pa << *it_next;
}

void Object::render() {
	PointArray pa;
	pa = basis * points_triag;
	pa.move_to(pos);
	fc.set_color();
	pa.render_triangles();
	pa = basis * points_orig;
	pa.move_to(pos);
	bc.set_color();
	pa.render();
}

std::istream &operator>>(std::istream& input, std::vector<Object>& arr) {
	std::string temp;
	char ch = 0, x = 0;
	std::string com = "VvHhMmLlQqBFZ";
	int c = 0;
	Point p[3];
	Color bc(0.5, 0.5, 0.5), fc(1, 1, 1);
	GLfloat * fl[6];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
			fl[2 * i + j] = &p[i][j]; 
	Path pth;
	while (!input.eof()) {
		input >> temp;
		x = 0;
		for (size_t i = 0; i < com.size() && !x; i++)
			if (temp[0] == com[i]) x = 1;
		if (x) ch = temp[0];
			
		p[0](0,0,0); p[1](0,0,0); p[2](0,0,0);
		if (ch == 'V' || ch == 'v' || ch == 'H' || ch == 'h') c = 1;
		else if (ch == 'M' || ch == 'm' || ch == 'L' || ch == 'l') c = 2;
		else if (ch == 'B' || ch == 'F') c = 3;
		else if (ch == 'Q' || ch == 'q') c = 4;
		else if (ch == 'Z') {
			arr.push_back(Object(pth, bc, fc));
			pth.clear();
			continue;
		} else c = 0;
		if (x) input >> temp;
		for (int i = 0; i < c; i++) {
			if (i > 0) input >> temp;
			*(fl[i]) = atof(temp.c_str());
		}
		if (ch == 'B') {
			bc[0] = *(fl[0]); bc[1] = *(fl[1]); bc[2] = *(fl[2]);
		} else if (ch == 'F') {
			fc[0] = *(fl[0]); fc[1] = *(fl[1]); fc[2] = *(fl[2]);
		} else if (c == 1 || c == 2) pth.push_command(ch, p[0]);
		else if (c == 3 || c == 4) pth.push_command(ch, p[0], p[1]);
		else if (c == 5 || c == 6) pth.push_command(ch, p[0], p[1], p[2]);
		temp = "";
	}
	if (!pth.empty()) arr.push_back(Object(pth, bc, fc));
	return input;
}

GLfloat scalar_product(const Point& a, const Point& b, const Point& c) {
	return (a - b) * (c - b);
}

GLfloat semi_vector_product(const Point& a, const Point& b, const Point& c) {
	Point ab = b - a, bc = c - b;
	return ab[0] * bc[1] - ab[1] * bc[0];
}

GLfloat is_d_in_abc(const Point& a, const Point& b, const Point& c,
                       const Point& d) {
	if (a == d || b == d || c == d || a == b || a == c || b == c) return false;
	GLfloat u = (a[0] - d[0]) * (b[1] - a[1]) - (b[1] - a[1]) * (a[1] - d[1]),
			v = (b[0] - d[0]) * (c[1] - b[1]) - (c[0] - b[0]) * (b[1] - d[1]),
			w = (c[0] - d[0]) * (a[1] - c[1]) - (a[0] - c[0]) * (c[1] - d[1]);
	if (u * v > 0 && v * w > 0) return true;
	else return false;
}


std::list<Point>::iterator prev_uniq(std::list<Point>& list,
                                     const std::list<Point>::iterator& item) {
	std::list<Point>::iterator i = prev(list, item);
	while(*i == *item) i = prev(list, i);
	return i;
}

std::list<Point>::iterator next_uniq(std::list<Point>& list,
                                     const std::list<Point>::iterator& item) {
	std::list<Point>::iterator i = next(list, item);
	while(*i == *item) i = next(list, i);
	return i;
}

std::list<Point>::iterator prev(std::list<Point>& list,
                            	const std::list<Point>::iterator& item) {
	std::list<Point>::iterator out = item;
	if (out == list.begin()) out = list.end();
	--out;
	return out;
}

std::list<Point>::iterator next(std::list<Point>& list,
                            	const std::list<Point>::iterator& item) {
	std::list<Point>::iterator out = item; out++;
	if (out == list.end()) out = list.begin();
	return out;
}

