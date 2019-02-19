/***************************************************************
 * Name:      ShapeChilds.h
 * Purpose:   Defines shapes childs
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#ifndef SHAPECHILDS_H
#define SHAPECHILDS_H

#include "Shapes.h"

namespace ShapeChilds {

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
#endif // SHAPECHILDS_H
