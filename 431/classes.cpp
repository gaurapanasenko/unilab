#include "classes.h"

Point::Point() : cord(3, 0.0) {}

Point::Point(GLfloat x, GLfloat y) : cord(3, 0.0) {cord[0] = x; cord[1] = y;}

Point::Point(std::vector<GLfloat> cords) : cord(3, 0.0) {set(cords);}

void Point::set(GLfloat x, GLfloat y) {cord[0] = x; cord[1] = y;}

void Point::set(GLfloat x, GLfloat y, GLfloat z)
	{cord[0] = x; cord[1] = y; cord[2] = z;}

void Point::set(std::vector<GLfloat> cords) {
	if (cords.size() != 3) return;
	for (int i = 0; i < 3; i++)
		cord[i] = cords[i];
}

GLfloat Point::get_x() {return cord[0];}

GLfloat Point::get_y() {return cord[1];}

GLfloat Point::get_z() {return cord[2];}

std::vector<GLfloat> Point::get_cords() const {return cord;}

void Point::vertex() {glVertex3f(cord[0], cord[1], cord[2]);}

void Point::put_on_line(const Point& a, const Point& b, GLfloat t) {
	std::vector<GLfloat> ac = a.get_cords(), bc = b.get_cords(), cords(3);
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
	cord = right.cord;
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
	std::vector<GLfloat> v(3,0);
	for (int i = 0; i < 3; i++)
		v[i] = left.cord[i] + right.cord[i];
	return Point(v);
}

Canvas::Canvas(GLint width, GLint height, char * window_title) {
	char* argv[1];
	// dummy argument list for glutInit()
	char dummy_string[8];
	argv[1] = dummy_string;
	// hook up the pointer
	int argc = 1;
	// to satisfy glutInit()
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
	CP.set(0, 0);
	// initialize the CP to (0, 0)
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

PointArray::PointArray() : Matrix(0, 3), CP(0, 0), path(0) {}

PointArray::PointArray(size_t rows) : Matrix(rows, 3), CP(0, 0), path(0) {}

void PointArray::clear() {recreate(0, 3);}

void PointArray::push(const Point& p) {
	if (row) {
		Point b(matrix.back());
		if (b == p) return;
	}
	matrix.push_back(p.get_cords());
	row++;
}

void PointArray::set_path(std::vector<GLfloat> pth) {path = pth;}

void PointArray::set_path(std::string pth) {
	std::stringstream ss;
	ss << pth;
	std::string temp;
	GLfloat fl; char ch;
	int c = 0;
	path.resize(0);
	while (!ss.eof()) {
		ss >> temp;
		if (std::stringstream(temp) >> fl) {
			if (((ch == 'M' || ch == 'm' || ch == 'L') && c == 2)
					|| ((ch == 'Q' || ch == 'q') && c == 4)) {
				path.push_back(ch);
				c = 0;
			}
			path.push_back(fl);
			c++;
		} else if (std::stringstream(temp) >> ch) {
			path.push_back(ch);
			c = 0;
		}
		temp = "";
	}
}

std::vector<GLfloat> PointArray::get_path() {return path;}

void PointArray::generate_from_path() {
	clear();
	Point p1,p2,p3;
	for(int i = 0; i < path.size(); i++) {
		switch ((char)path[i]) {
			case 'M':
				CP.set(path[i+1], path[i+2]);
				push(CP);
				i+=2;
				break;
			case 'm':
				p1.set(path[i+1], path[i+2]);
				CP = CP + p1;
				push(CP);
				i+=2;
				break;
			case 'L':
				p1.set(path[i+1], path[i+2]);
				push(p1);
				CP = p1;
				i+=2;
				break;
			case 'Q':
				p1.set(path[i+1], path[i+2]);
				p2.set(path[i+3], path[i+4]);
				q_bezier(CP,p1,p2);
				CP = p2;
				i+=4;
				break;
			case 'q':
				p1.set(path[i+1], path[i+2]);
				p1 = p1 + CP;
				p2.set(path[i+3], path[i+4]);
				p2 = p2 + CP;
				q_bezier(CP,p1,p2);
				CP = p2;
				i+=4;
				break;
			case 'h':
				CP.set(CP.get_x()+path[i+1],CP.get_y());
				push(CP);
				i+=1;
				break;
			case 'H':
				CP.set(path[i+1],CP.get_y());
				push(CP);
				i+=1;
				break;
			case 'v':
				CP.set(CP.get_x(),CP.get_y()+path[i+1]);
				push(CP);
				i+=1;
				break;
			case 'V':
				CP.set(CP.get_x(),path[i+1]);
				push(CP);
				i+=1;
				break;
		}
	}
}

void PointArray::q_bezier(Point p1, Point p2, Point p3) {
	Point b1, b2, b3;
	for (GLfloat t = 0; t <= 1; t+=0.1) {
		b1.put_on_line(p1, p2, t);
		b2.put_on_line(p2, p3, t);
		b3.put_on_line(b1, b2, t);
		push(b3);
	}
}

void PointArray::render() {
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < row; i++)
			glVertex2f(matrix[i][0], matrix[i][1]);
	glEnd();
}
