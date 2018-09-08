#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include "tclasses.h"

class Point {
private:
	std::vector<GLfloat> cord; // cordinats
public:
	Point();
	Point(GLfloat, GLfloat);
	Point(std::vector<GLfloat> cord);
	void set(GLfloat, GLfloat);
	void set(GLfloat, GLfloat, GLfloat);
	void set(std::vector<GLfloat> cord);
	GLfloat get_x();
	GLfloat get_y();
	GLfloat get_z();
	std::vector<GLfloat> get_cords() const;
	void vertex();
	void put_on_line(const Point& a, const Point& b, GLfloat t);
	void calculate_third (Point, Point, GLfloat, GLfloat, int);
	Point& operator=(const Point&);
	friend bool operator==(const Point& left, const Point& right);
	friend bool operator!=(const Point& left, const Point& right);
	friend Point operator+(const Point& left, const Point& right);
};

bool operator==(const Point& left, const Point& right);
bool operator!=(const Point& left, const Point& right);
Point operator+(const Point& left, const Point& right);

class Canvas {
private:
	Point CP;
	// current position in the world
	GLfloat CD;
	// current degree in the world
	Rect<GLint> viewport;
	// the current viewport
	Rect<GLfloat> window;
	// the current window
	GLint window_width, window_height;
public:
	Canvas(GLint width, GLint height, char * window_title);
	// constructor
	void resize(GLint width, GLint height);
	void set_viewport(GLint l, GLint r, GLint b, GLint  t);
	void set_window(GLfloat l, GLfloat r, GLfloat b, GLfloat t);
	Rect<GLint> get_viewport();
	// divulge the viewport data
	Rect<GLfloat> get_window();
	// divulge the window data
	GLfloat get_width();
	GLfloat get_height();
	GLfloat get_window_aspect_ratio();
	void clear_screen();
	void set_background_color(GLfloat r, GLfloat g, GLfloat b);
	void set_color(GLfloat r, GLfloat g, GLfloat b);
	void line_to(GLfloat x, GLfloat y);
	void line_to(Point p);
	void move_to(GLfloat x, GLfloat y);
	void move_to(Point p);
	void line_rel(GLfloat x, GLfloat y);
	void move_rel(GLfloat x, GLfloat y);
	void turn(GLfloat angl);
	void turn_to(GLfloat angl);
	void forward(GLfloat dist, int is_visible);
};

class PointArray : public Matrix<GLfloat> {
private:
	std::vector<GLfloat> path;
	Point CP;
public:
	PointArray();
	PointArray(size_t rows);
	void clear();
	void push(const Point& p);
	void set_path(std::vector<GLfloat> pth);
	void set_path(std::string pth);
	std::vector<GLfloat> get_path();
	void generate_from_path();
	void q_bezier(Point p1, Point p2, Point p3);
	void render();
};
