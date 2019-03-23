/***************************************************************
 * Name:      shapes.h
 * Purpose:   Defines shapes
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#ifndef SHAPES_H
#define SHAPES_H

#include "libgaura.h"

#include <gtkmm.h>
#include <math.h>
#include <time.h>

/// \brief default width of shape
const           int SHAPE_DEFAULT_WIDTH  = 100;
/// \brief default height of shape
const           int SHAPE_DEFAULT_HEIGHT = 100;

/// \brief number of cloned objects when doing trace
const unsigned char SHAPE_TRACE_SIZE     = 16;
/// \brief interval between saving state of Shape object
/// when doing trace
const        double SHAPE_TRACE_TIME     = 0.001;

class Point : public Gdk::Point {
public:
	Point();
	Point(int x, int y);
	Point& operator=(const Point& rhs);
};

Point operator+(const Point& lhs, const Point& rhs);
Point operator-(const Point& lhs, const Point& rhs);
Point operator*(const Point& lhs, const int    rhs);
Point operator/(const Point& lhs, const int    rhs);
const double calulateVectorLength(const Point& point);

class Color {
public:
	Color();
	Color(unsigned char r, unsigned char g, unsigned char b);
	const double operator[](unsigned char index);
	unsigned char& operator()(unsigned char index);
	void applyToContext(const Cairo::RefPtr<Cairo::Context>& context);

private:
	unsigned char color_[4];
};

class Shape;

/// \brief Stores and draws trace for Shape object.
/// <p>This class creates trace of Shape object. It uses static array
/// sized by constant SHAPE_TRACE_SIZE and asks new Shape object every
/// SHAPE_TRACE_TIME seconds.</p>
class ShapeTrace {
public:
	/// \brief Constructor, initializes empty queue.
	ShapeTrace();
	/// \brief Assignment operator that switches to a new Shape object
	/// \param Reference to the new Shape object
	ShapeTrace& operator=(Shape& pointer);
	/// \brief Draws all Shape objects that being stored in queue.
	/// \param context class used to draw
	void draw(const Cairo::RefPtr<Cairo::Context>& context);

private:
	/// \brief pointer to Shape object that being traced
	Shape* shape_;
	/// \brief queue of Shape objects to create trace
	Pointer<Shape> queue_[SHAPE_TRACE_SIZE];
	/// \brief index to last added element
	unsigned char tail_;
	/// \brief time when last element was added
	clock_t time_;
};

/// Processes one shape on paining
class Shape {
public:
	/// \brief Constructor to create empty shape with default size
	Shape();

	/// \brief Virtual destructor to support inheritance
	virtual ~Shape();
	/// \brief Virtual method to draw specific inherited shape
	virtual void drawShape(const Cairo::RefPtr<Cairo::Context>& context);
	/// \brief Virtual cloning method to support inheritance
	virtual const Pointer<Shape> clone();
	/// \brief Checks is point is in shape by shape parameters
	/// \param p checking point
	/// \return true if point in shape, else false
	virtual bool isInShapeVirtual(const Point& p);

	/// \brief Sets shape between two points, automatically computes
	/// position and size of shape
	/// \param one some point on paining
	/// \param two some point on paining
	void setFrame(const Point& one, const Point& two);

	/// \brief Checks that object fields are valid for device size
	/// \param allocation position and size of drawing widget
	void render(const Gtk::Allocation& allocation);
	/// \brief Drawing shape on painting
	/// \param context class used to draw
	/// \param alpha sets transparency for shape
	void draw(
		const Cairo::RefPtr<Cairo::Context>& context,
		double alpha = 1
	);
	/// \brief Moves shape to a new position
	/// \param p new position point
	void moveTo(const Point& p);
	/// \brief Gets current position, if shape is not deformed,
	/// the position will be center of shape
	/// \return position point
	const Point& getPosition();
	/// \brief Checks is point is in shape
	/// \param p checking point
	/// \return true if point in shape, else false
	bool isInShape(const Point& p);

	/// \brief Toggles visibility of shape
	void toggleVisibility();
	/// \brief Sets random color to shape
	void changeColor();
	/// \brief Sets default color to shape
	void resetColor();
	/// \brief Checks if shape has trace
	/// \return true if has trace, else false
	bool hasTrace();
	/// \brief Toggles trace of shape
	void toggleTrace();
	/// \brief Checks that two shape objects are intersected
	void areIntersected(Shape& shape);

	/// \brief Gets top left point of shape frame
	/// \return Point class
	const Point& getPoint1() const;
	/// \brief Gets bottom right point of shape frame
	/// \return Point class
	const Point& getPoint2() const;

private:
	/// \brief Top left corner of shape frame
	Point point1;
	/// \brief Bottom right corner of shape frame
	Point point2;
	/// \brief Position of object, can be changed with moveTo method and
	/// automatically computed in setFrame method by finding
	/// center of frame
	Point position_;
	/// \brief Default size of object, can be changed with setFrame
	Point defaultSize_;
	/// \brief Color, that is generating in constructor
	Color defaultColor_;
	/// \brief Current color of shape
	Color color_;
	/// \brief List of Shape objects that intersects this object
	Array<Shape*> intersected_;
	/// \brief Saved path for automated motion
	//Array<Point> path_;
	/// \brief Defines visibility of shape
	bool visible_;
	/// \brief Defines visibility of shape
	bool trace_;
};

/// \brief Container for Shape objects, also stores trace for object
class Shapes {
public:
	/// \brief Default construction that initializes fields
	Shapes();

	/// \brief Gets active shape
	/// \return Reference to shape object
	Shape& getActive();
	/// \brief Gets index of active shape
	/// \return Reference to index
	const size_t& getActiveId();

	/// \brief Adds new Shape object to list of objects
	/// \param item Wrapped pointer to Shape object in Pointer object
	void add(const Pointer<Shape>& item);
	/// \brief Deletes Shape object on index
	/// \param index index of object that will be deleted
	void erase(const size_t index);

	/// \brief Activates Shape object if point in argument placed in Shape
	/// \param p point by which finding Shape object
	void activate(const Point& p);
	/// \brief Moves Shape object activated by function activate
	/// to a new position relatively position where object was activated
	/// \param p new position of shape relatively of activation point
	void moveActive(const Point& p);
	/// \brief Releases activated object, so it will not be
	/// moved by moveActive.
	void release();

	/// \brief Draws all Shape objects that stored in list
	/// \param context class used to draw
	/// \param allocation position and size of drawing widget
	void draw(
		const Cairo::RefPtr<Cairo::Context>& context,
		const Gtk::Allocation& allocation
	);

private:

	/// \brief Element of object for correct processing Shape object and
	/// ShapeTrace object
	class Element {
	public:
		/// \brief sets new Shape object and prepare ShapeTrace
		/// for new object
		/// \param pointer Wrapped pointer of Shape object to Pointer object
		/// \return reference to the current element
		Element& operator=(const Pointer<Shape>& pointer);
		/// \brief Member operator to access Shape object
		/// \return Reference to Shape object
		Shape& operator*();
		/// \brief Member operator to access members of Shape object
		/// \return Pointer to Shape object
		Shape* operator->();
		/// \brief Draws Shape object and all traces
		/// \param context class used to draw
		void draw(const Cairo::RefPtr<Cairo::Context>& context);

	private:
		/// \brief Wrapped pointer to Shape object
		Pointer<Shape> pointer_;
		/// \brief ShapeTrace object to create trace of Shape object stored
		/// in pointer_
		ShapeTrace shapeTrace_;
	};
	/// \brief Array of elements
	Array<Element> array_;
	/// \brief Active Shape object index
	size_t activeId_;
	/// \brief Detects that active id will be moved by mouse motion
	bool activated_;
	/// \brief Activation point of Shape object
	Point activationPoint_;

};

#endif // SHAPES_H
