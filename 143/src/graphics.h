/***************************************************************
 * Name:      graphics.h
 * Purpose:   Declaration of Point, Size and Sizes
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <iostream>

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
  float x_, y_;
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
float calculatePseudoscalarProduct(const Point& a, const Point& b,
                                   const Point& c);

/// \brief Detects that points C and D is on one side
/// relatively to straight line AB
/// \param a point A
/// \param b point B
/// \param c point C
/// \param d point D
/// \return True if points on one side, else false
bool isOneSizePointsToStraight(const Point& a, const Point& b,
                               const Point& c, const Point& d);

std::ostream& operator <<(std::ostream& out, const Point& rhs);
std::istream& operator >>(std::istream& in,  Point& rhs);

class Size : public Point {
public:
  Size();
  Size(float x, float y);
  Size(const Point& point);
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
  double getR() const;
  double getG() const;
  double getB() const;
  friend std::ostream& operator <<(std::ostream& out, const Color& rhs);
  friend std::istream& operator >>(std::istream& in,  Color& rhs);

private:
  unsigned char r_, g_, b_;
};

Color randomColor();

/// \brief The function converts the plane so that the
/// ellipse turns into a circle of radius 1 in the center of the
/// coordinate grid, and then the function gives the distance to the
/// center of the coordinate grid
/// \param point checking point
/// \param size size of ellipse
/// \return distance
float calculateDistanceToEllipse(const Point& point, const Point& size);


class Sizes {
public:
  Sizes();
  const Size& getMinimumSize() const;
  const Size& getDefaultSize() const;
  void setDefaultSize(const Size& size);
  void setDefaultSizeX(float x);
  void setDefaultSizeY(float y);
  const Size& getMaximumSize() const;
  void setMaximumSize(const Size& size);
  void setMaximumSizeX(float x);
  void setMaximumSizeY(float y);
  float getMinimumZoom() const;
  void setMinimumZoom(float minimumZoom);

  unsigned char checkSize(const Size& size) const;
  Size validateSize(const Size& size) const;
  float validateZoom(const Size& size, float zoom) const;

private:
  Size minimumSize_;
  Size defaultSize_;
  Size maximumSize_;
  float minimumZoom_;
};

#endif // GRAPHICS_H
