/***************************************************************
 * Name:      shape-childs.cpp
 * Purpose:   Definition of shapes childs
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "shape-childs.h"

namespace ShapeChilds {

/***********
* Triangle *
***********/
const Glib::RefPtr<Shape> Triangle::create() {
  return Glib::RefPtr<Shape>(new Triangle);
}

const Glib::RefPtr<Shape> Triangle::clone() {
  return Glib::RefPtr<Shape>(new Triangle(*this));
}

void Triangle::drawShape(const Cairo::RefPtr<Cairo::Context>& context, float) {
  if (context) {
    context->move_to(-1.0,  1.0);
    context->line_to( 0.0, -1.0);
    context->line_to( 1.0,  1.0);
    context->line_to(-1.0,  1.0);
  }
}

bool Triangle::isInShapeVirtual(const Point& p) const {
  Point a = Point(-1,  1),
        b = Point( 0, -1),
        c = Point( 1,  1),
        d = p;
	return isOneSizePointsToStraight(a, b, c, d) &&
	       isOneSizePointsToStraight(b, c, a, d) &&
	       isOneSizePointsToStraight(c, a, b, d);
}

/************
* Rectangle *
************/
const Glib::RefPtr<Shape> Rectangle::create() {
  return Glib::RefPtr<Shape>(new Rectangle);
}

const Glib::RefPtr<Shape> Rectangle::clone() {
  return Glib::RefPtr<Shape>(new Rectangle(*this));
}

void Rectangle::drawShape(const Cairo::RefPtr<Cairo::Context>& context, float) {
  if (context) {
    context->rectangle(-1, -1, 2, 2);
  }
}

bool Rectangle::isInShapeVirtual(const Point&) const {
	return true;
}

/**********
* Ellipse *
**********/
const Glib::RefPtr<Shape> Ellipse::create() {
  return Glib::RefPtr<Shape>(new Ellipse);
}

const Glib::RefPtr<Shape> Ellipse::clone() {
  return Glib::RefPtr<Shape>(new Ellipse(*this));
}

void Ellipse::drawShape(const Cairo::RefPtr<Cairo::Context>& context, float) {
  if (context) {
    context->arc(0, 0, 1, 0, 2 * M_PI);
  }
}

bool Ellipse::isInShapeVirtual(const Point& p) const {
  return calculateDistanceToEllipse(p, Size(2, 2)) < 1;
}

}
