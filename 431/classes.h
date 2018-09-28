#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include "tclasses.h"

namespace Classes {

class Point {
private:
	GLfloat cord[3]; // cordinats
public:
	Point();
	Point(GLfloat x, GLfloat y);
	Point(GLfloat x, GLfloat y, GLfloat z);
	Point(GLfloat cord[3]);
	GLfloat * get_cords() const;
	void vertex();
	void put_on_line(const Point& a, const Point& b, GLfloat t);
	void calculate_third (const Point pa, const Point pb,
		GLfloat lac, GLfloat lbc, int direction);
	GLfloat distance(const Point& p);
	Point& operator=(const Point& right);
	Point& operator=(const GLfloat right[3]);
	GLfloat& operator[](const int index);
	const GLfloat& operator[](const int index) const;
	Point& operator()(GLfloat x, GLfloat y);
	Point& operator()(GLfloat x, GLfloat y, GLfloat z);
	friend bool operator==(const Point& left, const Point& right);
	friend bool operator!=(const Point& left, const Point& right);
	friend Point operator+(const Point& left, const Point& right);
	friend Point& operator+=(Point& left, const Point& right);
	friend const GLfloat operator*(const Point&, const Point&);
};

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
	Canvas();
	// constructor
	void init(int argc, char * argv[], GLint width, GLint height,
		char * window_title);
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
	//~ void line_to(GLfloat x, GLfloat y);
	//~ void line_to(Point p);
	//~ void move_to(GLfloat x, GLfloat y);
	//~ void move_to(Point p);
	//~ void line_rel(GLfloat x, GLfloat y);
	//~ void move_rel(GLfloat x, GLfloat y);
	//~ void turn(GLfloat angl);
	//~ void turn_to(GLfloat angl);
	//~ void forward(GLfloat dist, int is_visible);
};

class PointArray {
protected:
	std::vector<Point> arr;
public:
	PointArray();
	PointArray(size_t rows);
	PointArray(const PointArray& right);
	PointArray(const std::vector<Point>& right);
	void clear();
	const std::vector<Point>& get_arr() const;
	void move_to(const Point& p);
	GLfloat min(int cord);
	GLfloat max(int cord);
	void render();
	Point& operator[](const int index);
	const Point& operator[](const int index) const;
	PointArray& operator=(const PointArray& right);
	PointArray& operator=(const std::vector<Point>& right);
	PointArray& operator<<(const Point& right);
	PointArray& operator<<(GLfloat cords[3]);
	const PointArray operator*(const PointArray& right);
	const PointArray operator*(const PointArray& right) const;
	const Point operator*(const Point& right);
	const Point operator*(const Point& right) const;
};

class Basis : public PointArray {
public:
	Basis();
	void rotate_to(GLfloat angle);
	void rotate_rel(GLfloat angle);
	Basis& operator=(const PointArray& right);
};

//~ class Viewbox : public PointArray {
//~ private:
	//~ Point p[2];
//~ public:
	//~ Viewbox();
	//~ Viewbox(Point p1, Point p2);
	//~ void generate_points();
	//~ Viewbox& operator()(const Point& p1, const Point& p2);
	//~ Viewbox operator=(const Viewbox& right);
//~ };

class Path : public PointArray {
private:
	Point CP;
	std::string commands;
public:
	Path();
	Path(const Path& pth);
	void clear();
	bool empty();
	void move_to(const Point& p);
	void push_command(char com, const Point& p1);
	void push_command(char com, const Point& p1, const Point& p2);
	void push_command(char com, const Point& p1, const Point& p2,
			const Point& p3);
	void generate(PointArray& dest);
	void q_bezier(PointArray& dest, const Point& p1, const Point& p2,
		const Point& p3);
	friend const Path operator*(const PointArray&, const Path&);
};

//~ class PathBox {
//~ private:
	//~ Path path;
	//~ Viewbox box;
//~ public:
	//~ PathBox();
	//~ PathBox(const Path& pth, const Viewbox& b);
	//~ void move_to(const Point& p);
	//~ Path& get_path();
	//~ const Path& get_path() const;
	//~ Viewbox& get_box();
	//~ const Viewbox& get_box() const;
	//~ friend const PathBox operator*(const PointArray&, const PathBox&);
//~ };

class Object {
private:
	Path path;
	PointArray points_orig, points, points_triag;
	Basis basis;
	Point pos;
public:
	Object();
	Object(const Path& pth);
	void move_to(const Point& p);
	void move_rel(const Point& p);
	void rotate_to(GLfloat angle);
	void rotate_rel(GLfloat angle);
	void generate();
	void render();
};

void parse_string(std::vector<Object>& arr, const std::string& str);

int vector_product(Point p1, Point p2, Point p3);

}
