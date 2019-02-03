/***************************************************************
 * Name:      Shapes.cpp
 * Purpose:   Implementation of shapes
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#include "Shapes.h"

/// \brief Creates random color
/// \return colors
wxColor randomColor() {
  return wxColour(rand() % 256, rand() % 256, rand() % 256);
}

ShapeTrace::ShapeTrace() : shape_(0), tail_(0), time_(0) {}

ShapeTrace& ShapeTrace::operator=(Shape& pointer) {
  if (shape_ == &pointer) return *this;
  shape_ = &pointer;
  tail_ = 0;
  time_ = 0;
  for (unsigned char i = 0; i < SHAPE_TRACE_SIZE; i++) {
    queue_[i] = pointer.clone();
    queue_[i]->setFrame(wxPoint(-2,-2), wxPoint(-2,-2));
  }
  return *this;
}

void ShapeTrace::draw(wxDC& dc) {
  if (shape_->hasTrace()) {
    if (((double)(clock() - time_)) /
        CLOCKS_PER_SEC > SHAPE_TRACE_TIME) {
      time_ = clock();
      *queue_[tail_] = *shape_;
      tail_++;
      if (tail_ >= SHAPE_TRACE_SIZE) tail_ = 0;
    }
    for (unsigned char i = 0; i < SHAPE_TRACE_SIZE; i++) {
      unsigned char index = tail_ + i;
      while (index >= SHAPE_TRACE_SIZE) index -= SHAPE_TRACE_SIZE;
      queue_[index]->draw(dc, 200 / (SHAPE_TRACE_SIZE - i));
    }
  }
}




Shape::Shape() : defaultColor_(randomColor()), color_(defaultColor_),
visible_(1), trace_(0) {
  setFrame(wxPoint(0, 0), wxPoint(SHAPE_DEFAULT_WIDTH,
                                  SHAPE_DEFAULT_HEIGHT));
}

Shape::~Shape() {
}

void Shape::drawShape(wxDC& dc) {}

const Pointer<Shape> Shape::clone() {
  return Pointer<Shape>(new Shape(*this));
}

void Shape::setFrame(const wxPoint& one, const wxPoint& two) {
  point1 = wxPoint(min(one.x, two.x), min(one.y, two.y));
  point2 = wxPoint(max(one.x, two.x), max(one.y, two.y));
  position_ = (point1 + point2) / 2;
  defaultSize_ = point2 - point1;
}

void Shape::render(wxDC& dc) {
  int x = position_.x, y = position_.y;
  wxSize sz = dc.GetSize();
  int h = sz.GetHeight(), w = sz.GetWidth();

  if (x < 10)     x = 10;
  if (y < 10)     y = 10;
  if (x > w - 10) x = w;
  if (y > h - 10) y = h;
  moveTo(wxPoint(x, y));
  if (point1.x < 1)      point1.x = 1;
  if (point1.y < 1)      point1.y = 1;
  if (point2.x > w - 1)  point2.x = w - 1;
  if (point2.y > h - 1)  point2.y = h - 1;
}

void Shape::draw(wxDC& dc, unsigned char alpha) {
  if (visible_) {
    dc.SetBrush(wxBrush(wxColor(color_.Red(), color_.Green(),
                                color_.Blue(), alpha)));
    dc.SetPen(wxPen(wxColor(0, 0, 0, alpha), 2));
    drawShape(dc);
  }
}

void Shape::moveTo(const wxPoint& p) {
  wxPoint s = defaultSize_ / 2;

  position_ = p;
  point1 = p - s;
  point2 = p + s;
}

const wxPoint& Shape::getPosition() {
  return position_;
}

bool Shape::isInShape(const wxPoint& p) {
  return (point1.x <= p.x && p.x <= point2.x &&
          point1.y <= p.y && p.y <= point2.y);
}

void Shape::toggleVisibility() {
  visible_ = (visible_) ? 0 : 1;
}

void Shape::changeColor() {
  color_ = randomColor();
}

void Shape::resetColor() {
  color_ = defaultColor_;
}

bool Shape::hasTrace() {
  return trace_;
}

void Shape::toggleTrace() {
  trace_ = (trace_) ? 0 : 1;
}

void Shape::areIntersected(Shape& shape) {
  if (this == &shape) return;
  wxPoint pos  =      (position_ + shape.position_)   / 2;
  wxPoint s1p1 =       position_ -       defaultSize_ / 2;
  wxPoint s1p2 =       position_ +       defaultSize_ / 2;
  wxPoint s2p1 = shape.position_ - shape.defaultSize_ / 2;
  wxPoint s2p2 = shape.position_ + shape.defaultSize_ / 2;
     bool b1   = s2p1.x <= s1p1.x && s1p1.x <= s2p2.x &&
                 s2p1.y <= s1p1.y && s1p1.y <= s2p2.y;
     bool b2   = s2p1.x <= s1p1.x && s1p1.x <= s2p2.x &&
                 s2p1.y <= s1p2.y && s1p2.y <= s2p2.y;
     bool b3   = s2p1.x <= s1p2.x && s1p2.x <= s2p2.x &&
                 s2p1.y <= s1p2.y && s1p2.y <= s2p2.y;
     bool b4   = s2p1.x <= s1p2.x && s1p2.x <= s2p2.x &&
                 s2p1.y <= s1p1.y && s1p1.y <= s2p2.y;
  if (b1 || b2 || b3 || b4) {
    size_t index;
    if (!intersected_.find(&shape, index)) {
      color_ = randomColor();
      shape.color_ = randomColor();
      intersected_.add(&shape);
      if (!shape.intersected_.find(this, index))
        shape.intersected_.add(this);
    }
    /*if (b1) {

    }
    if (b2) {
      point1.x = pos.x;       point2.y = pos.y;
      shape.point2.x = pos.x; shape.point1.y = pos.y;
    }*/
  } else {
    size_t index;
    if (intersected_.find(&shape, index))
      intersected_.erase(index);
    if (shape.intersected_.find(this, index))
      shape.intersected_.erase(index);
  }
}

Shapes::Shapes() : array_(), activeId_(0), activated_(0),
activationPoint_(0, 0) {}

Shape& Shapes::getActive() {
  return *array_[activeId_];
}

const size_t& Shapes::getActiveId() {
  return activeId_;
}

void Shapes::add(const Pointer<Shape>& item) {
  array_.add(Element());
  array_[array_.size() - 1] = item;
}

void Shapes::erase(const size_t& index) {
  array_.erase(index);
}

void Shapes::activate(const wxPoint& p) {
  if (array_.size() < 1) return;
  size_t i = array_.size();
  while(i > 0) {
    i--;
    if (array_[i]->isInShape(p)) {
      activeId_ = i;
      activated_ = 1;
      activationPoint_ = p - array_[i]->getPosition();
      break;
    }
  }
}

void Shapes::moveActive(const wxPoint& p) {
  if (activated_ && activeId_ < array_.size()) {
    array_[activeId_]->moveTo(p - activationPoint_);
  }
}

void Shapes::release() {
  activated_ = 0;
}

void Shapes::draw(wxDC& dc) {
  for (size_t i = 0; i < array_.size(); i++) {
    array_[i]->render(dc);
  }
  for (size_t i = 0; i < array_.size(); i++) {
    for (size_t j = 0; j < array_.size(); j++) {
      array_[i]->areIntersected(*array_[j]);
    }
  }
  for (size_t i = 0; i < array_.size(); i++) {
    array_[i].draw(dc);
  }
}

Shapes::Element&
Shapes::Element::operator=(const Pointer<Shape>& pointer) {
  pointer_ = pointer;
  shapeTrace_ = *pointer_;
  return (*this);
}

Shape& Shapes::Element::operator*() {
  return *pointer_;
}

Shape* Shapes::Element::operator->() {
  return &pointer_;
}


void Shapes::Element::draw(wxDC& dc) {
  shapeTrace_.draw(dc);
  pointer_->draw(dc);
}
