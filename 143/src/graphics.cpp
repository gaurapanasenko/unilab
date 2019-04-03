/***************************************************************
 * Name:      graphics.h
 * Purpose:   Definition of Point, Size and Sizes
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "graphics.h"
#include <cmath>
#include <cstdlib>

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
  return std::pow(point.getX(), 2.0f) + std::pow(point.getY(), 2.0f);
}

const Point abs(const Point& point) {
  return Point(std::abs(point.getX()), std::abs(point.getY()));
}

const Point floor(const Point& point) {
  return Point(std::floor(point.getX()), std::floor(point.getY()));
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

Size::Size(float x, float y)
  : Point((x > 0) ? x : 0, (y > 0) ? y : 0) {}

Size::Size(const Point& point)
  : Point((point.getX() > 0) ? point.getX() : 0,
          (point.getY() > 0) ? point.getY() : 0) {}

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
  return double(g_) / 255;
}

double Color::getB() {
  return double(b_) / 255;
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
  return std::pow(point.getX(), 2.0f)
           / std::pow(size.getX() / 2.0f, 2.0f) +
         std::pow(point.getY(), 2.0f)
           / std::pow(size.getY() / 2.0f, 2.0f);
}

/********
* Sizes *
********/

Sizes::Sizes() : minimumZoom_(0) {}

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
  defaultSize_.setX
      ((x > maximumSize_.getX()) ? maximumSize_.getX() : x);
  minimumSize_.setX(defaultSize_.getX() * minimumZoom_);
}

void Sizes::setDefaultSizeY(float y) {
  defaultSize_.setY
      ((y > maximumSize_.getY()) ? maximumSize_.getY() : y);
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
  minimumZoom_
      = (minimumZoom < 0) ? 0 : (minimumZoom > 1) ? 1 : minimumZoom;
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
