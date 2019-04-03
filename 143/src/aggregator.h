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
  Aggregator(std::vector< std::shared_ptr<Shape> > array);
  Aggregator(const Aggregator& object);
  Aggregator(Aggregator&& object) = default;
  ~Aggregator() override = default;

  Aggregator& operator=(const Aggregator& object);
  Aggregator& operator=(Aggregator&& object) = default;

  static const std::shared_ptr<Shape>
  create(const std::vector< std::shared_ptr<Shape> >& array);

  const std::shared_ptr<Shape> cloneVirtual() override;
  void drawShape(const Cairo::RefPtr<Cairo::Context>& context,
                 bool selected, float alpha = 0.8f) override;
  bool isInShapeVirtual(const Point& p) const override;

  const std::vector< std::shared_ptr<Shape> > deaggregate();

private:
  std::vector< std::shared_ptr<Shape> > array_;
};

#endif // AGGREGATOR_H
