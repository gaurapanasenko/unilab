#include "classes.h"

namespace Classes {

Point::Point() {memset(cord, 0, 3*sizeof(float));}

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

// pa - point A, pb - point B, lac - AC length, lbc - BC length
void Point::calculate_third (const Point pa, const Point pb,
		GLfloat lac, GLfloat lbc, int direction) {
	if (pa[3] || pb[3]) return;
	if (direction < 1 && direction > 2) return;
	GLdouble a, b, c, x0, y0, x2, y2;
	// http://e-maxx.ru/algo/circles_intersection
	x2 = pb[0] - pa[0];
	y2 = pb[1] - pa[1];
	a = -2 * x2; b = -2 * y2; c = x2*x2 + y2*y2 + lac*lac - lbc*lbc;
	// http://e-maxx.ru/algo/circle_line_intersection
	x0 = -a*c/(a*a+b*b); y0 = -b*c/(a*a+b*b);
	if (c*c > lac*lac*(a*a+b*b)+0.000001)
		return;
	else if (abs (c*c - lac*lac*(a*a+b*b)) > 0.0001) {
		double d = lac*lac - c*c/(a*a+b*b);
		double mult = sqrt (d / (a*a+b*b));
		if (direction == 1) {x0 += b * mult; y0 -= a * mult;}
		else {x0 -= b * mult; y0 += a * mult;}
	}
	(*this)(x0 + pa[0], y0 + pa[0]);
}

GLfloat Point::distance(const Point& a) {
	return sqrt(pow(cord[0]-a[0],2)+pow(cord[1]-a[1],2)
		+pow(cord[2]-a[2],2));
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
	{cord[0] = x; cord[1] = y; return *this;}

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
	GLfloat v[3] = {0, 0, 0};
	for (int i = 0; i < 3; i++)
		v[i] = left.cord[i] + right.cord[i];
	return Point(v);
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

Canvas::Canvas() : CP(), viewport(), window() {
}

void Canvas::init(int argc, char * argv[], GLint width, GLint height,
		char * window_title) {
	glutInit(&argc, argv);
	// initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// set display mode
	glutInitWindowSize(width, height);
	// set window size
	glutInitWindowPosition(100, 100);
	// set window position
	glutCreateWindow(window_title);
	// open the screen window
	set_window(-width/2, width/2, -height/2, height/2);
	// default world window
	resize(width, height);
}

void Canvas::resize(GLint width, GLint height) {
	GLfloat R = get_window_aspect_ratio();
	GLint diff = abs(width - height) / 2;
	if (R > width/height) set_viewport(0, width, diff, width + diff);
	else set_viewport(diff, height + diff, 0, height);
	window_width = width;
	window_height = height;
}

void Canvas::set_viewport(GLint l, GLint r, GLint b, GLint t) {
	glViewport(l, b, r-l, t-b);
	viewport.set(l, r, b, t);
}

void Canvas::set_window(GLfloat l, GLfloat r, GLfloat b, GLfloat t) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(l, r, b, t);
	window.set(l, r, b, t);
}

Rect<GLint> Canvas::get_viewport() {return viewport;}

Rect<GLfloat> Canvas::get_window() {return window;}

GLfloat Canvas::get_width() {return window_width;}
GLfloat Canvas::get_height() {return window_height;}

GLfloat Canvas::get_window_aspect_ratio() {
	return ((GLfloat)(window.get_right()-window.get_left()))/
			(window.get_top()-window.get_bottom());
}

void Canvas::clear_screen() {glClear(GL_COLOR_BUFFER_BIT);}

void Canvas::set_background_color(GLfloat r, GLfloat g, GLfloat b) {
	glClearColor(r, g, b, 0);
}

void Canvas::set_color(GLfloat r, GLfloat g, GLfloat b) {
	glColor3f(0.0f, 0.0f, 0.0f);
}

//~ void Canvas::line_to(GLfloat x, GLfloat y) {
	//~ glBegin(GL_LINES);
		//~ glVertex2f(CP[0], CP[1]);
		//~ glVertex2f(x, y);
	//~ glEnd();
	//~ // draw the line
	//~ CP.set(x, y);
	//~ // update the CP
	//~ glFlush();
//~ }
//~ void Canvas::line_to(Point p) {line_to(p[0], p[1]);}

//~ void Canvas::move_to(GLfloat x, GLfloat y) {CP.set(x, y);}
//~ void Canvas::move_to(Point p) {CP = p;}

//~ void Canvas::line_rel(GLfloat x, GLfloat y) {
	//~ line_to(CP[0] + x, CP[1] + y);
//~ }

//~ void Canvas::move_rel(GLfloat x, GLfloat y) {
	//~ move_to(CP[0] + x, CP[1] + y);
//~ }

//~ void Canvas::turn(GLfloat angle) {CD += angle;}
//~ void Canvas::turn_to(GLfloat angle) {CD = angle;}


//~ void Canvas::forward(GLfloat dist, int is_visible) {
	//~ GLfloat x, y;
	//~ const GLfloat RadPerDeg = M_PI / 180;
	//~ // radians per degree
	//~ x = CP[0] + dist * cos(RadPerDeg * CD);
	//~ y = CP[1] + dist * sin(RadPerDeg * CD);
	//~ if (is_visible) line_to(x, y);
	//~ else move_to(x, y);
//~ }

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

GLfloat PointArray::min(int cord) {
	if (arr.size() == 0) return 0;
	GLfloat min = arr[0][cord];
	for (size_t i = 1; i < arr.size(); i++)
		if (min > arr[i][cord]) min = arr[i][cord];
	return min;
}

GLfloat PointArray::max(int cord) {
	if (arr.size() == 0) return 0;
	GLfloat max = arr[0][cord];
	for (size_t i = 1; i < arr.size(); i++)
		if (max < arr[i][cord]) max = arr[i][cord];
	return max;
}

void PointArray::render() {
	glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < arr.size(); i++)
			glVertex2fv(arr[i].get_cords());
	glEnd();
	glPointSize(10);
	glBegin(GL_POINTS);
		for (size_t i = 0; i < arr.size(); i++)
			glVertex2fv(arr[i].get_cords());
	glEnd();
	glBegin(GL_POLYGON);
		for (size_t i = 0; i < arr.size(); i++)
			glVertex2fv(arr[i].get_cords());
	glEnd();
	glFlush();
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
	arr.push_back(right); return *this;
}

PointArray& PointArray::operator<<(GLfloat cords[3]) {
	arr.push_back(Point(cords)); return *this;
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

Basis::Basis() : PointArray(0) {
	*this << Point(1, 0, 0);
	*this << Point(0, 1, 0);
	*this << Point(0, 0, 1);
}

void Basis::rotate_to(GLfloat angle) {
	Point zero;
	zero(0,0,0);
	arr[0](arr[0].distance(zero),0,0);
	arr[1](0,arr[1].distance(zero),0);
	arr[2](0,0,arr[2].distance(zero));
	rotate_rel(angle);
}

void Basis::rotate_rel(GLfloat angle) {
	PointArray pa, th = *this;
	GLfloat a = angle * M_PI / 180;
	pa << Point(cos(a), -sin(a), 0);
	pa << Point(sin(a), cos(a), 0);
	pa << Point(0, 0, 1);
	*this = pa * th;
}

Basis& Basis::operator=(const PointArray& right) {
	if (this == &right) {return *this;}
	arr = right.get_arr(); return *this;
}


//~ Viewbox::Viewbox() : PointArray() {p[0](0,0,0); p[1](0,0,0);}

//~ Viewbox::Viewbox(Point p1, Point p2) : PointArray() {(*this)(p1,p2);}

//~ void Viewbox::generate_points() {
	//~ clear();
	//~ for (int i = 0; i < 2; i++)
		//~ for (int j = 0; j < 2; j++)
			//~ for (int k = 0; k < 2; k++)
				//~ *this << Point(p[i][0], p[j][1], p[k][2]);
//~ }

//~ Viewbox& Viewbox::operator()(const Point& p1, const Point& p2) {
	//~ p[0] = p1; p[1] = p2;
	//~ generate_points();
	//~ return *this;
//~ }

//~ Viewbox Viewbox::operator=(const Viewbox& right) {
	//~ Viewbox v;
	//~ v.arr = right.arr;
	//~ v.p[0] = right.p[0];
	//~ v.p[1] = right.p[1];
	//~ return v;
//~ }

Path::Path() : PointArray(0), CP(0, 0) {}

Path::Path(const Path& right) : PointArray(0), CP(0, 0) {*this = right;}

void Path::clear() {
	PointArray::clear(); CP(0, 0, 0); commands.clear();
}

bool Path::empty() {return arr.size() == 0 && commands.size() == 0;}

void Path::move_to(const Point& p) {
	int x = 0;
	for(int i = 0; i < commands.size(); i++) {
		switch ((char)commands[i]) {
			case 'M': arr[x] += p; x++; break;
			case 'm': x++; break;
			case 'L': arr[x] += p; x++; break;
			case 'Q': arr[x] += p; arr[x+1] += p; x+=2; break;
			case 'q': x+=2; break;
			case 'h': x++; break;
			case 'H': arr[x] += p; x++; break;
			case 'v': x++; break;
			case 'V': arr[x] += p; x++; break;
		}
	}
}

void Path::push_command(char com, const Point& p1) {
	char * all_com = (char *)"MmLVvHh";
	int len = strlen(all_com);
	for (int i = 0; i < len; i++)
		if (com == all_com[i]) {
			//~ printf("%c %f %f\n",com,p1[0],p1[1]);
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
			//~ printf("%c %f %f %f %f\n",com,p1[0],p1[1],p2[0],p2[1]);
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
	for(int i = 0; i < commands.size(); i++) {
		switch ((char)commands[i]) {
			case 'M': CP = arr[x]; dest << CP; x++; break;
			case 'm': CP = CP + arr[x]; dest << CP; x++; break;
			case 'L': CP = arr[x]; dest << CP; x++; break;
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
				CP(CP[0], CP[1] + arr[x][1]);
				dest << CP;
				x++;
				break;
			case 'V':
				CP(CP[0], arr[x][1]);
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
	for (GLfloat t = 0; t < 1; t+=1) {
		b1.put_on_line(p1, p2, t);
		b2.put_on_line(p2, p3, t);
		b3.put_on_line(b1, b2, t);
		dest << b3;
	}
}

//~ Path& Path::operator=(const std::string& right) {
	//~ clear();
	//~ commands = (char*)"";
	//~ std::stringstream ss;
	//~ ss << right;
	//~ std::string temp;
	//~ GLfloat fl; char ch;
	//~ int c = 0;
	//~ clear();
	//~ GLfloat c1[3]={0,0,0},c2[3]={0,0,0},c3[3]={0,0,0};
	//~ Point p1, p2, p3;
	//~ while (!ss.eof()) {
		//~ ss >> temp;
		//~ if (std::stringstream(temp) >> fl) {
			//~ if ((c/2)%3 == 0) c1[c%2] = fl;
			//~ else if ((c/2)%3 == 1) c2[c%2] = fl;
			//~ else c3[c%2] = fl;
			//~ c++;
			//~ if ((ch == 'V' || ch == 'v' || ch == 'H' || ch == 'h') && c == 1) {
				//~ if (ch == 'V' || ch == 'v') {
					//~ c1[1] = c1[0]; c1[0] = 0;
				//~ } else c1[1] = 0;
				//~ p1 = c1;
				//~ push_command(ch, p1);
				//~ c = 0;
			//~ } else if ((ch == 'M' || ch == 'm' || ch == 'L') && c == 2) {
				//~ if (ch == 'V' || ch == 'v') {
					//~ c1[1] = c1[0]; c1[0] = 0;
				//~ }
				//~ p1 = c1;
				//~ push_command(ch, p1);
				//~ c = 0;
			//~ } else if ((ch == 'Q' || ch == 'q') && c == 4) {
				//~ p1 = c1; p2 = c2;
				//~ push_command(ch, p1, p2);
				//~ c = 0;
			//~ }
		//~ } else if (std::stringstream(temp) >> ch) {
			//~ if (ch == 'Z') break;
			//~ c = 0;
		//~ }
		//~ temp = "";
	//~ }
	//~ return *this;
//~ }

const Path operator*(const PointArray& left, const Path& right) {
	Path p;
	PointArray pa;
	pa = left.operator*(right);
	p.arr = pa.get_arr();
	p.commands = right.commands;
	return p;
}

//~ PathBox::PathBox() : path(), box() {}

//~ PathBox::PathBox(const Path& pth, const Viewbox& b) : path(pth), box(b) {}

//~ void PathBox::move_to(const Point& p) {
	//~ path.move_to(p);
	//~ box.move_to(p);
//~ }

//~ Path& PathBox::get_path() {return path;}

//~ const Path& PathBox::get_path() const {return path;}

//~ Viewbox& PathBox::get_box() {return box;}

//~ const Viewbox& PathBox::get_box() const {return box;}

//~ const PathBox operator*(const PointArray& left, const PathBox& right) {
	//~ PathBox p;
	//~ p.get_path() = left * right.get_path();
	//~ p.get_box() = left * right.get_box();
	//~ return p;
//~ }

Object::Object() : path(), points_orig(), points(), points_triag(),
	basis(), pos() {}

Object::Object(const Path& pth) : path(pth),
	points_orig(), points(), points_triag(), basis(), pos() {generate();}

void Object::move_to(const Point& p) {pos = p;}

void Object::move_rel(const Point& p) {pos = pos + p;}

void Object::rotate_to(GLfloat angle) {basis.rotate_to(angle);}

void Object::rotate_rel(GLfloat angle) {basis.rotate_rel(angle);}

void Object::generate() {
	path.generate(points_orig);
	Point center((points_orig.min(0) - points_orig.max(0)) / 2
				- points_orig.min(0),
			(points_orig.min(1) - points_orig.max(1)) / 2 - points_orig.min(1),
			(points_orig.min(2) - points_orig.max(2)) / 2 - points_orig.min(2));
	points_orig.move_to(center);
}

void Object::render() {
	points = basis * points_orig;
	points.move_to(pos);
	points.render();
}

//~ Object& Object::operator=(const Path& right)
	//~ {path_orig = right; return *this;}

void parse_string(std::vector<Object>& arr, const std::string& str) {
	arr.clear();
	std::stringstream ss;
	ss << str;
	std::string temp;
	GLfloat fl; char ch = 0;
	int c = 0;
	Point p1, p2, p3;
	Path pth;
	while (!ss.eof()) {
		ss >> temp;
		if (std::stringstream(temp) >> fl) {
			if ((c/2)%3 == 0) p1[c%2] = fl;
			else if ((c/2)%3 == 1) p2[c%2] = fl;
			else p3[c%2] = fl;
			c++;
			if ((ch == 'V' || ch == 'v' || ch == 'H' || ch == 'h') && c == 1) {
				if (ch == 'V' || ch == 'v') {
					p1[1] = p1[0]; p1[0] = 0;
				} else p1[1] = 0;
				pth.push_command(ch, p1);
				c = 0;
			} else if ((ch == 'M' || ch == 'm' || ch == 'L') && c == 2) {
				if (ch == 'V' || ch == 'v') {
					p1[1] = p1[0]; p1[0] = 0;
				}
				pth.push_command(ch, p1);
				c = 0;
			} else if ((ch == 'Q' || ch == 'q') && c == 4) {
				pth.push_command(ch, p1, p2);
				c = 0;
			}
		} else if (std::stringstream(temp) >> ch) {
			if (ch == 'Z') {
				arr.push_back(Object(pth));
				pth.clear();
			}
			c = 0;
		}
		temp = "";
	}
	if (!pth.empty()) arr.push_back(Object(pth));
}

int vector_product(Point p1, Point p2, Point p3) {
	//~ return (a.x*b.y - a.x*c.y - b.x*a.y + b.x*c.y + c.x*a.y - c.x*b.y );
	return 0;
}

}
