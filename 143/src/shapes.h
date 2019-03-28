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

class Point : public Gdk::Point {
public:
	Point();
	Point(int x, int y);
	Point& operator=(const Point& rhs);
};

Point operator+(const Point& lhs, const Point& rhs);
Point operator-(const Point& lhs, const Point& rhs);
Point operator*(const Point& lhs, const Point& rhs);
Point operator/(const Point& lhs, const Point& rhs);
Point operator+(const Point& lhs, const double rhs);
Point operator-(const Point& lhs, const double rhs);
Point operator*(const Point& lhs, const double rhs);
Point operator/(const Point& lhs, const double rhs);
const double calulateVectorLengthSqruare(const Point& point);
const Point abs(const Point& point);

/// \brief Calculates pseudo-scalar product of vectors AB and AC
/// \param a point A
/// \param b point B
/// \param c point C
/// \return Pseudo-scalar product
double calculatePseudoscalarProduct(
	const Point& a, const Point& b, const Point& c
);

/// \brief Detects that points C and D is on one side
/// relatively to straight line AB
/// \param a point A
/// \param b point B
/// \param c point C
/// \param d point D
/// \return True if points on one side, else false
bool isOneSizePointsToStraight(
	const Point& a, const Point& b, const Point& c, const Point& d
);

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

/// Frame for shape
class Frame {
public:
	/// \brief Constructor to create empty frame
	Frame();
	/// \brief Constructor with parameters
	Frame(const Point& point1, const Point& point2);

	/// \brief Function call operator to set new frame by these parameters.
	/// If size has negative values then operator does nothing
	void operator()(const Point& position, const Point& size);

	/// \brief Checks is point is in frame
	/// \param point checking point
	/// \return true if point in frame, else false
	const bool isInFrame(const Point& point) const;

	/// \brief Getter for position of frame center
	Point& getPosition();
	/// \brief Constant getter for position of frame center
	const Point& getPosition() const;
	/// \brief Setter for position of frame center
	void setPosition(const Point& position);
	/// \brief Getter for size of frame
	Point& getSize();
	/// \brief Constant getter for size of frame
	const Point& getSize() const;
	/// \brief Setter for size of frame
	/// \return true if size was changed, else false
	void setSize(const Point& size);
	/// \brief Calculates top left corner of frame
	const Point getPoint1() const;
	/// \brief Calculates bottom right corner of frame
	const Point getPoint2() const;

private:
	Point position_, size_;
};

/// \brief The function converts the plane so that the ellipse turns into
/// a circle of radius 1 in the center of the coordinate grid, and then the
/// function gives the distance to the center of the coordinate grid
/// \param point checking point
/// \param frame ellipse
/// \return distance
const double calculateDistanceToEllipse(const Point& point, const Frame& frame);

class ShapeParameters {
public:
	ShapeParameters();
	Frame& getFrame();
	const Frame& getFrame() const;
	const int getX();
	void setX(const int x);
	const int getY();
	void setY(const int y);
	const int getWidth();
	void setWidth(const int width);
	const int getHeight();
	void setHeight(const int height);
	const double getMinimumZoom();
	void setMinimumZoom(const double minimumZoom);
	const unsigned char getTraceSize();
	void setTraceSize(const unsigned char traceSize);
	const double getTraceTime();
	void setTraceTime(const double traceTime);

private:
	/// \brief default frame
	Frame frame_;
	/// \brief minimum zoom
	double minimumZoom_;
	/// \brief number of cloned objects when doing trace
	unsigned char traceSize_;
	/// \brief interval between saving state of Shape object
	/// when doing trace
	double traceTime_;
};

extern ShapeParameters SHAPE;

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
	Array< Pointer<Shape> > queue_;
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
	virtual const bool isInShapeVirtual(const Point& p) const;

	/// \brief Checks that object fields are valid for device size
	/// \param allocation position and size of drawing widget
	const double render(const Gtk::Allocation& allocation);
	/// \brief Drawing shape on painting
	/// \param context class used to draw
	/// \param alpha sets transparency for shape
	void draw(
		const Cairo::RefPtr<Cairo::Context>& context,
		double alpha = 1
	);
	/// \brief Checks is point is in shape
	/// \param p checking point
	/// \return true if point in shape, else false
	const bool isInShape(const Point& p) const;

	/// \brief Getter for default frame for shape
	const Frame& getDefaultFrame() const;
	/// \brief Getter for current frame for shape
	const Frame& getFrame() const;
	/// \brief Setter for new frame for shape
	void setFrame(const Frame& frame);
	/// \brief Getter for current position, if shape is not deformed,
	/// the position will be center of shape
	/// \return position point
	const Point& getPosition() const;
	/// \brief Setter for position of shape
	/// \param point point of center of shape
	void setPosition(const Point& point);
	/// \brief Calculates size of shape
	/// \return point with size in coordinates
	const Point& getSize() const;
	/// \brief Setter for size of shape
	/// \param point point with size in coordinates
	const bool setSize(const Point& point);
	/// \brief Getter for default zoom
	const float getZoom() const;
	/// \brief Toggle zoom
	void toggleZoom();

	/// \brief Toggles visibility of shape
	void toggleVisibility();
	/// \brief Sets random color to shape
	void changeColor();
	/// \brief Sets default color to shape
	void reset();
	/// \brief Checks if shape has trace
	/// \return true if has trace, else false
	bool hasTrace();
	/// \brief Toggles trace of shape
	void toggleTrace();
	/// \brief Checks that two shape objects are intersected
	const double areIntersected(Shape& shape);

private:
	/// \brief Current frame of object
	Frame frame_;
	/// \brief Default zoom
	float zoom_;
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

void updateGlobalDefaultFrame();

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
