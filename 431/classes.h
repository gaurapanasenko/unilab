#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <list>
#include <unistd.h>
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
	friend Point operator-(const Point& left, const Point& right);
	friend Point& operator+=(Point& left, const Point& right);
	friend const GLfloat operator*(const Point&, const Point&);
};

class Color {
private:
	GLfloat rgb[3];
public:
	Color();
	Color(const Color& c);
	Color(GLfloat r, GLfloat g, GLfloat b);
	Color(const GLfloat in_rgb[3]);
	Color& operator=(const Color& right);
	Color& operator=(const GLfloat right[3]);
	Color& operator()(GLfloat r, GLfloat g, GLfloat b);
	GLfloat& operator[](const int index);
	void set_color();
};

class Canvas {
private:
	GLint width, height;
	GLfloat ortho[4];
public:
	Canvas();
	void set_ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t);
	void set_window_size(GLint w, GLint h);
	void resize();
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
	int min(int cord);
	int max(int cord);
	void render();
	void render_triangles();
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
private:
	GLfloat CA, CZ;
public:
	Basis();
	void rotate_to(GLfloat angle);
	void rotate_rel(GLfloat angle);
	void zoom_to(GLfloat x);
	void zoom_rel(GLfloat x);
	Basis& operator=(const PointArray& right);
private:
	void reset();
	void rotate(GLfloat angle);
	void zoom(GLfloat x);
};

class Path : public PointArray {
private:
	Point CP;
	std::string commands;
public:
	Path();
	Path(const Path& pth);
	void clear();
	bool empty();
	//void move_to(const Point& p);
	void push_command(char com, const Point& p1);
	void push_command(char com, const Point& p1, const Point& p2);
	void push_command(char com, const Point& p1, const Point& p2,
			const Point& p3);
	void generate(PointArray& dest);
	void q_bezier(PointArray& dest, const Point& p1, const Point& p2,
		const Point& p3);
	friend const Path operator*(const PointArray&, const Path&);
};

class Object {
private:
	Color bc, fc, fc_orig;
	Path path;
	PointArray points_orig, points, points_triag;
	Basis basis;
	Point pos;
public:
	Object();
	Object(const Path& pth, const Color& b, const Color& f);
	void move_to(const Point& p);
	void move_rel(const Point& p);
	void rotate_to(GLfloat angle);
	void rotate_rel(GLfloat angle);
	void fill(const Color& c);
	void refill();
	void generate();
	void triangulate(const PointArray& in_pa, PointArray& out_pas);
	void render();
};

std::istream &operator>>(std::istream& input, std::vector<Object>& arr);

GLfloat scalar_product(const Point& p1, const Point& p2, const Point& p3);

GLfloat semi_vector_product(const Point& a, const Point& b, const Point& c);

GLfloat is_d_in_abc(const Point& a, const Point& b, const Point& c,
                       const Point& d);

std::list<Point>::iterator prev_uniq(std::list<Point>& list,
                                     const std::list<Point>::iterator& item);

std::list<Point>::iterator next_uniq(std::list<Point>& list,
                                     const std::list<Point>::iterator& item);

std::list<Point>::iterator prev(std::list<Point>& list,
                            	const std::list<Point>::iterator& item);

std::list<Point>::iterator next(std::list<Point>& list,
                            	const std::list<Point>::iterator& item);

}
