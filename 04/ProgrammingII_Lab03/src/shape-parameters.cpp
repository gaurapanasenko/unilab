/***************************************************************
 * Name:      aggregator.cpp
 * Purpose:   Definition of ShapeParameters
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "shape-parameters.h"

/******************
* ShapeParameters *
******************/

ShapeParameters::ShapeParameters()
  : position_(Point(0, 0)), traceSize_(0), traceTime_(0.1f) {}

const Point& ShapeParameters::getPosition() const {
  return position_;
}

void ShapeParameters::setPosition(const Point& position) {
  position_ = position;
}

float ShapeParameters::getX() {
  return getPosition().getX();
}

void ShapeParameters::setX(float x) {
  position_.setX(x);
}

float ShapeParameters::getY() {
  return getPosition().getY();
}

void ShapeParameters::setY(float y) {
  position_.setY(y);
}


const Sizes& ShapeParameters::getSizes() const {
  return sizes_;
}

const Size& ShapeParameters::getDefaultSize() const {
  return getSizes().getDefaultSize();
}

float ShapeParameters::getDefaultWidth() {
  return sizes_.getDefaultSize().getX();
}

void ShapeParameters::setDefaultWidth(float width) {
  sizes_.setDefaultSizeX(width);
}

float ShapeParameters::getDefaultHeight() {
  return sizes_.getDefaultSize().getY();
}

void ShapeParameters::setDefaultHeight(float height) {
  sizes_.setDefaultSizeY(height);
}

float ShapeParameters::getMaximumWidth() {
  return getSizes().getMaximumSize().getX();
}

void ShapeParameters::setMaximumWidth(float width) {
  sizes_.setMaximumSizeX(width);
}

float ShapeParameters::getMaximumHeight() {
  return getSizes().getMaximumSize().getY();
}

void ShapeParameters::setMaximumHeight(float height) {
  sizes_.setMaximumSizeY(height);
}

void ShapeParameters::setMinimumZoom(float minimumZoom) {
  sizes_.setMinimumZoom(minimumZoom);
}

unsigned char ShapeParameters::getTraceSize() {
  return traceSize_;
}

void ShapeParameters::setTraceSize(const unsigned char traceSize) {
  traceSize_ = traceSize;
}

float ShapeParameters::getTraceTime() {
  return traceTime_;
}

void ShapeParameters::setTraceTime(float traceTime) {
  traceTime_ = traceTime;
}

const Cairo::Matrix& ShapeParameters::getDefaultMatrix() {
  return defaultMatrix_;
}

void ShapeParameters::setDefaultMatrix(
    const Cairo::Matrix& defaultMatrix) {
  defaultMatrix_ = defaultMatrix;
}

ShapeParameters SHAPE;
