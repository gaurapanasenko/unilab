/***************************************************************
 * Name:      shape-parameters.h
 * Purpose:   Declaration of ShapeParameters
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#ifndef SHAPE_PARAMETERS_H
#define SHAPE_PARAMETERS_H

#include "graphics.h"
#include <cairomm/context.h>

class Shapes;

class ShapeParameters {
public:
  ShapeParameters();
  const Point& getPosition() const;
  void setPosition(const Point& position);
  float getX();
  void setX(float x);
  float getY();
  void setY(float y);
  const Sizes& getSizes() const;
  const Size& getDefaultSize() const;
  void setMinimumZoom(float minimumZoom);
  float getDefaultWidth();
  void setDefaultWidth(float width);
  float getDefaultHeight();
  void setDefaultHeight(float height);
  float getMaximumWidth();
  void setMaximumWidth(float width);
  float getMaximumHeight();
  void setMaximumHeight(float height);
  unsigned char getTraceSize();
  void setTraceSize(unsigned char traceSize);
  float getTraceTime();
  void setTraceTime(float traceTime);
  const Cairo::Matrix& getDefaultMatrix();
  void setDefaultMatrix(const Cairo::Matrix& defaultMatrix);

private:
  /// \brief default position
  Point position_;
  /// \brief draw area sizes
  Sizes sizes_;
  /// \brief number of cloned objects when doing trace
  unsigned char traceSize_;
  /// \brief interval between saving state of Shape object
  /// when doing trace
  float traceTime_;
  Cairo::Matrix defaultMatrix_;
};

extern ShapeParameters SHAPE;

#endif // SHAPE_PARAMETERS_H
