/***************************************************************
 * Name:      shape-childs.h
 * Purpose:   Declaration on shapes childs
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#ifndef SHAPECHILDS_H
#define SHAPECHILDS_H

#include "shapes.h"

namespace ShapeChilds {

class Triangle : public Shape {
public:
  Triangle() = default;
  static const Glib::RefPtr<Shape> create();
  const Glib::RefPtr<Shape> clone() override;
  void drawShape(
    const Cairo::RefPtr<Cairo::Context>& context, float alpha = 0.8f
  ) override;
  bool isInShapeVirtual(const Point& p) const override;
};


class Rectangle : public Shape {
public:
  Rectangle() = default;
  static const Glib::RefPtr<Shape> create();
  const Glib::RefPtr<Shape> clone() override;
  void drawShape(
    const Cairo::RefPtr<Cairo::Context>& context, float alpha = 0.8f
  ) override;
  bool isInShapeVirtual(const Point& p) const override;
};


class Ellipse : public Shape {
public:
  Ellipse() = default;
  static const Glib::RefPtr<Shape> create();
  const Glib::RefPtr<Shape> clone() override;
  void drawShape(
    const Cairo::RefPtr<Cairo::Context>& context, float alpha = 0.8f
  ) override;
  bool isInShapeVirtual(const Point& p) const override;
};

}

#endif // SHAPECHILDS_H
