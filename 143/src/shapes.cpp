/***************************************************************
 * Name:      shapes.cpp
 * Purpose:   Implementation of shapes
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#include "shapes.h"

/********
* Point *
********/
Point::Point() : Gdk::Point(0, 0) {}

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

Point operator*(const Point& lhs, const Point& rhs) {
	return Point(lhs.get_x() * rhs.get_x(), lhs.get_y() * rhs.get_y());
}

Point operator/(const Point& lhs, const Point& rhs) {
	return Point(lhs.get_x() / rhs.get_x(), lhs.get_y() / rhs.get_y());
}


Point operator*(const Point& lhs, const int    rhs) {
	return Point(lhs.get_x() * rhs, lhs.get_y() * rhs);
}

Point operator/(const Point& lhs, const int    rhs) {
	return Point(lhs.get_x() / rhs, lhs.get_y() / rhs);
}

const double calulateVectorLengthSqruare(const Point& point) {
	return pow(point.get_x(), 2.0) + pow(point.get_y(), 2.0);
}

const Point abs(const Point& point) {
	return Point(abs(point.get_x()), abs(point.get_y()));
}

double calculatePseudoscalarProduct(
	const Point& a, const Point& b, const Point& c
) {
	Point ab = b - a, ac = c - a;
	return ab.get_x() * ac.get_y() - ab.get_y() * ac.get_x();
}

bool isOneSizePointsToStraight(
	const Point& a, const Point& b, const Point& c, const Point& d
) {
	return calculatePseudoscalarProduct(c, a, b) *
	       calculatePseudoscalarProduct(d, a, b) > 0;
}

/********
* Color *
********/
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

/*************
* ShapeTrace *
*************/
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

/********
* Frame *
********/
Frame::Frame() : point1_(0, 0), point2_(0, 0) {}

Frame::Frame(const Point& point1, const Point& point2) {
	(*this)(point1, point2);
}

void Frame::operator()(const Point& point1, const Point& point2) {
	point1_ = Point(
		min(point1.get_x(), point2.get_x()), min(point1.get_y(), point2.get_y())
	);
	point2_ = Point(
		max(point1.get_x(), point2.get_x()), max(point1.get_y(), point2.get_y())
	);
}

const bool Frame::isInFrame(const Point& point) const {
	return (
		point1_.get_x() <= point.get_x() && point.get_x() <= point2_.get_x() &&
		point1_.get_y() <= point.get_y() && point.get_y() <= point2_.get_y()
	);
}

const Point Frame::getPosition() const {
	return (point1_ + point2_) / 2;
}

void Frame::setPosition(const Point& point) {
	Point size = getSize() / 2;
	point1_ = point - size;
	point2_ = point + size;
}

const Point Frame::getSize() const {
	return point2_ - point1_;
}

void Frame::setSize(const Point& point) {
	if (point.get_x() <= 0 || point.get_y() <= 0) return;
	Point position = getPosition(), p = point / 2;
	point1_ = position - p;
	point2_ = position + p;
}

const Point& Frame::getPoint1() const {
	return point1_;
}

const Point& Frame::getPoint2() const {
	return point1_;
}

bool isPointInCircle(const Point& point, const Frame& frame) {
	Point size = (frame.getSize()) / 2;
	Point pr = point - frame.getPosition();
	if (calulateVectorLengthSqruare(pr) < size.get_x() * size.get_x())
		return true;
	else return false;
}

/********
* Shape *
********/
Shape::Shape() : defaultFrame_(
	Point(SHAPE_DEFAULT_X, SHAPE_DEFAULT_Y), Point(
		SHAPE_DEFAULT_WIDTH  + SHAPE_DEFAULT_X,
		SHAPE_DEFAULT_HEIGHT + SHAPE_DEFAULT_Y
	)
), frame_(defaultFrame_), position_(frame_.getPosition()),
defaultColor_(randomColor()), color_(defaultColor_), visible_(1), trace_(0) {}

Shape::~Shape() {}

void Shape::drawShape(const Cairo::RefPtr<Cairo::Context>& context) {}

const Pointer<Shape> Shape::clone() {
	return Pointer<Shape>(new Shape(*this));
}

bool Shape::isInShapeVirtual(const Point& p) {
	return true;
}

void Shape::setFrame(const Point& one, const Point& two) {
	frame_(one, two);
	position_ = frame_.getPosition();
}

void Shape::render(const Gtk::Allocation& allocation) {
	const int h = allocation.get_height(), w = allocation.get_width();
	int x = position_.get_x(), y = position_.get_y();

	if (x < 20)     x = 20;
	if (y < 20)     y = 20;
	if (x > w - 20) x = w - 20;
	if (y > h - 20) y = h - 20;
	moveTo(Point(x, y));
	auto sz = defaultFrame_.getSize();
	int arr[] = {
		(x - 2) * 2, (y - 2) * 2,
		(w - 2 - x) * 2, (h - 2 - y) * 2,
		sz.get_x(), sz.get_y()
	}, min = 0;
	auto n = sizeof(arr) / sizeof(int);
	for (unsigned char i = 1; i < n; i++) {
		if (arr[min] > arr[i]) min = i;
	}
	int a = arr[min];
	frame_.setSize(Point(a, a));
}

void Shape::draw(
  const Cairo::RefPtr<Cairo::Context>& context,
  double alpha
) {
	if (visible_) {
		const Point size = frame_.getSize();
		const Point position = frame_.getPosition();
		if (size.get_x() == 0 || size.get_y() == 0) return;
		auto matrix = Cairo::Matrix(
			double(size.get_x()) / 2, 0, 0, double(size.get_y()) / 2,
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
	position_ = p;
	frame_.setPosition(p);
}

const Point& Shape::getPosition() {
	return position_;
}

const Frame& Shape::getFrame() {
	return frame_;
}

bool Shape::isInShape(const Point& p) {
	return (getFrame().isInFrame(p) && isInShapeVirtual(p));
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
	Frame f(defaultFrame_);
	f.setPosition(getPosition());
	bool b = f.isInFrame(shape.getPosition());
	Point pos1 = getPosition(),
	      pos2 = shape.getPosition();
	if (b) {
		size_t index;
		if (!intersected_.find(&shape, index)) {
			color_ = randomColor();
			shape.color_ = randomColor();
			intersected_.add(&shape);
		}
		Point p = abs(pos1 - pos2);
		int len = sqrt(calulateVectorLengthSqruare(p));
		if (len < 20) len = 20;
		Point pl(len, len);
		if (len < getFrame().getSize().get_x() / 2) {
			frame_(pos1 - pl, pos1 + pl);
		}
		if (len < shape.getFrame().getSize().get_x() / 2) {
			shape.frame_(pos2 - pl, pos2 + pl);
		}
	} else {
		size_t index;
		if (intersected_.find(&shape, index))
			intersected_.erase(index);
	}
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
		for (size_t j = i + 1; j < array_.size(); j++) {
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
