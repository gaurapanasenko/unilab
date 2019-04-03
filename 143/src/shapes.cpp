/***************************************************************
 * Name:      shapes.cpp
 * Purpose:   Definition of Shape and smart shape container
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "shapes.h"
#include <algorithm>
#include <ctime>
#include <iostream>

/*************
* ShapeTrace *
*************/
ShapeTrace::ShapeTrace(std::shared_ptr<Shape> pointer)
  : shape_(std::move(pointer)), queue_(SHAPE.getTraceSize()),
    tail_(0), time_(0) {
  if (SHAPE_DEBUG) {
    std::cout << "Created " << this << " trace\n";
  }
}

ShapeTrace::~ShapeTrace() {
  if (SHAPE_DEBUG) {
    std::cout << "Deleted " << this << " trace\n";
  }
}

std::shared_ptr<ShapeTrace> ShapeTrace::create(
    const std::shared_ptr<Shape>& pointer) {
  return std::make_shared<ShapeTrace>(pointer);
}

void ShapeTrace::draw(const Cairo::RefPtr<Cairo::Context>& context,
                      bool selected) {
  if (shape_) {
    const unsigned char& size = SHAPE.getTraceSize();
    if (size != queue_.size()) {
      queue_.resize(0);
      queue_.resize(size);
      tail_ = 0;
    }
    if ((clock() - time_) * 1000.0f / CLOCKS_PER_SEC
        > SHAPE.getTraceTime()) {
      time_ = clock();
      queue_[tail_] = shape_->clone();
      tail_ = (tail_ + 1) % size;
    }
    for (unsigned char i = 0; i < size; i++) {
      unsigned char index = (tail_ + i) % size;
      if (queue_[index]) {
        queue_[index]->draw(context, selected, 0.8f / (size - i));
      }
    }
  }
}

/********
* Shape *
********/
Shape::Shape()
  : size_(SHAPE.getDefaultSize()), zoom_(1.0),
    defaultColor_(randomColor()), color_(defaultColor_),
    visible_(true), trace_(false) {
  if (SHAPE_DEBUG) {
    std::cout << "Created " << this << "\n";
  }
}

Shape::Shape(const Shape& shape)
  : position_(shape.position_), size_(shape.size_),
    zoom_(shape.zoom_), defaultColor_(shape.defaultColor_),
    color_(shape.color_), visible_(shape.visible_),
    trace_(shape.trace_)  {
  if (SHAPE_DEBUG) {
    std::cout << "Cloned " << this << " from " << &shape << "\n";
  }
}

Shape::~Shape() {
  if (SHAPE_DEBUG) {
    std::cout << "Deleted " << this << "\n";
  }
}

void Shape::drawShape(const Cairo::RefPtr<Cairo::Context>&,
                      bool, float) {}

const std::shared_ptr<Shape> Shape::cloneVirtual() {
  return std::make_shared<Shape>(*this);
}

bool Shape::isInShapeVirtual(const Point&) const {
  return true;
}

const std::shared_ptr<Shape> Shape::clone() {
  try {
    return cloneVirtual();
  } catch (const std::bad_alloc&) {
    return {};
  }
}

void Shape::render(bool selected) {
  Point minSize = getSize();
  auto s = SHAPE.getSizes();
  if (!selected) {
    minSize = minSize * s.validateZoom(getSize(), s.getMinimumZoom());
  }
  minSize = minSize / 2;
  const float h = SHAPE.getMaximumHeight(), w = SHAPE.getMaximumWidth();

  float x = getPosition().getX(), y = getPosition().getY();
  if (x < minSize.getX())     x = minSize.getX();
  if (y < minSize.getY())     y = minSize.getY();
  if (x > w - minSize.getX()) x = w - minSize.getX();
  if (y > h - minSize.getY()) y = h - minSize.getY();

  setPosition(Point(x, y));
  if (!selected) {
    Point sz = Point(2, 2) / getSize();
    float minX = std::min(x * sz.getX(), (w - x) * sz.getX());
    float minY = std::min(y * sz.getY(), (h - y) * sz.getY());
    setZoom(std::min(std::min(minX, minY), 1.0f));
  } else {
    setZoom(1.0f);
  }
}

bool Shape::areIntersected(const std::shared_ptr<Shape>& shape,
                           bool wasIntersected) {
  if (!shape || this == shape.operator->()) return false;
  const Point position = abs(getPosition() - shape->getPosition());
  bool b = getSize().isInFrame(position);
  if (b) {
    float zoom = calculateDistanceToEllipse(position, getSize());
    if (zoom < 1.0f) {
      if (!wasIntersected) {
        color_ = randomColor();
      }
      if (zoom < getZoom()) {
        setZoom(zoom);
      }
      return true;
    }
  }
  return false;
}


void Shape::draw(const Cairo::RefPtr<Cairo::Context>& context,
                 bool selected, float alpha) {
  if (context && visible_) {
    const Point size = getSize() * zoom_ / 2;
    if (size.getX() <= 0 || size.getY() <= 0) return;
    Cairo::Matrix matrix
        (double(size.getX()), 0, 0, double(size.getY()),
         double(getPosition().getX()), double(getPosition().getY()));
    context->save();
    context->transform(matrix);
    drawShape(context, selected, alpha);
    context->set_matrix(SHAPE.getDefaultMatrix());
    context->set_source_rgba
        (double(color_.getR()) * 0.6 + 0.4,
         double(color_.getG()) * 0.4 + 0.6,
         double(color_.getB()) * 0.6 + 0.4, double(alpha));
    context->fill_preserve();
    if (selected) {
      context->set_source_rgba(0.8, 0.2, 0.2, double(alpha));
    } else {
      context->set_source_rgba(0.2, 0.8, 0.2, double(alpha));
    }
    context->stroke();
    context->restore();
  }
}

bool Shape::isInShape(const Point& point) const {
  const Point p = (point - getPosition()) / getSize() / getZoom() * 2;
  return (Size(2, 2).isInFrame(p) && isInShapeVirtual(p));
}

const Point& Shape::getPosition() const {
  return position_;
}

void Shape::setPosition(const Point& position) {
  position_ = position;
}

const Size&Shape::getSize() const {
  return size_;
}

void Shape::setSize(const Size& size) {
  size_ = SHAPE.getSizes().validateSize(size);
  setZoom(getZoom());
}

void Shape::setSizeForce(const Size& size) {
  size_ = size;
  zoom_ = 1.0;
}

float Shape::getZoom() const {
  return zoom_;
}

void Shape::setZoom(float zoom) {
  zoom_ = SHAPE.getSizes().validateZoom(size_, zoom);
}

void Shape::toggleVisibility() {
  visible_ = !visible_;
}

void Shape::changeColor() {
  color_ = randomColor();
}

void Shape::reset() {
  position_ = SHAPE.getPosition();
  size_ = SHAPE.getSizes().getDefaultSize();
  zoom_ = 1.0;
  color_ = defaultColor_;
  visible_ = true;
  trace_ = false;
}

bool Shape::hasTrace() {
  return trace_;
}

void Shape::toggleTrace() {
  trace_ = !trace_;
}

/*********
* Shapes *
*********/
Shapes::Shapes() : activated_(false), activationPoint_(0, 0) {}

std::vector<Shapes::Element>::iterator Shapes::getActiveIterator() {
  return array_.end() - ((array_.size()) ? 1 : 0);
}

std::shared_ptr<Shape> Shapes::getActive() {
  if (!array_.empty()) {
    return **(array_.end() - 1);
  }
  return std::shared_ptr<Shape>();
}

std::vector<Shapes::Element>::iterator
Shapes::getTopIterator(const Point& point) {
  for (auto i = array_.end(); i != array_.begin();) {
    i--;
    if (*i && (*i)->isInShape(point)) {
      return i;
    }
  }
  return array_.end();
}

std::shared_ptr<Shape> Shapes::getTop(const Point& point) {
  auto i = getTopIterator(point);
  if (i != array_.end()) {
    return **i;
  }
  return std::shared_ptr<Shape>();
}

void Shapes::add(const std::shared_ptr<Shape>& pointer) {
  array_.emplace_back(Element(pointer));
  for (auto& i : intersected_) {
    i.resize(array_.size(), false);
  }
  intersected_.resize
      (array_.size(), std::vector<bool>(array_.size(), false));
}

void Shapes::erase(const std::vector<Element>::iterator& iterator) {
  activated_ = false;
  if (iterator != array_.end()) {
    size_t index = iterator - array_.begin();
    if (iterator->isSelected()) {
      toggleSelection(iterator);
    }
    array_.erase(iterator);
    intersected_.erase(intersected_.begin() + index);
    for (auto& i : intersected_) {
      i.erase(i.begin() + index);
    }
  }
}

void Shapes::toggleSelection
(const std::vector<Element>::iterator& iterator) {
  if (iterator != array_.end()) {
    bool b = iterator->isSelected();
    if (!b) {
      selected_.push_back(**iterator);
    } else {
      auto it
          = std::find(selected_.begin(), selected_.end(), **iterator);
      if (it != selected_.end()) {
        selected_.erase(it);
      }
    }
    iterator->toggleSelection();
  }
}

void Shapes::activate(const Point& p) {
  auto top = getTopIterator(p);
  if (top != array_.end() && *top) {
    activated_ = true;
    activationPoint_ = floor(p - (*top)->getPosition());
    std::rotate(top, top + 1, array_.end());
  }
}

void Shapes::moveActive(const Point& p) {
  auto i = getActiveIterator();
  if (activated_ && i != array_.end() && *i) {
    (*i)->setPosition(floor(p - activationPoint_));
  }
}

void Shapes::release() {
  activated_ = false;
}

void Shapes::render() {
  for (size_t i = 0; i < array_.size(); i++) {
    auto& it = array_[i];
    auto& s = *it;
    if (s) {
      s->render(it.isSelected());
      if (!it.isSelected()) {
        for (size_t j = 0; j < array_.size(); j++) {
          auto a = intersected_[i][j];
          a = s->areIntersected(*array_[j], a);
        }
      }
    }
  }
}

void Shapes::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  render();
  for (auto& i : array_) {
    i.draw(context);
  }
}

const std::vector< std::shared_ptr<Shape> > Shapes::getSelected() {
  std::vector< std::shared_ptr<Shape> > array;
  for (auto& i : array_) {
    if (i && i.isSelected()) {
      array.emplace_back(i->clone());
      i.toggleSelection();
    }
  }
  return array;
}

/******************
* Shapes::Element *
******************/
Shapes::Element::Element(std::shared_ptr<Shape> pointer)
  : pointer_(std::move(pointer)), selected_(false) {}

const std::shared_ptr<Shape>& Shapes::Element::operator*() {
  return pointer_;
}

Shape* Shapes::Element::operator->() {
  return pointer_.operator->();
}

void
Shapes::Element::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  if (pointer_) {
    if (pointer_->hasTrace()) {
      if (shapeTrace_) {
        shapeTrace_->draw(context, isSelected());
      } else {
        shapeTrace_ = ShapeTrace::create(pointer_);
        shapeTrace_->draw(context, isSelected());
      }
    } else if (shapeTrace_) {
      shapeTrace_.reset();
    }
    pointer_->draw(context, isSelected());
  }
}

Shapes::Element::operator bool() const {
  return bool(pointer_);
}

bool Shapes::Element::isSelected() const {
  return selected_;
}

void Shapes::Element::toggleSelection() {
  selected_ = !selected_;
}
