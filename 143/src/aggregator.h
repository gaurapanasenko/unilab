/***************************************************************
 * Name:      aggregator.h
 * Purpose:   Declaration of Aggregator
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include "shapes.h"

class Aggregator : public Shape {
public:
  Aggregator(std::vector< Glib::RefPtr<Shape> > array);
  Aggregator(const Aggregator& object);
  Aggregator(Aggregator&& object) = default;
  ~Aggregator() override = default;

  Aggregator& operator=(const Aggregator& object);
  Aggregator& operator=(Aggregator&& object) = default;

  static const Glib::RefPtr<Shape> create(
    const std::vector< Glib::RefPtr<Shape> >& array
  );
  const Glib::RefPtr<Shape> clone() override;
  void drawShape(
    const Cairo::RefPtr<Cairo::Context>& context, float alpha = 0.8f
  ) override;
  bool isInShapeVirtual(const Point& p) const override;
  void toggleSelectionVirtual() override;

  const std::vector< Glib::RefPtr<Shape> > deaggregate();

private:
  std::vector< Glib::RefPtr<Shape> > array_;
};

#endif // AGGREGATOR_H
