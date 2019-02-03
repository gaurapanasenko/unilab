/***************************************************************
 * Name:      ShapesChilds.cpp
 * Purpose:   Implementation of shapes childs
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#include "ShapesChilds.h"

const Pointer<Shape> Triangle::clone() {
  return Pointer<Shape>(new Triangle(*this));
}

void Triangle::drawShape(wxDC& dc) {
  wxPoint size = point2 - point1;
  wxPoint p[] = {
    wxPoint(0.0 * size.x, 1.0 * size.y),
    wxPoint(0.5 * size.x, 0.0 * size.y),
    wxPoint(1.0 * size.x, 1.0 * size.y)
  };
  dc.DrawPolygon(WXSIZEOF(p), p, point1.x, point1.y);
}

const Pointer<Shape> createTriangle() {
  return Pointer<Shape>(new Triangle);
}

const Pointer<Shape> Rectangle::clone() {
  return Pointer<Shape>(new Rectangle(*this));
}

void Rectangle::drawShape(wxDC& dc) {
  wxPoint size = point2 - point1;
  wxPoint p[] = {
    wxPoint(0.0 * size.x, 0.0 * size.y),
    wxPoint(1.0 * size.x, 0.0 * size.y),
    wxPoint(1.0 * size.x, 1.0 * size.y),
    wxPoint(0.0 * size.x, 1.0 * size.y)
  };
  dc.DrawPolygon(WXSIZEOF(p), p, point1.x, point1.y);
}

const Pointer<Shape> createRectangle() {
  return Pointer<Shape>(new Rectangle);
}
