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
  Point mp1(0, 0), mp2(0, 0);
  bool first = 1;
  for (auto& i : array_) {
    Point p1 = i->getPosition() - i->getSize() / 2,
          p2 = i->getPosition() + i->getSize() / 2;
    if (first) {
      mp1 = p1; mp2 = p2;
      first = 0;
    }
    if (p1.getX() < mp1.getX()) {
      mp1.setX(p1.getX());
    }
    if (p1.getY() < mp1.getY()) {
      mp1.setY(p1.getY());
    }
    if (p2.getX() > mp2.getX()) {
      mp2.setX(p2.getX());
    }
    if (p2.getY() > mp2.getY()) {
      mp2.setY(p2.getY());
    }
  }
  Point position = (mp1 + mp2) / 2,
            size =  mp2 - mp1;

  setSize(size);
  setPosition(floor(position));
  for (auto& i : array_) {
    i->setSizeForce(i->getSize() * 2.0f / size);
    i->setPosition((i->getPosition() - position) * 2.0f / size);
    if (i->isSelected())
      i->toggleSelection();
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

bool Aggregator::isInShapeVirtual(const Point& point) const {
  for (auto& i : array_) {
    if (i->isInShape(point))
      return true;
  }
  return false;
}

void Aggregator::toggleSelectionVirtual() {
  for (auto& i : array_) {
    i->toggleSelection();
  }
}

const std::vector< Pointer<Shape> > Aggregator::deaggregate() {
  auto arr(std::move(array_));
  for (auto& i : arr) {
    i->setSizeForce(i->getSize() * getSize() / 2.0f);
    i->setPosition(i->getPosition() * getSize() / 2.0f + getPosition());
    if (i->isSelected())
      i->toggleSelection();
  }
  return arr;
}

}
