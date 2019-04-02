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

/*************
* ShapeTrace *
*************/
ShapeTrace::ShapeTrace()
  : shape_(nullptr), tail_(0), time_(0) {}

ShapeTrace& ShapeTrace::operator=(const Glib::RefPtr<Shape>& pointer) {
  if (shape_ == pointer) return *this;
  shape_ = pointer;
	tail_ = 0;
	time_ = 0;
  queue_.resize(0);
	return *this;
}

void ShapeTrace::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  if (shape_ && shape_->hasTrace()) {
		const unsigned char& size = SHAPE.getTraceSize();
		if (size != queue_.size()) {
			queue_.resize(0);
			queue_.resize(size);
			tail_ = 0;
		}
		if (
      (clock() - time_) * 1000.0f / CLOCKS_PER_SEC > SHAPE.getTraceTime()
		) {
			time_ = clock();
			queue_[tail_] = shape_->clone();
			tail_++;
			while (tail_ >= size) tail_ -= size;
		}
		for (unsigned char i = 0; i < size; i++) {
			unsigned char index = tail_ + i;
			while (index >= size) index -= size;
			if (queue_[index])
        queue_[index]->draw(context, 0.8f / (size - i));
		}
  } else if (!queue_.empty()) {
		queue_.resize(0);
		tail_ = 0;
	}
}

/********
* Shape *
********/
Shape::Shape() : referenced_(0), size_(SHAPE.getDefaultSize()), zoom_(1.0),
  defaultColor_(randomColor()), color_(defaultColor_),
  visible_(true), trace_(false), selected_(false) {}

void Shape::reference() {
  referenced_++;
}

void Shape::unreference() {
  referenced_--;
  if (!referenced_) {
    delete this;
  }
}

void Shape::drawShape(const Cairo::RefPtr<Cairo::Context>&, float) {}

const Glib::RefPtr<Shape> Shape::clone() {
  return Glib::RefPtr<Shape>(new Shape());
}

bool Shape::isInShapeVirtual(const Point&) const {
  return true;
}

void Shape::toggleSelectionVirtual() {}

void Shape::render() {
  Point minSize = (
    (isSelected()) ? getSize() :
      getSize() * SHAPE.getSizes().validateZoom(
        getSize(), SHAPE.getSizes().getMinimumZoom()
      )
  ) / 2;
  const float h = SHAPE.getMaximumHeight(), w = SHAPE.getMaximumWidth();

  float x = getPosition().getX(), y = getPosition().getY();
  if (x < minSize.getX())     x = minSize.getX();
  if (y < minSize.getY())     y = minSize.getY();
  if (x > w - minSize.getX()) x = w - minSize.getX();
  if (y > h - minSize.getY()) y = h - minSize.getY();

	setPosition(Point(x, y));
	if (!isSelected()) {
    setZoom(std::min(std::min(
      std::min(
        x * 2.0f / getSize().getX(), (w - x) * 2.0f / getSize().getX()
      ),
      std::min(
        y * 2.0f / getSize().getY(), (h - y) * 2.0f / getSize().getY()
      )
    ), 1.0f));
	} else {
    setZoom(1.0f);
	}
}

void Shape::areIntersected(const Glib::RefPtr<Shape>& shape) {
  if (!shape || this == shape.operator->()) return;
	const Point position = abs(
    getPosition() - shape->getPosition()
  );
  bool b = getSize().isInFrame(position);
	if (b) {
		float zoom = calculateDistanceToEllipse(
      position, getSize()
		);
    if (zoom < 1.0f) {
      auto it = std::find(intersected_.begin(), intersected_.end(), shape);
      if (it == intersected_.end()) {
				color_ = randomColor();
        intersected_.emplace_back(shape);
			}
      if (zoom < getZoom())
        setZoom(zoom);
			return;
		}
	}
  auto it = std::find(intersected_.begin(), intersected_.end(), shape);
  if (it != intersected_.end())
    intersected_.erase(it);
}


void Shape::draw(
	const Cairo::RefPtr<Cairo::Context>& context,
	float alpha
) {
  if (context && visible_) {
    const Point size = getSize() * zoom_ / 2;
    if (size.getX() <= 0 || size.getY() <= 0) return;
		Cairo::Matrix matrix(
      double(size.getX()), 0, 0, double(size.getY()),
      double(getPosition().getX()), double(getPosition().getY())
    );
    context->save();
		context->transform(matrix);
    drawShape(context, alpha);
		context->set_matrix(SHAPE.getDefaultMatrix());
		context->set_source_rgba(
      double(color_.getR()) * 0.6 + 0.4, double(color_.getG()) * 0.4 + 0.6,
      double(color_.getB()) * 0.6 + 0.4, double(alpha)
		);
		context->fill_preserve();
		if (isSelected()) {
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

bool Shape::isSelected() {
	return selected_;
}

void Shape::toggleSelection() {
  selected_ = !selected_;
  toggleSelectionVirtual();
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
  selected_ = false;
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

Glib::RefPtr<Shape> Shapes::getActive() {
  return **(array_.end() - ((array_.size()) ? 1 : 0));
}

std::vector<Shapes::Element>::iterator Shapes::getActiveIterator() {
  return array_.end() - ((array_.size()) ? 1 : 0);
}

void Shapes::add(const Glib::RefPtr<Shape>& item) {
  array_.emplace_back(Element());
  array_.at(array_.size() - 1) = item;
}

void Shapes::erase(const std::vector<Element>::iterator& iterator) {
  activated_ = false;
  if (iterator != array_.end())
    array_.erase(iterator);
}

Glib::RefPtr<Shape> Shapes::getTop(const Point& p) {
  return **getTopIterator(p);
}

void Shapes::activate(const Point& p) {
  auto top = getTopIterator(p);
  if (top != array_.end() && (*top)) {
    activated_ = true;
    activationPoint_ = floor(p - (*top)->getPosition());
    std::rotate(top, top + 1, array_.end());
	}
}

void Shapes::moveActive(const Point& p) {
  auto i = getActiveIterator();
  if (activated_ && i != array_.end()) {
    (*i)->setPosition(floor(p - activationPoint_));
	}
}

void Shapes::release() {
  activated_ = false;
}

void Shapes::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  for (auto& i : array_) {
    if (i) {
    i->render();
      if (!i->isSelected()) {
        for (auto& j : array_) {
          i->areIntersected(*j);
        }
      }
    }
  }
  for (auto& i : array_) {
    i.draw(context);
  }
}

const std::vector< Glib::RefPtr<Shape> > Shapes::getSelected() {
  std::vector< Glib::RefPtr<Shape> > array;
  for (auto& i : array_) {
    if (i) {
      if (i->isSelected()) {
        array.emplace_back(i->clone());
        i->toggleSelection();
      }
    }
  }
  return array;
}

std::vector<Shapes::Element>::iterator Shapes::getTopIterator(const Point& p) {
  for (auto i = array_.end(); i != array_.begin();) {
    i--;
    if (*i && (*i)->isInShape(p)) {
      return i;
    }
  }
  return array_.end();
}

/******************
* Shapes::Element *
******************/
Shapes::Element&
Shapes::Element::operator=(const Glib::RefPtr<Shape>& pointer) {
  pointer_ = pointer;
  shapeTrace_ = pointer_;
  return (*this);
}

const Glib::RefPtr<Shape>& Shapes::Element::operator*() {
  return pointer_;
}

Shape* Shapes::Element::operator->() {
  return pointer_.operator->();
}

Glib::RefPtr<Shape> Shapes::Element::release() {
  return pointer_;
}

void Shapes::Element::draw(
  const Cairo::RefPtr<Cairo::Context>& context
) {
  if (pointer_) {
    shapeTrace_.draw(context);
    pointer_->draw(context);
  }
}

Shapes::Element::operator bool() const {
  return bool(pointer_);
}
