/***************************************************************
 * Name:      shape-childs.h
 * Purpose:   Defines shapes childs
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
	Triangle() {}
	static const Pointer<Shape> create();
	const Pointer<Shape> clone();
	void drawShape(const Cairo::RefPtr<Cairo::Context>& context);
	bool isInShapeVirtual(const Point& p);
};


class Rectangle : public Shape {
public:
	Rectangle() {}
	static const Pointer<Shape> create();
	const Pointer<Shape> clone();
	void drawShape(const Cairo::RefPtr<Cairo::Context>& context);
	bool isInShapeVirtual(const Point& p);
};


class Circle : public Shape {
public:
	Circle() {}
	static const Pointer<Shape> create();
	const Pointer<Shape> clone();
	void drawShape(const Cairo::RefPtr<Cairo::Context>& context);
	bool isInShapeVirtual(const Point& p);
};

}

#endif // SHAPECHILDS_H
