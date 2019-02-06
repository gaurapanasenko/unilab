/***************************************************************
 * Name:      ShapesChilds.h
 * Purpose:   Defines shapes childs
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#ifndef SHAPESCHILDS_H
#define SHAPESCHILDS_H

#include "Shapes.h"

namespace ShapesChilds {

class Triangle : public Shape {
public:
  Triangle() {}
  const Pointer<Shape> clone();
  void drawShape(wxDC& dc);
};

const Pointer<Shape> createTriangle();

class Rectangle : public Shape {
public:
  Rectangle() {}
  const Pointer<Shape> clone();
  void drawShape(wxDC& dc);
};

const Pointer<Shape> createRectangle();

}
#endif // SHAPESCHILDS_H
