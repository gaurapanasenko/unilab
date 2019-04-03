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
  static const std::shared_ptr<Shape> create();
  const std::shared_ptr<Shape> clone() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context,
                 bool selected, float alpha = 0.8f) override;
  bool isInShapeVirtual(const Point& p) const override;
};


class Rectangle : public Shape {
public:
  static const std::shared_ptr<Shape> create();
  const std::shared_ptr<Shape> clone() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context,
                 bool selected, float alpha = 0.8f) override;
  bool isInShapeVirtual(const Point& p) const override;
};


class Ellipse : public Shape {
public:
  static const std::shared_ptr<Shape> create();
  const std::shared_ptr<Shape> clone() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context,
                 bool selected, float alpha = 0.8f) override;
  bool isInShapeVirtual(const Point& p) const override;
};

}

#endif // SHAPECHILDS_H
