/***************************************************************
 * Name:      shapes.cpp
 * Purpose:   Implementation of shapes
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#include "shapes.h"
#include <algorithm>

/********
* Point *
********/
Point::Point() : coordinates_{0, 0} {}

Point::Point(float x, float y) : coordinates_{x, y} {}

float Point::getX() const {
	return coordinates_[0];
}

void Point::setX(float x) {
	coordinates_[0] = x;
}

float Point::getY() const {
	return coordinates_[1];
}

void Point::setY(float y) {
	coordinates_[1] = y;
}

const Point operator+(const Point& lhs, const Point& rhs) {
	return Point(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY());
}

const Point operator-(const Point& lhs, const Point& rhs) {
	return Point(lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY());
}

const Point operator*(const Point& lhs, const Point& rhs) {
	return Point(lhs.getX() * rhs.getX(), lhs.getY() * rhs.getY());
}

const Point operator/(const Point& lhs, const Point& rhs) {
	return Point(lhs.getX() / rhs.getX(), lhs.getY() / rhs.getY());
}


const Point operator+(const Point& lhs, float rhs) {
	return Point(lhs.getX() + rhs, lhs.getY() + rhs);
}

const Point operator-(const Point& lhs, float rhs) {
	return Point(lhs.getX() - rhs, lhs.getY() - rhs);
}

const Point operator*(const Point& lhs, float rhs) {
	return Point(lhs.getX() * rhs, lhs.getY() * rhs);
}

const Point operator/(const Point& lhs, float rhs) {
	return Point(lhs.getX() / rhs, lhs.getY() / rhs);
}

float calulateVectorLengthSqruare(const Point& point) {
  return pow(point.getX(), 2.0f) + pow(point.getY(), 2.0f);
}

const Point abs(const Point& point) {
  return Point(std::abs(point.getX()), std::abs(point.getY()));
}

const Point floor(const Point& point) {
	return Point(floor(point.getX()), floor(point.getY()));
}

float calculatePseudoscalarProduct(
	const Point& a, const Point& b, const Point& c
) {
	Point ab = b - a, ac = c - a;
	return ab.getX() * ac.getY() - ab.getY() * ac.getX();
}

bool isOneSizePointsToStraight(
	const Point& a, const Point& b, const Point& c, const Point& d
) {
	return calculatePseudoscalarProduct(c, a, b) *
	       calculatePseudoscalarProduct(d, a, b) > 0;
}

Size::Size() : Point(0, 0) {}

Size::Size(float x, float y) : Point(x, y) {}

void Size::setX(float x) {
	Point::setX((x > 0) ? x : 0);
}

void Size::setY(float y) {
	Point::setY((y > 0) ? y : 0);
}

bool Size::isInFrame(const Point& point) const {
	Point p(abs(point) * 2);
	return (p.getX() < getX() && p.getY() < getY());
}

/********
* Color *
********/
Color::Color() : r_(0), g_(0), b_(0) {}

Color::Color(unsigned char r, unsigned char g, unsigned char b) :
r_(r), g_(g), b_(b) {}

double Color::getR() {
  return double(r_) / 255;
}

double Color::getG() {
  return double(r_) / 255;
}

double Color::getB() {
  return double(r_) / 255;
}

/// \brief Creates random color
/// \return colors
Color randomColor() {
  return {
    static_cast<unsigned char>(rand() % 256),
    static_cast<unsigned char>(rand() % 256),
    static_cast<unsigned char>(rand() % 256)
  };
}

float calculateDistanceToEllipse(
	const Point& point, const Point& size
) {
	if (size.getX() <= 0 || size.getY() <= 0) return 0;
  return pow(point.getX(), 2.0f) / pow(size.getX() / 2.0f, 2.0f) +
         pow(point.getY(), 2.0f) / pow(size.getY() / 2.0f, 2.0f);
}

/******************
* ShapeParameters *
******************/

ShapeParameters::ShapeParameters()
: position_(Point(0, 0)), size_(Size(10, 10)),
  contextWidth_(0), contextHeight_(0),
  minimumZoom_(1), traceSize_(0), traceTime_(0.1f) {}

Point& ShapeParameters::getPosition() {
	return position_;
}

const Point& ShapeParameters::getPosition() const {
	return position_;
}

Size& ShapeParameters::getSize() {
	return size_;
}

const Size& ShapeParameters::getSize() const {
	return size_;
}

float ShapeParameters::getX() {
	return getPosition().getX();
}

void ShapeParameters::setX(float x) {
	getPosition().setX(x);
}

float ShapeParameters::getY() {
	return getPosition().getY();
}

void ShapeParameters::setY(float y) {
	getPosition().setY(y);
}

float ShapeParameters::getWidth() {
	return getSize().getX();
}

void ShapeParameters::setWidth(float width) {
	if (width <= 0) return;
	getSize().setX(width);
}

float ShapeParameters::getHeight() {
	return getSize().getY();
}

void ShapeParameters::setHeight(float height) {
	if (height <= 0) return;
	getSize().setY(height);
}

int ShapeParameters::getContextWidth() {
	return contextWidth_;
}

void ShapeParameters::setContextWidth(const int width) {
	contextWidth_ = width;
}

int ShapeParameters::getContextHeight() {
	return contextHeight_;
}

void ShapeParameters::setContextHeight(const int height) {
	contextHeight_ = height;
}

float ShapeParameters::getMinimumZoom() {
	return minimumZoom_;
}

void ShapeParameters::setMinimumZoom(float minimumZoom) {
	if (minimumZoom <= 0 || minimumZoom > 1) return;
	minimumZoom_ = minimumZoom;
}

unsigned char ShapeParameters::getTraceSize() {
	return traceSize_;
}

void ShapeParameters::setTraceSize(const unsigned char traceSize) {
	traceSize_ = traceSize;
}

float ShapeParameters::getTraceTime() {
	return traceTime_;
}

void ShapeParameters::setTraceTime(float traceTime) {
	traceTime_ = traceTime;
}

const Cairo::Matrix& ShapeParameters::getDefaultMatrix() {
	return defaultMatrix_;
}

void ShapeParameters::setDefaultMatrix(
  const Cairo::Matrix& defaultMatrix
) {
	defaultMatrix_ = defaultMatrix;
}

ShapeParameters SHAPE;

/*************
* ShapeTrace *
*************/
ShapeTrace::ShapeTrace()
  : shape_(nullptr), tail_(0), time_(0) {}

ShapeTrace& ShapeTrace::operator=(Shape& pointer) {
	if (shape_ == &pointer) return *this;
	shape_ = &pointer;
	tail_ = 0;
	time_ = 0;
  queue_.resize(0);
  /*for (auto& i : queue_) {
    i = pointer.clone();
    i->setPosition(SHAPE.getSize() * -1000);
  }*/
	return *this;
}

void ShapeTrace::draw(const Cairo::RefPtr<Cairo::Context>& context) {
	if (shape_->hasTrace()) {
		const unsigned char& size = SHAPE.getTraceSize();
		if (size != queue_.size()) {
			queue_.resize(0);
			queue_.resize(size);
			tail_ = 0;
		}
		if (
      (clock() - time_) * 1000.0f / CLOCKS_PER_SEC >
      SHAPE.getTraceTime() &&
      queue_.empty()
		) {
			time_ = clock();
			queue_[tail_] = shape_->clone();
			tail_++;
			while (tail_ >= size) tail_ -= size;
		}
		for (unsigned char i = 0; i < size; i++) {
			unsigned char index = tail_ + i;
			while (index >= size) index -= size;
			if (queue_[index])
        queue_[index]->draw(context, 0.75f / (size - i));
		}
  } else if (!queue_.empty()) {
		queue_.resize(0);
		tail_ = 0;
	}
}

/********
* Shape *
********/
Shape::Shape() : defaultZoom_(1.0), zoom_(defaultZoom_),
defaultColor_(randomColor()), color_(defaultColor_),
visible_(true), trace_(false), selected_(false) {}

void Shape::drawShape(const Cairo::RefPtr<Cairo::Context>&) {}

const Pointer<Shape> Shape::clone() {
  return Pointer<Shape>(new Shape());
}

bool Shape::isInShapeVirtual(const Point&) const {
  return true;
}

const Size& Shape::getDefaultSize() const {
  return SHAPE.getSize();
}

void Shape::render() {
  const Size& ds = getDefaultSize();
  Point size = ds * getDefaultZoom() / 2 + 2;
	if (!isSelected()) {
    size = ds * SHAPE.getMinimumZoom() / 2 + 2;
  }
  const int h = SHAPE.getContextHeight(), w = SHAPE.getContextWidth();
  float minX = size.getX(), minY = size.getY();
  float x = getPosition().getX(), y = getPosition().getY();

	if (x < minX)     x = minX;
	if (y < minY)     y = minY;
	if (x > w - minX) x = w - minX;
	if (y > h - minY) y = h - minY;
	setPosition(Point(x, y));
	if (!isSelected()) {
		zoom_ = min(float(min(
      min((x - 2) * 2.0f / ds.getX(), (w - 2 - x) * 2.0f / ds.getX()),
      min((y - 2) * 2.0f / ds.getY(), (h - 2 - y) * 2.0f / ds.getY())
		)), getDefaultZoom());
	} else {
		zoom_ = getDefaultZoom();
	}
}

void Shape::areIntersected(Shape& shape) {
	if (this == &shape) return;
	const Point position = abs(
		getPosition() - shape.getPosition()
  ) * getDefaultZoom();
  bool b = getDefaultSize().isInFrame(position);
	if (b) {
		float zoom = calculateDistanceToEllipse(
      position, getDefaultSize()
		);
    if (zoom < 1.0f) {
      auto it = std::find(intersected_.begin(), intersected_.end(), &shape);
      if (it == intersected_.end()) {
				color_ = randomColor();
        intersected_.emplace_back(&shape);
			}
			if (zoom < zoom_) {
				if (zoom < SHAPE.getMinimumZoom()) {
					zoom_ = SHAPE.getMinimumZoom();
				} else {
					zoom_ = zoom;
				}
			}
			return;
		}
	}
  auto it = std::find(intersected_.begin(), intersected_.end(), &shape);
  if (it != intersected_.end())
    intersected_.erase(it);
}


void Shape::draw(
	const Cairo::RefPtr<Cairo::Context>& context,
	float alpha
) {
	if (visible_) {
    const Point size = getDefaultSize() * zoom_ / 2;
    if (int(size.getX()) == 0 || int(size.getY()) == 0) return;
		Cairo::Matrix matrix(
      double(size.getX()), 0, 0, double(size.getY()),
      double(getPosition().getX()), double(getPosition().getY())
		);
		Cairo::Matrix oldMatrix = context->get_matrix();
		context->transform(matrix);
		drawShape(context);
		context->set_matrix(SHAPE.getDefaultMatrix());
		context->set_source_rgba(
      double(color_.getR()) * 0.6 + 0.4, double(color_.getG()) * 0.4 + 0.6,
      double(color_.getB()) * 0.6 + 0.4, double(alpha) * 0.8
		);
		context->fill_preserve();
		if (isSelected()) {
      context->set_source_rgba(0.8, 0.2, 0.2, double(alpha) * 0.8);
		} else {
      context->set_source_rgba(0.2, 0.8, 0.2, double(alpha) * 0.8);
		}
		context->stroke();
    context->set_matrix(oldMatrix);
	}
}

bool Shape::isInShape(const Point& point) const {
	const Point p = (getPosition() - point) / getZoom();
  return (getDefaultSize().isInFrame(p) && isInShapeVirtual(p));
}

const Point& Shape::getPosition() const {
	return position_;
}

void Shape::setPosition(const Point& position) {
	position_ = position;
}

float Shape::getDefaultZoom() const {
	return defaultZoom_;
}

void Shape::setDefaultZoom(float zoom) {
	if (zoom >= SHAPE.getMinimumZoom()) {
		defaultZoom_ = zoom;
	} else {
		defaultZoom_ = SHAPE.getMinimumZoom();
	}
}

void Shape::toggleDefaultZoom() {
	defaultZoom_ = (int(defaultZoom_) == 1) ? 2 : 1;
}

float Shape::getZoom() const {
	return zoom_;
}

bool Shape::isSelected() {
	return selected_;
}

void Shape::toggleSelection() {
  selected_ = !selected_;
}

void Shape::toggleVisibility() {
  visible_ = !visible_;
}

void Shape::changeColor() {
	color_ = randomColor();
}

void Shape::reset() {
	position_ = SHAPE.getPosition();
	defaultZoom_ = 1.0;
	zoom_ = 1.0;
	color_ = defaultColor_;
  visible_ = true;
  trace_ = false;
  selected_ = false;
}

bool Shape::hasTrace() {
	return trace_;
}

void Shape::toggleTrace() {
  trace_ = !trace_;
}

/*********
* Shapes *
*********/
Shapes::Shapes() :
activeId_(0), activated_(false), activationPoint_(0, 0) {}

Shape& Shapes::getActive() {
	return *array_[activeId_];
}

const size_t& Shapes::getActiveId() {
	return activeId_;
}

void Shapes::add(const Pointer<Shape>& item) {
  array_.emplace_back(Element());
	array_[array_.size() - 1] = item;
}

void Shapes::erase(int index) {
  activated_ = false;
  activeId_ = 0;
  if (index >= 0 && index < int(array_.size()))
    array_.erase(array_.begin() + index);
}

Shape& Shapes::getTop(const Point& p) {
	return *array_[getTopIndex(p)];
}

void Shapes::activate(const Point& p) {
	const size_t top = getTopIndex(p);
	if (top != array_.size()) {
		activeId_ = top;
    activated_ = true;
		activationPoint_ = floor(p - array_[top]->getPosition());
	}
}

void Shapes::moveActive(const Point& p) {
	if (activated_ && activeId_ < array_.size()) {
		array_[activeId_]->setPosition(floor(p - activationPoint_));
	}
}

void Shapes::release() {
  activated_ = false;
}

void Shapes::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  for (auto& i : array_) {
    i->render();
    if (!i->isSelected()) {
      for (auto& j : array_) {
        i->areIntersected(*j);
			}
		}
  }
  for (auto& i : array_) {
    i->draw(context);
	}
}

Shapes::Element&
Shapes::Element::operator=(const Pointer<Shape>& pointer) {
	pointer_ = pointer;
	shapeTrace_ = *pointer_;
	return (*this);
}

Shape& Shapes::Element::operator*() {
	return *pointer_;
}

Shape* Shapes::Element::operator->() {
	return &pointer_;
}

void Shapes::Element::draw(
  const Cairo::RefPtr<Cairo::Context>& context
) {
	shapeTrace_.draw(context);
	pointer_->draw(context);
}

size_t Shapes::getTopIndex(const Point& p) {
	size_t i = array_.size();
	while(i > 0) {
		i--;
		if (array_[i]->isInShape(p)) {
      return i;
		}
	}
	return array_.size();
}
