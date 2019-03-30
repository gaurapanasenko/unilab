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
#include <vector>
#include <cmath>
#include <ctime>

class Point {
public:
	Point();
  Point(const Point&) = default;
  Point(Point&&) = default;
  Point(float x, float y);
  ~Point() = default;
  Point& operator=(const Point& rhs) = default;
  Point& operator=(Point&& rhs) = default;
  float getX() const;
  void setX(float x);
  float getY() const;
  void setY(float y);

private:
	float coordinates_[2];
};

const Point operator+(const Point& lhs, const Point& rhs);
const Point operator-(const Point& lhs, const Point& rhs);
const Point operator*(const Point& lhs, const Point& rhs);
const Point operator/(const Point& lhs, const Point& rhs);
const Point operator+(const Point& lhs, float rhs);
const Point operator-(const Point& lhs, float rhs);
const Point operator*(const Point& lhs, float rhs);
const Point operator/(const Point& lhs, float rhs);
float calulateVectorLengthSqruare(const Point& point);
const Point abs(const Point& point);
const Point floor(const Point& point);

/// \brief Calculates pseudo-scalar product of vectors AB and AC
/// \param a point A
/// \param b point B
/// \param c point C
/// \return Pseudo-scalar product
float calculatePseudoscalarProduct(
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

class Size : public Point {
public:
	Size();
  Size(float x, float y);
  void setX(float x);
  void setY(float y);
	/// \brief Checks is point is in frame
	/// \param point checking point
	/// \return true if point in frame, else false
  bool isInFrame(const Point& point) const;
};

class Color {
public:
	Color();
	Color(unsigned char r, unsigned char g, unsigned char b);
  double getR();
  double getG();
  double getB();

private:
  unsigned char r_, g_, b_;
};

/// \brief The function converts the plane so that the
/// ellipse turns into a circle of radius 1 in the center of the
/// coordinate grid, and then the function gives the distance to the
/// center of the coordinate grid
/// \param point checking point
/// \param size size of ellipse
/// \return distance
float calculateDistanceToEllipse(
  const Point& point, const Point& size
);

class ShapeParameters {
public:
	ShapeParameters();
	Point& getPosition();
	const Point& getPosition() const;
  Size& getSize();
  const Size& getSize() const;
  float getX();
  void setX(float x);
  float getY();
  void setY(float y);
  float getWidth();
  void setWidth(float width);
  float getHeight();
  void setHeight(float height);
  int getContextWidth();
  void setContextWidth(int width);
  int getContextHeight();
  void setContextHeight(int height);
  float getMinimumZoom();
  void setMinimumZoom(float minimumZoom);
  unsigned char getTraceSize();
  void setTraceSize(unsigned char traceSize);
  float getTraceTime();
  void setTraceTime(float traceTime);
  const Cairo::Matrix& getDefaultMatrix();
	void setDefaultMatrix(const Cairo::Matrix& defaultMatrix);

private:
	/// \brief default position
	Point position_;
	/// \brief default size
	Size size_;
	/// \brief context width
	int contextWidth_;
	/// \brief context Height
	int contextHeight_;
	/// \brief minimum zoom
	float minimumZoom_;
	/// \brief number of cloned objects when doing trace
	unsigned char traceSize_;
	/// \brief interval between saving state of Shape object
	/// when doing trace
	float traceTime_;
	Cairo::Matrix defaultMatrix_;
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
  std::vector< Pointer<Shape> > queue_;
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
  Shape(const Shape&) = default;
  Shape(Shape&&) = default;

  Shape& operator=(const Shape&) = default;
  Shape& operator=(Shape&&) = default;

	/// \brief Virtual destructor to support inheritance
  virtual ~Shape() = default;
	/// \brief Virtual method to draw specific inherited shape
	virtual void drawShape(const Cairo::RefPtr<Cairo::Context>& context);
	/// \brief Virtual cloning method to support inheritance
	virtual const Pointer<Shape> clone();
	/// \brief Checks is point is in shape by shape parameters
	/// \param p checking point
	/// \return true if point in shape, else false
  virtual bool isInShapeVirtual(const Point& p) const;
  virtual const Size& getDefaultSize() const;

	/// \brief Checks that two shape objects are intersected
	void areIntersected(Shape& shape);
	/// \brief Checks that object fields are valid for device size
	/// \param allocation position and size of drawing widget
	void render();
	/// \brief Drawing shape on painting
	/// \param context class used to draw
	/// \param alpha sets transparency for shape
	void draw(
		const Cairo::RefPtr<Cairo::Context>& context,
		float alpha = 1
	);
	/// \brief Checks is point is in shape
	/// \param p checking point
	/// \return true if point in shape, else false
  bool isInShape(const Point& point) const;

	/// \brief Getter for current position, if shape is not deformed,
	/// the position will be center of shape
	/// \return position point
	const Point& getPosition() const;
	/// \brief Setter for position of shape
	/// \param point point of center of shape
	void setPosition(const Point& position);
	/// \brief Getter for default zoom
  float getDefaultZoom() const;
	/// \brief Setter for default zoom
  void setDefaultZoom(float zoom);
	/// \brief Toggle zoom
	void toggleDefaultZoom();
	/// \brief Getter for current zoom
  float getZoom() const;
	/// \brief Detects is shape is selected
  bool isSelected();
	/// \brief Toggles selection of shape
	void toggleSelection();

	/// \brief Toggles visibility of shape
	void toggleVisibility();
	/// \brief Sets random color to shape
	void changeColor();
	/// \brief Sets default parameters for shape
	void reset();
	/// \brief Checks if shape has trace
	/// \return true if has trace, else false
	bool hasTrace();
	/// \brief Toggles trace of shape
	void toggleTrace();

private:
	/// \brief Current frame of object
	Point position_;
	/// \brief Default zoom
	float defaultZoom_;
	/// \brief Current zoom
	float zoom_;
	/// \brief Color, that is generating in constructor
	Color defaultColor_;
	/// \brief Current color of shape
	Color color_;
	/// \brief List of Shape objects that intersects this object
  std::vector<Shape*> intersected_;
	/// \brief Saved path for automated motion
	//Array<Point> path_;
	/// \brief Defines visibility of shape
	bool visible_;
	/// \brief Defines visibility of shape
	bool trace_;
	/// \brief Variable to detect is shape selected
	bool selected_;
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
  void erase(int index);
	Shape& getTop(const Point& p);

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
	void draw(const Cairo::RefPtr<Cairo::Context>& context);

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

private:
  size_t getTopIndex(const Point& p);

	/// \brief Array of elements
  std::vector<Element> array_;
	/// \brief Active Shape object index
	size_t activeId_;
	/// \brief Detects that active id will be moved by mouse motion
	bool activated_;
	/// \brief Activation point of Shape object
	Point activationPoint_;

};

#endif // SHAPES_H
