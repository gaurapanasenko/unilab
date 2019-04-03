/***************************************************************
 * Name:      shapes.h
 * Purpose:   Declaration of Shape and smart shape container
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#ifndef SHAPES_H
#define SHAPES_H

#include "graphics.h"
#include "shape-parameters.h"

#include <cairomm/matrix.h>
#include <cairomm/context.h>
#include <glibmm/refptr.h>
#include <vector>
#include <memory>

#ifndef DEBUG
const bool SHAPE_DEBUG = false;
#else
const bool SHAPE_DEBUG = true;
#endif

class Shape;

/// \brief Stores and draws trace for Shape object.
/// <p>This class creates trace of Shape object. It uses static array
/// sized by constant SHAPE_TRACE_SIZE and asks new Shape object every
/// SHAPE_TRACE_TIME seconds.</p>
class ShapeTrace {
public:
  /// \brief Constructor, initializes empty queue
  /// \param Reference to the Shape object
  ShapeTrace(std::shared_ptr<Shape> pointer);
  ShapeTrace(const ShapeTrace&) = default;
  ShapeTrace(ShapeTrace&&) noexcept = default;
  ~ShapeTrace();
  static std::shared_ptr<ShapeTrace>
  create(const std::shared_ptr<Shape>& pointer);

  ShapeTrace& operator=(const ShapeTrace&) = default;
  ShapeTrace& operator=(ShapeTrace&&) = default;

  /// \brief Draws all Shape objects that being stored in queue.
  /// \param context class used to draw
  void draw(const Cairo::RefPtr<Cairo::Context>& context,
            bool selected);

private:
  /// \brief Glib::RefPtr to Shape object that being traced
  std::shared_ptr<Shape> shape_;
  /// \brief queue of Shape objects to create trace
  std::vector< std::shared_ptr<Shape> > queue_;
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
  Shape(const Shape& shape);
  Shape(Shape&&) = default;
  Shape& operator=(const Shape&) = default;
  Shape& operator=(Shape&&) = default;

  /// \brief Virtual destructor to support inheritance
  virtual ~Shape();
  /// \brief Virtual method to draw specific inherited shape
  virtual void drawShape(const Cairo::RefPtr<Cairo::Context>& context,
                         bool selected, float alpha = 0.8f);
  /// \brief Virtual cloning method to support inheritance
  virtual const std::shared_ptr<Shape> cloneVirtual();
  /// \brief Checks is point is in shape by shape parameters
  /// \param p checking point
  /// \return true if point in shape, else false
  virtual bool isInShapeVirtual(const Point& p) const;

  const std::shared_ptr<Shape> clone();

  /// \brief Checks that object fields are valid for device size
  /// \param allocation position and size of drawing widget
  void render(bool selected);
  /// \brief Checks that two shape objects are intersected
  bool areIntersected(const std::shared_ptr<Shape>& shape,
                      bool wasIntersected);
  /// \brief Drawing shape on painting
  /// \param context class used to draw
  /// \param alpha sets transparency for shape
  void draw(const Cairo::RefPtr<Cairo::Context>& context,
            bool selected, float alpha = 0.8f);
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
  const Size& getSize() const;
  void setSize(const Size& size);
  void setSizeForce(const Size& size);
  /// \brief Getter for current zoom
  float getZoom() const;
  /// \brief Setter for current zoom
  void setZoom(float zoom);

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
  Size size_;
  /// \brief Current zoom
  float zoom_;
  /// \brief Color, that is generating in constructor
  Color defaultColor_;
  /// \brief Current color of shape
  Color color_;
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
  class Element;
  /// \brief Default construction that initializes fields
  Shapes();

  /// \brief Getter for iterator of active shape
  /// \return Iterator to element with reference to Shape
  std::vector<Element>::iterator getActiveIterator();
  std::shared_ptr<Shape> getActive();
  std::vector<Element>::iterator getTopIterator(const Point& point);
  std::shared_ptr<Shape> getTop(const Point& point);

  /// \brief Adds new Shape object to list of objects
  /// \param item Wrapped pointer to Shape object in pointer object
  void add(const std::shared_ptr<Shape>& pointer);
  /// \brief Deletes Shape object on index
  /// \param index index of object that will be deleted
  void erase(const std::vector<Element>::iterator& iterator);
  void toggleSelection(const std::vector<Element>::iterator& iterator);

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

  void render();
  /// \brief Draws all Shape objects that stored in list
  /// \param context class used to draw
  void draw(const Cairo::RefPtr<Cairo::Context>& context);

  const std::vector< std::shared_ptr<Shape> > getSelected();

private:
  /// \brief Array of elements
  std::vector<Element> array_;
  std::vector< std::vector<bool> > intersected_;
  std::vector< std::shared_ptr<Shape> > selected_;
  /// \brief Detects that active id will be moved by mouse motion
  bool activated_;
  /// \brief Activation point of Shape object
  Point activationPoint_;

};

/// \brief Element of object for correct processing Shape object and
/// ShapeTrace object
class Shapes::Element {
public:
  /// \brief sets new Shape object and prepare ShapeTrace
  /// for new object
  /// \param pointer Wrapped pointer of Shape object to pointer object
  Element(std::shared_ptr<Shape> pointer);
  /// \brief Member operator to access Shape object
  /// \return Reference to Shape object
  const std::shared_ptr<Shape>& operator*();
  /// \brief Member operator to access members of Shape object
  /// \return Glib::RefPtr to Shape object
  Shape* operator->();
  /// \brief Draws Shape object and all traces
  /// \param context class used to draw
  void draw(const Cairo::RefPtr<Cairo::Context>& context);
  operator bool() const;
  bool isSelected() const;
  void toggleSelection();

private:
  /// \brief Wrapped Glib::RefPtr to Shape object
  std::shared_ptr<Shape> pointer_;
  /// \brief ShapeTrace object to create trace of Shape object stored
  /// in pointer_
  std::shared_ptr<ShapeTrace> shapeTrace_;
  bool selected_;
};

#endif // SHAPES_H
