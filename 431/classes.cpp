#include "classes.h"

Point::Point() {memset(cord, 0, 3*sizeof(float));}

Point::Point(GLfloat x, GLfloat y) { cord[0] = x; cord[1] = y; cord[2] = 0; }

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
	cord[0] = x; cord[1] = y; cord[2] = z;
}

Point::Point(GLfloat cords[3]) {memcpy(cord, cords, sizeof(cord));}

void Point::set(GLfloat x, GLfloat y) {cord[0] = x; cord[1] = y;}

void Point::set(GLfloat x, GLfloat y, GLfloat z)
	{cord[0] = x; cord[1] = y; cord[2] = z;}

void Point::set(GLfloat cords[3]) {memcpy(cord, cords, sizeof(cord));}

void Point::set_x(GLfloat x) { cord[0] = x; }

void Point::set_y(GLfloat y) { cord[1] = y; }

void Point::set_z(GLfloat z) { cord[2] = z; }

GLfloat Point::get_x() {return cord[0];}

GLfloat Point::get_y() {return cord[1];}

GLfloat Point::get_z() {return cord[2];}

GLfloat * Point::get_cords() const {return (GLfloat *) cord;}

void Point::vertex() {glVertex3f(cord[0], cord[1], cord[2]);}

void Point::put_on_line(const Point& a, const Point& b, GLfloat t) {
	GLfloat * ac = a.get_cords(), * bc = b.get_cords(), cords[3] = {0, 0, 0};
	for (int i = 0; i < 3; i++)
		cords[i] = (bc[i] - ac[i]) * t + ac[i];
	set(cords);
}

// pa - point A, pb - point B, lac - AC length, lbc - BC length
void Point::calculate_third (Point pa, Point pb, GLfloat lac, GLfloat lbc,
		int direction) {
	if (pa.get_z() || pb.get_z()) return;
	if (direction < 1 && direction > 2) return;
	GLdouble a, b, c, x0, y0, x2, y2;
	// http://e-maxx.ru/algo/circles_intersection
	x2 = pb.get_x() - pa.get_x();
	y2 = pb.get_y() - pa.get_y();
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
	set(x0 + pa.get_x(), y0 + pa.get_y());
}

Point& Point::operator=(const Point& right) {
	if (this == &right) {
		return *this;
	}
	memcpy(cord, right.cord, sizeof(cord));
	return *this;
}

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

void Canvas::line_to(GLfloat x, GLfloat y) {
	glBegin(GL_LINES);
		glVertex2f(CP.get_x(), CP.get_y());
		glVertex2f(x, y);
	glEnd();
	// draw the line
	CP.set(x, y);
	// update the CP
	glFlush();
}
void Canvas::line_to(Point p) {line_to(p.get_x(), p.get_y());}

void Canvas::move_to(GLfloat x, GLfloat y) {CP.set(x, y);}
void Canvas::move_to(Point p) {CP = p;}

void Canvas::line_rel(GLfloat x, GLfloat y) {
	line_to(CP.get_x() + x, CP.get_y() + y);
}

void Canvas::move_rel(GLfloat x, GLfloat y) {
	move_to(CP.get_x() + x, CP.get_y() + y);
}

void Canvas::turn(GLfloat angle) {CD += angle;}
void Canvas::turn_to(GLfloat angle) {CD = angle;}


void Canvas::forward(GLfloat dist, int is_visible) {
	GLfloat x, y;
	const GLfloat RadPerDeg = M_PI / 180;
	// radians per degree
	x = CP.get_x() + dist * cos(RadPerDeg * CD);
	y = CP.get_y() + dist * sin(RadPerDeg * CD);
	if (is_visible) line_to(x, y);
	else move_to(x, y);
}

PointArray::PointArray() : arr(0) {}

PointArray::PointArray(size_t rows) : arr(rows) {}

void PointArray::clear() {arr.clear();}

void PointArray::push(const Point& p) {
	arr.push_back(p);
}

void PointArray::push(GLfloat cords[3]) {
	Point p(cords);
	arr.push_back(p);
}

void PointArray::set(size_t i, const Point& p) { arr[i] = p; }

std::vector<Point> PointArray::get_arr() const { return arr; }

void PointArray::move_to(Point p) {
	for (int i = 0; i < arr.size(); i++)
		arr[i] += p;
}

void PointArray::render() {
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < arr.size(); i++)
			glVertex2f(arr[i].get_x(), arr[i].get_y());
	glEnd();
	glFlush();
}

PointArray& PointArray::operator=(const PointArray& right) {
	arr = right.arr;
	return *this;
}

Basis::Basis() : PointArray(0) {
	push(Point(1, 0, 0));
	push(Point(0, 1, 0));
	push(Point(0, 0, 1));
}

void Basis::rotate_rel(GLfloat angle) {
	PointArray pa, th = *this;
	GLfloat a = angle * M_PI / 180;
	pa.push(Point(cos(a), -sin(a), 0));
	pa.push(Point(sin(a), cos(a), 0));
	pa.push(Point(0, 0, 1));
	*this = pa * th;
}

Basis& Basis::operator=(const PointArray& right) {
	arr = right.get_arr();
	return *this;
}

const PointArray operator*(const PointArray& left, const PointArray& right) {
	if (left.arr.size() != 3) return right;
	PointArray pa(right.arr.size());
	Point p;
	GLfloat c[3] = {0,0,0};
	for (int i = 0; i < right.arr.size(); i++) {
		for (int j = 0; j < 3; j++) {
			c[j] = left.arr[j] * right.arr[i];
		}
		p.set(c); pa.set(i,p);
	}
	return pa;
}

Path::Path() : PointArray(0), CP(0, 0) {}

Path::Path(std::string pth) : PointArray(0), CP(0, 0) {set(pth);}

void Path::set(std::string pth) {
	clear();
	commands = (char*)"";
	std::stringstream ss;
	ss << pth;
	std::string temp;
	GLfloat fl; char ch;
	int c = 0;
	clear();
	GLfloat c1[3]={0,0,0},c2[3]={0,0,0},c3[3]={0,0,0};
	Point p1, p2, p3;
	while (!ss.eof()) {
		ss >> temp;
		if (std::stringstream(temp) >> fl) {
			if ((c/2)%3 == 0) c1[c%2] = fl;
			else if ((c/2)%3 == 1) c2[c%2] = fl;
			else c3[c%2] = fl;
			c++;
			if ((ch == 'V' || ch == 'v' || ch == 'H' || ch == 'h') && c == 1) {
				if (ch == 'V' || ch == 'v') {
					c1[1] = c1[0]; c1[0] = 0;
				} else c1[1] = 0;
				p1.set(c1);
				push_command(ch, p1);
				c = 0;
			} else if ((ch == 'M' || ch == 'm' || ch == 'L') && c == 2) {
				if (ch == 'V' || ch == 'v') {
					c1[1] = c1[0]; c1[0] = 0;
				}
				p1.set(c1);
				push_command(ch, p1);
				c = 0;
			} else if ((ch == 'Q' || ch == 'q') && c == 4) {
				p1.set(c1);p2.set(c2);
				push_command(ch, p1, p2);
				c = 0;
			}
		} else if (std::stringstream(temp) >> ch) {
			if (ch == 'Z') break;
			c = 0;
		}
		temp = "";
	}
}

void Path::push_command(char com, Point p1) {
	char * all_com = (char *)"MmLVvHh";
	int len = strlen(all_com);
	for (int i = 0; i < len; i++)
		if (com == all_com[i]) {
			//~ printf("%c %g %g\n", com, p1.get_x(), p1.get_y());
			push(p1);
			commands += com;
			return;
		}
}

void Path::push_command(char com, Point p1, Point p2) {
	char * all_com = (char *)"Qq";
	int len = strlen(all_com);
	for (int i = 0; i < len; i++)
		if (com == all_com[i]) {
			//~ printf("%c %g %g %g %g\n", com, p1.get_x(), p1.get_y(),
				//~ p2.get_x(), p2.get_y());
			push(p1);push(p2);
			commands += com;
			return;
		}
}

void Path::push_command(char com, Point p1, Point p2, Point p3) {
	char * all_com = (char *)"Cc";
	int len = strlen(all_com);
	for (int i = 0; i < len; i++)
		if (com == all_com[i]) {
			push(p1);push(p2);push(p3);
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
			case 'M': CP = arr[x]; dest.push(CP); x++; break;
			case 'm': CP = CP + arr[x]; dest.push(CP); x++; break;
			case 'L': CP = arr[x]; dest.push(CP); x++; break;
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
				CP.set(CP.get_x() + arr[x].get_x(), CP.get_y());
				dest.push(CP);
				x++; i+=1;
				break;
			case 'H':
				CP.set(arr[x].get_x(), CP.get_y());
				dest.push(CP);
				x++;
				break;
			case 'v':
				CP.set(CP.get_x(), CP.get_y() + arr[x].get_y());
				dest.push(CP);
				x++;
				break;
			case 'V':
				CP.set(CP.get_x(), arr[x].get_y());
				dest.push(CP);
				x++;
				break;
		}
	}
	dest.push(CP);
}

void Path::q_bezier(PointArray& dest, Point p1, Point p2, Point p3) {
	Point b1, b2, b3;
	for (GLfloat t = 0; t < 1; t+=0.1) {
		b1.put_on_line(p1, p2, t);
		b2.put_on_line(p2, p3, t);
		b3.put_on_line(b1, b2, t);
		dest.push(b3);
	}
}

Object::Object() : path_orig(), path(), points_orig(), points(),
	basis(), pos() {}

Object::Object(std::string pth) : path_orig(pth), path(), points_orig(),
	points(), basis(), pos() {}

Object::Object(Path pth) : path(), points_orig(),
	points(), basis(), pos() {path_orig = pth;}

void Object::set(std::string pth) {path_orig.set(pth);}

void Object::set(Path pth) {path_orig = pth;}

void Object::move_to(Point p) {pos = p;}

void Object::move_rel(Point p) {pos = pos + p;}

//~ void Object::rotate_to(GLfloat angle) {basis.rotate_to(angle);}

void Object::rotate_rel(GLfloat angle) {basis.rotate_rel(angle);}

void Object::render() {
	path_orig.generate(points_orig);
	points = basis * points_orig;
	points.move_to(pos);
	points.render();
}
