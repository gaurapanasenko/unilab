/*
 * classes.h - Set of useful classes for graphics
 * Copyright (C) 2018 Egor Panasenko <gaura.panasenko@gmail.com>
 *
 * This project is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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


// Point class
// -----------
class Point {
private:
	// Coordinates of point
	GLfloat cord[3];
public:
	// Main constructor
	Point();
	// Create 2D point
	Point(GLfloat x, GLfloat y);
	// Create 3D point
	Point(GLfloat x, GLfloat y, GLfloat z);
	// Create 3D point by array
	Point(GLfloat cord[3]);

	// Getter for coordinates
	GLfloat * get_cords() const;

	// Render point
	void vertex();

	// Proportionally put on the segment
	void put_on_line(const Point& a, const Point& b, GLfloat t);
	// Find distance to another point
	GLfloat distance(const Point& p);

	// Copy data from another point
	Point& operator=(const Point& right);
	// Copy data from array
	Point& operator=(const GLfloat right[3]);

	// Get coordinate
	GLfloat& operator[](const int index);
	const GLfloat& operator[](const int index) const;

	// Change coordinates
	Point& operator()(GLfloat x, GLfloat y);
	Point& operator()(GLfloat x, GLfloat y, GLfloat z);

	// Compare points
	friend bool operator==(const Point& left, const Point& right);
	friend bool operator!=(const Point& left, const Point& right);

	// Add/subtract two vectors
	friend Point operator+(const Point& left, const Point& right);
	friend Point operator-(const Point& left, const Point& right);
	friend Point& operator+=(Point& left, const Point& right);

	// Multiply coordinates as matrices (left as row, right ad column)
	friend const GLfloat operator*(const Point& left,
		const Point& right);
};

//
GLfloat scalar_product(const Point& p1, const Point& p2,
	const Point& p3);

GLfloat semi_vector_product(const Point& a, const Point& b,
	const Point& c);

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

// Color class
// -----------
class Color {
private:
	GLfloat rgb[3];
public:
	// Constructors
	Color();
	Color(const Color& c);
	Color(GLfloat r, GLfloat g, GLfloat b);
	Color(const GLfloat in_rgb[3]);

	// Copy data from another color
	Color& operator=(const Color& right);
	// Copy data from array
	Color& operator=(const GLfloat right[3]);

	// Change color
	Color& operator()(GLfloat r, GLfloat g, GLfloat b);

	// Get one color parameter
	GLfloat& operator[](const int index);

	// Send color to GL
	void set_color();
};


// View controller class
// ---------------------
class Canvas {
private:
	// Size of opened window
	GLint width, height;
	// Area of view
	GLfloat ortho[4];
public:
	// Constructor
	Canvas();
	// Set area of view
	void set_ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t);
	// Set new window size
	void set_window_size(GLint w, GLint h);
	// Send new parameters to GL
	void resize();
};


// Array of points class
// ---------------------
class PointArray {
protected:
	// Array of points
	std::vector<Point> arr;
public:
	// Constructor
	PointArray();
	PointArray(size_t rows);
	PointArray(const PointArray& right);
	PointArray(const std::vector<Point>& right);

	// Clear array
	void clear();
	// Get array
	const std::vector<Point>& get_arr() const;

	// Move all points to point p
	void move_to(const Point& p);

	// Find minimal and maximal of all coordinate cord in array
	// where 0 is x, 1 is y, 2 is z as cord argument;
	int min(int cord);
	int max(int cord);

	// Draw all points as line
	void render();
	// Draw all points as triangles
	void render_triangles();

	// Get one point
	Point& operator[](const int index);
	const Point& operator[](const int index) const;

	// Set new data
	PointArray& operator=(const PointArray& right);
	PointArray& operator=(const std::vector<Point>& right);

	// Add new point
	PointArray& operator<<(const Point& right);

	// Multiply two matrices 3x3
	const PointArray operator*(const PointArray& right);
	const PointArray operator*(const PointArray& right) const;

	// Multiply matrix 3x3 on column
	const Point operator*(const Point& right);
	const Point operator*(const Point& right) const;
};

// Set of transformations;
class Basis : public PointArray {
private:
	// Current angle and zoom
	GLfloat CA, CZ;
public:
	//Constructor
	Basis();

	// Rotation
	void rotate_to(GLfloat angle);
	void rotate_rel(GLfloat angle);

	// Zooming
	void zoom_to(GLfloat x);
	void zoom_rel(GLfloat x);

	// Convert array of points to basis
	Basis& operator=(const PointArray& right);
private:
	// Flush current parameters to matrix
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
