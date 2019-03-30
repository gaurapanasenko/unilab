/***************************************************************
 * Name:      shape-childs.cpp
 * Purpose:   Implementation of shapes childs
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#include "shape-childs.h"

namespace ShapeChilds {

const Pointer<Shape> Triangle::create() {
	return Pointer<Shape>(new Triangle);
}

const Pointer<Shape> Triangle::clone() {
	return Pointer<Shape>(new Triangle(*this));
}

void Triangle::drawShape(const Cairo::RefPtr<Cairo::Context>& context) {
	context->move_to(-1.0,  1.0);
	context->line_to( 0.0, -1.0);
	context->line_to( 1.0,  1.0);
	context->line_to(-1.0,  1.0);
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


const Pointer<Shape> Rectangle::create() {
	return Pointer<Shape>(new Rectangle);
}

const Pointer<Shape> Rectangle::clone() {
	return Pointer<Shape>(new Rectangle(*this));
}

void Rectangle::drawShape(const Cairo::RefPtr<Cairo::Context>& context) {
	context->move_to(-1.0, -1.0);
	context->line_to( 1.0, -1.0);
	context->line_to( 1.0,  1.0);
	context->line_to(-1.0,  1.0);
	context->line_to(-1.0, -1.0);
}

bool Rectangle::isInShapeVirtual(const Point&) const {
	return true;
}



const Pointer<Shape> Ellipse::create() {
	return Pointer<Shape>(new Ellipse);
}

const Pointer<Shape> Ellipse::clone() {
	return Pointer<Shape>(new Ellipse(*this));
}

void Ellipse::drawShape(const Cairo::RefPtr<Cairo::Context>& context) {
	context->arc(0, 0, 1, 0, 2 * M_PI);
}

bool Ellipse::isInShapeVirtual(const Point& p) const {
  return calculateDistanceToEllipse(p, Size(2, 2)) < 1;
}

Aggregator::Aggregator(const std::vector< Pointer<Shape> >& array)
  : array_(array) {
  for (auto& i : array_) {
    i->setPosition(i->getPosition() * 0.02f - 0.5);
  }
}

const Pointer<Shape> Aggregator::create(
  const std::vector< Pointer<Shape> >& array
) {
  return Pointer<Shape>(new Aggregator(array));
}

const Pointer<Shape> Aggregator::clone() {
	return Pointer<Shape>(new Aggregator(*this));
}

void Aggregator::drawShape(const Cairo::RefPtr<Cairo::Context>& context) {
  for (auto& i : array_) {
    i->draw(context);
  }
}

bool Aggregator::isInShapeVirtual(const Point&) const {
	return true;
}

}
