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


Point operator+(const Point& lhs, const double rhs) {
	return Point(lhs.get_x() + rhs, lhs.get_y() + rhs);
}

Point operator-(const Point& lhs, const double rhs) {
	return Point(lhs.get_x() - rhs, lhs.get_y() - rhs);
}

Point operator*(const Point& lhs, const double rhs) {
	return Point(lhs.get_x() * rhs, lhs.get_y() * rhs);
}

Point operator/(const Point& lhs, const double rhs) {
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

/********
* Frame *
********/
Frame::Frame() : position_(0, 0), size_(0, 0) {}

Frame::Frame(const Point& point1, const Point& point2) {
	(*this)(point1, point2);
}

void Frame::operator()(const Point& position, const Point& size) {
	if (size.get_x() < 0 || size.get_y() < 0) return;
	setPosition(position);
	setSize(size);
}

const bool Frame::isInFrame(const Point& point) const {
	Point p1 = getPoint1(), p2 = getPoint2();
	return (
		p1.get_x() <= point.get_x() &&
		point.get_x() <= p2.get_x() &&
		p1.get_y() <= point.get_y() &&
		point.get_y() <= p2.get_y()
	);
}

Point& Frame::getPosition() {
	return position_;
}

const Point& Frame::getPosition() const {
	return position_;
}

void Frame::setPosition(const Point& position) {
	position_ = position;
}

Point& Frame::getSize() {
	return size_;
}

const Point& Frame::getSize() const {
	return size_;
}

void Frame::setSize(const Point& size) {
	if (size.get_x() < 0 || size.get_y() < 0) return;
	size_ = size;
}

const Point Frame::getPoint1() const {
	return position_ - size_ / 2;
}

const Point Frame::getPoint2() const {
	return position_ + size_ / 2;
}

const double calculateDistanceToEllipse(
	const Point& point, const Frame& frame
) {
	Point size(frame.getSize()), p(point - frame.getPosition());
	if (size.get_x() <= 0 || size.get_y() <= 0) return 0;
	return pow(p.get_x(), 2.0) / pow(double(size.get_x()) / 2.0, 2.0) +
	       pow(p.get_y(), 2.0) / pow(double(size.get_y()) / 2.0, 2.0);
}

/******************
* ShapeParameters *
******************/

ShapeParameters::ShapeParameters()
: frame_(Point(0, 0), Point(10, 10)),
minimumZoom_(1), traceSize_(0), traceTime_(0.1) {}

Frame& ShapeParameters::getFrame() {
	return frame_;
}

const Frame& ShapeParameters::getFrame() const {
	return frame_;
}

const int ShapeParameters::getX() {
	return getFrame().getPosition().get_x();
}

void ShapeParameters::setX(const int x) {
	getFrame().getPosition().set_x(x);
}

const int ShapeParameters::getY() {
	return getFrame().getPosition().get_y();
}

void ShapeParameters::setY(const int y) {
	getFrame().getPosition().set_y(y);
}

const int ShapeParameters::getWidth() {
	return getFrame().getSize().get_x();
}

void ShapeParameters::setWidth(const int width) {
	if (width <= 0) return;
	getFrame().getSize().set_x(width);
}

const int ShapeParameters::getHeight() {
	return getFrame().getSize().get_y();
}

void ShapeParameters::setHeight(const int height) {
	if (height <= 0) return;
	getFrame().getSize().set_y(height);
}

const double ShapeParameters::getMinimumZoom() {
	return minimumZoom_;
}

void ShapeParameters::setMinimumZoom(const double minimumZoom) {
	if (minimumZoom <= 0) return;
	minimumZoom_ = minimumZoom;
}

const unsigned char ShapeParameters::getTraceSize() {
	return traceSize_;
}

void ShapeParameters::setTraceSize(const unsigned char traceSize) {
	traceSize_ = traceSize;
}

const double ShapeParameters::getTraceTime() {
	return traceTime_;
}

void ShapeParameters::setTraceTime(const double traceTime) {
	traceTime_ = traceTime;
}

ShapeParameters SHAPE;

/*************
* ShapeTrace *
*************/
ShapeTrace::ShapeTrace() : shape_(0), queue_(), tail_(0), time_(0) {}

ShapeTrace& ShapeTrace::operator=(Shape& pointer) {
	if (shape_ == &pointer) return *this;
	shape_ = &pointer;
	tail_ = 0;
	time_ = 0;
	for (unsigned char i = 0; i < queue_.size(); i++) {
		queue_[i] = pointer.clone();
		queue_[i]->setFrame(Frame(Point(-2, -2), Point(0, 0)));
	}
	return *this;
}

void ShapeTrace::draw(const Cairo::RefPtr<Cairo::Context>& context) {
	if (shape_->hasTrace()) {
		const unsigned char& size = SHAPE.getTraceSize();
		if (size != queue_.size()) {
			queue_.resize(0);
			queue_.resize(size);
			tail_ = 0;
		}
		if (
			(clock() - time_) * 1000.0 / CLOCKS_PER_SEC > SHAPE.getTraceTime() &&
			queue_.size() > 0
		) {
			time_ = clock();
			printf("%i\n",int(size));
			queue_[tail_] = shape_->clone();
			tail_++;
			while (tail_ >= size) tail_ -= size;
		}
		for (unsigned char i = 0; i < size; i++) {
			unsigned char index = tail_ + i;
			while (index >= size) index -= size;
			if (queue_[index])
				queue_[index]->draw(context, 0.75 / (size - i));
		}
	} else if (queue_.size() != 0) {
		queue_.resize(0);
		tail_ = 0;
	}
}

/********
* Shape *
********/
Shape::Shape() : frame_(SHAPE.getFrame()), zoom_(1.0),
defaultColor_(randomColor()), color_(defaultColor_), visible_(1), trace_(0) {}

Shape::~Shape() {}

void Shape::drawShape(const Cairo::RefPtr<Cairo::Context>& context) {}

const Pointer<Shape> Shape::clone() {
	return Pointer<Shape>(new Shape(*this));
}

const bool Shape::isInShapeVirtual(const Point& p) const {
	return true;
}

const double Shape::render(const Gtk::Allocation& allocation) {
	const Point size = getDefaultFrame().getSize() * SHAPE.getMinimumZoom()/2 + 2;
	const Point& ds = getDefaultFrame().getSize();
	const int h = allocation.get_height(), w = allocation.get_width(),
	minX = size.get_x(), minY = size.get_y();
	int x = getPosition().get_x(), y = getPosition().get_y();

	if (x < minX)     x = minX;
	if (y < minY)     y = minY;
	if (x > w - minX) x = w - minX;
	if (y > h - minY) y = h - minY;
	setPosition(Point(x, y));
	return min(min(
		min((x - 2) * 2.0 / ds.get_x(), (w - 2 - x) * 2.0 / ds.get_x()),
		min((y - 2) * 2.0 / ds.get_y(), (h - 2 - y) * 2.0 / ds.get_y())
	), double(getZoom()));
}

void Shape::draw(
  const Cairo::RefPtr<Cairo::Context>& context,
  double alpha
) {
	if (visible_) {
		const Point size = getSize();
		const Point position = getPosition();
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

const bool Shape::isInShape(const Point& p) const {
	return (getFrame().isInFrame(p) && isInShapeVirtual(p));
}

const Frame& Shape::getDefaultFrame() const {
	return SHAPE.getFrame();
}

const Frame& Shape::getFrame() const {
	return frame_;
}

void Shape::setFrame(const Frame& frame) {
	if (setSize(frame.getSize())) {
		setPosition(frame.getPosition());
	}
}

const Point& Shape::getPosition() const {
	return getFrame().getPosition();
}

void Shape::setPosition(const Point& point) {
	frame_.setPosition(point);
}

const Point& Shape::getSize() const {
	return getFrame().getSize();
}

const bool Shape::setSize(const Point& size) {
	frame_.setSize(size);
	return true;
}

const float Shape::getZoom() const {
	return zoom_;
}

void Shape::toggleZoom() {
	zoom_ = (int(zoom_) == 1) ? 2 : 1;
}

void Shape::toggleVisibility() {
	visible_ = (visible_) ? 0 : 1;
}

void Shape::changeColor() {
	color_ = randomColor();
}

void Shape::reset() {
	color_ = defaultColor_;
	frame_ = SHAPE.getFrame();
}

bool Shape::hasTrace() {
	return trace_;
}

void Shape::toggleTrace() {
	trace_ = (trace_) ? 0 : 1;
}

const double Shape::areIntersected(Shape& shape) {
	if (this == &shape) return getZoom();
	const Frame f = Frame(getPosition(), getDefaultFrame().getSize());
	bool b = f.isInFrame(shape.getPosition());
	if (b) {
		double zoom = calculateDistanceToEllipse(
			shape.getPosition(), f
		);
		if (zoom <= 1) {
			size_t index;
			if (!intersected_.find(&shape, index)) {
				color_ = randomColor();
				intersected_.add(&shape);
			}
			if (zoom < SHAPE.getMinimumZoom()) {
				return SHAPE.getMinimumZoom();
			} else {
				return zoom;
			}
			return getZoom();
		}
	}
	size_t index;
	if (intersected_.find(&shape, index))
		intersected_.erase(index);
	return getZoom();
}

/*********
* Shapes *
*********/
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
		array_[activeId_]->setPosition(p - activationPoint_);
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
		double m = array_[i]->render(allocation);
		for (size_t j = 0; j < array_.size(); j++) {
			double x = array_[i]->areIntersected(*array_[j]);
			if (x < m) m = x;
		}
		array_[i]->setSize(SHAPE.getFrame().getSize() * m);
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
