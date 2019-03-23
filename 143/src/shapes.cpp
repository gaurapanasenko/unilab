/***************************************************************
 * Name:      shapes.cpp
 * Purpose:   Implementation of shapes
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#include "shapes.h"

Point::Point() : Gdk::Point() {}

Point::Point(int x, int y) : Gdk::Point(x, y) {}

Point& Point::operator=(const Point& rhs) {
	set_x(rhs.get_x());
	set_y(rhs.get_y());
	return (*this);
}

Point operator+(const Point& lhs, const Point& rhs) {
	return Point(lhs.get_x() + rhs.get_x(), lhs.get_y() + rhs.get_y());
}

Point operator-(const Point& lhs, const Point& rhs) {
	return Point(lhs.get_x() - rhs.get_x(), lhs.get_y() - rhs.get_y());
}

Point operator*(const Point& lhs, const int    rhs) {
	return Point(lhs.get_x() * rhs, lhs.get_y() * rhs);
}

Point operator/(const Point& lhs, const int    rhs) {
	return Point(lhs.get_x() / rhs, lhs.get_y() / rhs);
}

const double calulateVectorLength(const Point& point) {
	return sqrt(pow(point.get_x(), 2.0) + pow(point.get_y(), 2.0));
}

Color::Color() : color_{0, 0, 0} {}

Color::Color(unsigned char r, unsigned char g, unsigned char b) :
color_{r, g, b} {}

const double Color::operator[](unsigned char index) {
	return double(color_[index]) / 255;
}

unsigned char& Color::operator()(unsigned char index) {
	return color_[index];
}

void Color::applyToContext(const Cairo::RefPtr<Cairo::Context>& context) {
	context->set_source_rgb((*this)[0], (*this)[1], (*this)[2]);
}

/// \brief Creates random color
/// \return colors
Color randomColor() {
	return Color(rand() % 256, rand() % 256, rand() % 256);
}

ShapeTrace::ShapeTrace() : shape_(0), tail_(0), time_(0) {}

ShapeTrace& ShapeTrace::operator=(Shape& pointer) {
	if (shape_ == &pointer) return *this;
	shape_ = &pointer;
	tail_ = 0;
	time_ = 0;
	for (unsigned char i = 0; i < SHAPE_TRACE_SIZE; i++) {
		queue_[i] = pointer.clone();
		queue_[i]->setFrame(Point(-2,-2), Point(-2,-2));
	}
	return *this;
}

void ShapeTrace::draw(const Cairo::RefPtr<Cairo::Context>& context) {
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
			queue_[index]->draw(context, 0.75 / (SHAPE_TRACE_SIZE - i));
		}
	}
}




Shape::Shape() : defaultColor_(randomColor()), color_(defaultColor_),
visible_(1), trace_(0) {
	setFrame(Point(0, 0), Point(SHAPE_DEFAULT_WIDTH, SHAPE_DEFAULT_HEIGHT));
}

Shape::~Shape() {
}

void Shape::drawShape(const Cairo::RefPtr<Cairo::Context>& context) {}

const Pointer<Shape> Shape::clone() {
	return Pointer<Shape>(new Shape(*this));
}

bool Shape::isInShapeVirtual(const Point& p) {
	return true;
}

void Shape::setFrame(const Point& one, const Point& two) {
	point1 = Point(min(one.get_x(), two.get_x()), min(one.get_y(), two.get_y()));
	point2 = Point(max(one.get_x(), two.get_x()), max(one.get_y(), two.get_y()));
	position_ = (point1 + point2) / 2;
	defaultSize_ = point2 - point1;
}

void Shape::render(const Gtk::Allocation& allocation) {
	int x = position_.get_x(), y = position_.get_y();
	int h = allocation.get_height(), w = allocation.get_width();

	if (x < 10)     x = 10;
	if (y < 10)     y = 10;
	if (x > w - 10) x = w;
	if (y > h - 10) y = h;
	moveTo(Point(x, y));
	if (point1.get_x() < 1)      point1.set_x(1);
	if (point1.get_y() < 1)      point1.set_y(1);
	if (point2.get_x() > w - 1)  point2.set_x(w - 1);
	if (point2.get_y() > h - 1)  point2.set_y(h - 1);
}

void Shape::draw(
  const Cairo::RefPtr<Cairo::Context>& context,
  double alpha
) {
	if (visible_) {
		Point size = point2 - point1;
		Point position = (point2 + point1) / 2;
		if (size.get_x() == 0 || size.get_y() == 0) return;
		auto matrix = Cairo::Matrix(
			size.get_x() / 2, 0, 0, size.get_y() / 2,
			position.get_x(), position.get_y()
		);
		context->transform(matrix);
		drawShape(context);
		matrix.invert();
		context->transform(matrix);
		context->set_source_rgba(color_[0], color_[1], color_[2], alpha);
		context->fill_preserve();
		context->set_source_rgba(0, 0, 0, alpha);
		context->stroke();
	}
}

void Shape::moveTo(const Point& p) {
	Point s = defaultSize_ / 2;

	position_ = p;
	point1 = p - s;
	point2 = p + s;
}

const Point& Shape::getPosition() {
	return position_;
}

bool Shape::isInShape(const Point& p) {
	return (
		point1.get_x() <= p.get_x() && p.get_x() <= point2.get_x() &&
		point1.get_y() <= p.get_y() && p.get_y() <= point2.get_y() &&
		isInShapeVirtual(p)
	);
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
	//~ Point pos  =      (position_ + shape.position_)   / 2;
	Point s1p1 =       position_ -       defaultSize_ / 2;
	Point s1p2 =       position_ +       defaultSize_ / 2;
	Point s2p1 = shape.position_ - shape.defaultSize_ / 2;
	Point s2p2 = shape.position_ + shape.defaultSize_ / 2;
	bool b1 = s2p1.get_x() <= s1p1.get_x() && s1p1.get_x() <= s2p2.get_x() &&
						s2p1.get_y() <= s1p1.get_y() && s1p1.get_y() <= s2p2.get_y();
	bool b2 = s2p1.get_x() <= s1p1.get_x() && s1p1.get_x() <= s2p2.get_x() &&
						s2p1.get_y() <= s1p2.get_y() && s1p2.get_y() <= s2p2.get_y();
	bool b3 = s2p1.get_x() <= s1p2.get_x() && s1p2.get_x() <= s2p2.get_x() &&
						s2p1.get_y() <= s1p2.get_y() && s1p2.get_y() <= s2p2.get_y();
	bool b4 = s2p1.get_x() <= s1p2.get_x() && s1p2.get_x() <= s2p2.get_x() &&
						s2p1.get_y() <= s1p1.get_y() && s1p1.get_y() <= s2p2.get_y();
	if (b1 || b2 || b3 || b4) {
		size_t index;
		if (!intersected_.find(&shape, index)) {
			color_ = randomColor();
			shape.color_ = randomColor();
			intersected_.add(&shape);
			if (!shape.intersected_.find(this, index))
				shape.intersected_.add(this);
		}
		// TODO: Deformation
		/*if (b1) {
			point1.x = pos.x;       point1.y = pos.y;
			shape.point2.x = pos.x; shape.point2.y = pos.y;
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

const Point& Shape::getPoint1() const {
	return point1;
}

const Point& Shape::getPoint2() const {
	return point2;
}

Shapes::Shapes() :
array_(), activeId_(0), activated_(0), activationPoint_(0, 0) {}

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

void Shapes::erase(const size_t index) {
	activated_ = 0;
	activeId_ = 0;
	array_.erase(index);
}

void Shapes::activate(const Point& p) {
	//if (array_.size() < 1) return;
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

void Shapes::moveActive(const Point& p) {
	if (activated_ && activeId_ < array_.size()) {
		array_[activeId_]->moveTo(p - activationPoint_);
	}
}

void Shapes::release() {
	activated_ = 0;
}

void Shapes::draw(
	const Cairo::RefPtr<Cairo::Context>& context,
	const Gtk::Allocation& allocation
) {
	for (size_t i = 0; i < array_.size(); i++) {
		array_[i]->render(allocation);
	}
	for (size_t i = 0; i < array_.size(); i++) {
		for (size_t j = 0; j < array_.size(); j++) {
			array_[i]->areIntersected(*array_[j]);
		}
	}
	for (size_t i = 0; i < array_.size(); i++) {
		array_[i].draw(context);
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


void Shapes::Element::draw(const Cairo::RefPtr<Cairo::Context>& context) {
	shapeTrace_.draw(context);
	pointer_->draw(context);
}
