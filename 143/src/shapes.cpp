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
Point::Point() : x_(0), y_(0) {}

Point::Point(float x, float y) : x_(x), y_(y) {}

float Point::getX() const {
  return x_;
}

void Point::setX(float x) {
  x_ = x;
}

float Point::getY() const {
  return y_;
}

void Point::setY(float y) {
  y_ = y;
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

/*******
* Size *
*******/
Size::Size() : Point(0, 0) {}

Size::Size(float x, float y) : Point((x > 0) ? x : 0, (y > 0) ? y : 0) {}

Size::Size(const Point& point) : Point(
  (point.getX() > 0) ? point.getX() : 0, (point.getY() > 0) ? point.getY() : 0
) {}

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

/********
* Sizes *
********/

const Size& Sizes::getMinimumSize() const {
  return minimumSize_;
}

const Size& Sizes::getDefaultSize() const {
  return defaultSize_;
}

void Sizes::setDefaultSize(const Size& size) {
  setDefaultSizeX(size.getX());
  setDefaultSizeY(size.getY());
}

void Sizes::setDefaultSizeX(float x) {
  defaultSize_.setX((x > maximumSize_.getX()) ? maximumSize_.getX() : x);
  minimumSize_.setX(defaultSize_.getX() * minimumZoom_);
}

void Sizes::setDefaultSizeY(float y) {
  defaultSize_.setY((y > maximumSize_.getY()) ? maximumSize_.getY() : y);
  minimumSize_.setY(defaultSize_.getY() * minimumZoom_);
}

const Size& Sizes::getMaximumSize() const {
  return maximumSize_;
}

void Sizes::setMaximumSize(const Size& size) {
  setMaximumSizeX(size.getX());
  setMaximumSizeY(size.getY());
}

void Sizes::setMaximumSizeX(float x) {
  maximumSize_.setX(x);
  if (maximumSize_.getX() < defaultSize_.getX()) {
    defaultSize_.setX(maximumSize_.getX());
  }
}

void Sizes::setMaximumSizeY(float y) {
  maximumSize_.setY(y);
  if (maximumSize_.getY() < defaultSize_.getY()) {
    defaultSize_.setY(maximumSize_.getY());
  }
}

float Sizes::getMinimumZoom() const {
  return minimumZoom_;
}

void Sizes::setMinimumZoom(float minimumZoom) {
  minimumZoom_ = (minimumZoom < 0) ? 0 : (minimumZoom > 1) ? 1 : minimumZoom;
  minimumSize_ = defaultSize_ * minimumZoom_;
}

unsigned char Sizes::checkSize(const Size& size) const {
  return static_cast<unsigned char>(
    (size.getX() < getMinimumSize().getX()) << 0 |
    (size.getY() < getMinimumSize().getY()) << 1 |
    (size.getX() > getMaximumSize().getX()) << 2 |
    (size.getY() > getMaximumSize().getY()) << 3
  );
}

Size Sizes::validateSize(const Size& size) const {
  unsigned char b = checkSize(size);
  return {
    (b & 1 << 0) ? getMinimumSize().getX() :
      (b & 1 << 2) ? getMaximumSize().getX() : size.getX(),
    (b & 1 << 1) ? getMinimumSize().getY() :
      (b & 1 << 3) ? getMaximumSize().getY() : size.getY()
  };
}

float Sizes::validateZoom(const Size& size, float zoom) const {
  if (checkSize(size)) return 1.0f;
  float z = zoom;
  if (checkSize(size * z) & 1 << 0) {
    z = getMinimumSize().getX() / size.getX();
  }
  if (checkSize(size * z) & 1 << 1) {
    z = getMinimumSize().getY() / size.getY();
  }
  if (checkSize(size * z) & 1 << 2) {
    z = getMaximumSize().getX() / size.getX();
  }
  if (checkSize(size * z) & 1 << 3) {
    z = getMaximumSize().getY() / size.getY();
  }
  return z;
}

/******************
* ShapeParameters *
******************/

ShapeParameters::ShapeParameters()
: position_(Point(0, 0)), sizes_(),
traceSize_(0), traceTime_(0.1f) {}

const Point& ShapeParameters::getPosition() const {
	return position_;
}

void ShapeParameters::setPosition(const Point& position) {
  position_ = position;
}

float ShapeParameters::getX() {
  return getPosition().getX();
}

void ShapeParameters::setX(float x) {
  position_.setX(x);
}

float ShapeParameters::getY() {
  return getPosition().getY();
}

void ShapeParameters::setY(float y) {
  position_.setY(y);
}


const Sizes& ShapeParameters::getSizes() const {
  return sizes_;
}

const Size& ShapeParameters::getDefaultSize() const {
  return getSizes().getDefaultSize();
}

float ShapeParameters::getDefaultWidth() {
  return sizes_.getDefaultSize().getX();
}

void ShapeParameters::setDefaultWidth(float width) {
  sizes_.setDefaultSizeX(width);
}

float ShapeParameters::getDefaultHeight() {
  return sizes_.getDefaultSize().getY();
}

void ShapeParameters::setDefaultHeight(float height) {
  sizes_.setDefaultSizeY(height);
}

float ShapeParameters::getMaximumWidth() {
  return getSizes().getMaximumSize().getX();
}

void ShapeParameters::setMaximumWidth(float width) {
  sizes_.setMaximumSizeX(width);
}

float ShapeParameters::getMaximumHeight() {
  return getSizes().getMaximumSize().getY();
}

void ShapeParameters::setMaximumHeight(float height) {
  sizes_.setMaximumSizeY(height);
}

void ShapeParameters::setMinimumZoom(float minimumZoom) {
  sizes_.setMinimumZoom(minimumZoom);
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
      (clock() - time_) * 1000.0f / CLOCKS_PER_SEC > SHAPE.getTraceTime()
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
        queue_[index]->draw(context, 0.8f / (size - i));
		}
  } else if (!queue_.empty()) {
		queue_.resize(0);
		tail_ = 0;
	}
}

/********
* Shape *
********/
Shape::Shape() : size_(SHAPE.getDefaultSize()), zoom_(1.0),
defaultColor_(randomColor()), color_(defaultColor_),
visible_(true), trace_(false), selected_(false) {}

void Shape::drawShape(const Cairo::RefPtr<Cairo::Context>&, float) {}

const Pointer<Shape> Shape::clone() {
  return Pointer<Shape>(new Shape());
}

bool Shape::isInShapeVirtual(const Point&) const {
  return true;
}

void Shape::toggleSelectionVirtual() {}

void Shape::render() {
  Point minSize = (
    (isSelected()) ? getSize() :
      getSize() * SHAPE.getSizes().validateZoom(
        getSize(), SHAPE.getSizes().getMinimumZoom()
      )
  ) / 2;
  const float h = SHAPE.getMaximumHeight(), w = SHAPE.getMaximumWidth();

  float x = getPosition().getX(), y = getPosition().getY();
  if (x < minSize.getX())     x = minSize.getX();
  if (y < minSize.getY())     y = minSize.getY();
  if (x > w - minSize.getX()) x = w - minSize.getX();
  if (y > h - minSize.getY()) y = h - minSize.getY();

	setPosition(Point(x, y));
	if (!isSelected()) {
    setZoom(min(float(min(
      min(
        x * 2.0f / getSize().getX(), (w - x) * 2.0f / getSize().getX()
      ),
      min(
        y * 2.0f / getSize().getY(), (h - y) * 2.0f / getSize().getY()
      )
    )), 1.0f));
	} else {
    setZoom(1.0f);
	}
}

void Shape::areIntersected(Shape& shape) {
	if (this == &shape) return;
	const Point position = abs(
		getPosition() - shape.getPosition()
  );
  bool b = getSize().isInFrame(position);
	if (b) {
		float zoom = calculateDistanceToEllipse(
      position, getSize()
		);
    if (zoom < 1.0f) {
      auto it = std::find(intersected_.begin(), intersected_.end(), &shape);
      if (it == intersected_.end()) {
				color_ = randomColor();
        intersected_.emplace_back(&shape);
			}
      if (zoom < getZoom())
        setZoom(zoom);
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
    const Point size = getSize() * zoom_ / 2;
    if (size.getX() <= 0 || size.getY() <= 0) return;
		Cairo::Matrix matrix(
      double(size.getX()), 0, 0, double(size.getY()),
      double(getPosition().getX()), double(getPosition().getY())
    );
    context->save();
		context->transform(matrix);
    drawShape(context, alpha);
		context->set_matrix(SHAPE.getDefaultMatrix());
		context->set_source_rgba(
      double(color_.getR()) * 0.6 + 0.4, double(color_.getG()) * 0.4 + 0.6,
      double(color_.getB()) * 0.6 + 0.4, double(alpha)
		);
		context->fill_preserve();
		if (isSelected()) {
      context->set_source_rgba(0.8, 0.2, 0.2, double(alpha));
		} else {
      context->set_source_rgba(0.2, 0.8, 0.2, double(alpha));
		}
    context->stroke();
    context->restore();
	}
}

bool Shape::isInShape(const Point& point) const {
  const Point p = (point - getPosition()) / getSize() / getZoom() * 2;
  return (Size(2, 2).isInFrame(p) && isInShapeVirtual(p));
}

const Point& Shape::getPosition() const {
	return position_;
}

void Shape::setPosition(const Point& position) {
  position_ = position;
}

const Size&Shape::getSize() const {
  return size_;
}

void Shape::setSize(const Size& size) {
  size_ = SHAPE.getSizes().validateSize(size);
  setZoom(getZoom());
}

void Shape::setSizeForce(const Size& size) {
  size_ = size;
  zoom_ = 1.0;
}

float Shape::getZoom() const {
	return zoom_;
}

void Shape::setZoom(float zoom) {
  zoom_ = SHAPE.getSizes().validateZoom(size_, zoom);
}

bool Shape::isSelected() {
	return selected_;
}

void Shape::toggleSelection() {
  selected_ = !selected_;
  toggleSelectionVirtual();
}

void Shape::toggleVisibility() {
  visible_ = !visible_;
}

void Shape::changeColor() {
	color_ = randomColor();
}

void Shape::reset() {
	position_ = SHAPE.getPosition();
  size_ = SHAPE.getSizes().getDefaultSize();
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
  return *array_.at(activeId_);
}

const size_t& Shapes::getActiveId() {
	return activeId_;
}

void Shapes::add(const Pointer<Shape>& item) {
  array_.emplace_back(Element());
  array_.at(array_.size() - 1) = item;
}

void Shapes::erase(const size_t& index) {
  activated_ = false;
  activeId_ = 0;
  if (index < array_.size())
    array_.erase(array_.begin() + int(index));
}

Shape& Shapes::getTop(const Point& p) {
  return *array_.at(getTopIndex(p));
}

void Shapes::activate(const Point& p) {
	const size_t top = getTopIndex(p);
	if (top != array_.size()) {
		activeId_ = top;
    activated_ = true;
    activationPoint_ = floor(p - array_.at(top)->getPosition());
	}
}

void Shapes::moveActive(const Point& p) {
	if (activated_ && activeId_ < array_.size()) {
    array_.at(activeId_)->setPosition(floor(p - activationPoint_));
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
    i.draw(context);
  }
}

const std::vector< Pointer<Shape> > Shapes::getSelected() {
  std::vector< Pointer<Shape> > array;
  for (auto it = array_.begin(); it != array_.end();) {
    if ((*it)->isSelected()) {
      array.emplace_back(it->release());
      array_.erase(it);
    } else {
      it++;
    }
  }
  return array;
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

Pointer<Shape> Shapes::Element::release() {
  return pointer_;
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
    if (array_.at(i)->isInShape(p)) {
      return i;
		}
	}
	return array_.size();
}

