/***************************************************************
 * Name:      libgaura.h
 * Purpose:   Many useful things
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-02-01
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#ifndef LIBGAURA_H
#define LIBGAURA_H

#include <stddef.h>

const        size_t LIBGAURA_ARRAY_STEP = 16;
const        size_t LIBGAURA_ARRAY_POW  = 1;

enum gauraException {
  GAURA_EXCEPTION_MEMFAIL,
  GAURA_EXCEPTION_ACCESSFAIL
};

template <typename T> const T& min (const T& a, const T& b) {
  return !(b < a) ? a : b;
}

template <typename T> const T& max (const T& a, const T& b) {
  return (a < b) ? b : a;
}

template<typename T>
class Array {
public:
  Array()  : array_(0), size_(0), realsize_(LIBGAURA_ARRAY_STEP) {
    array_ = new T[realsize_];
    if (array_ == 0) {
      throw GAURA_EXCEPTION_MEMFAIL;
    }
  }
  ~Array() {
    delete[] array_;
  }
  Array(const Array& rhs) : size_(rhs.size_), realsize_(rhs.realsize_) {
    array_ = new T[realsize_];
    if (array_ == 0) {
      throw GAURA_EXCEPTION_MEMFAIL;
    }
    for (size_t i = 0; i < rhs.size_; i++) {
      array_[i] = rhs.array_[i];
    }
  }
  Array& operator=(const Array& rhs) throw() {
    if (this == &rhs) return *this;
    T* tmp = new T[realsize_];
    if (tmp == 0) {
      throw GAURA_EXCEPTION_MEMFAIL;
    }
    for (size_t i = 0; i < rhs.size_; i++) {
      tmp[i] = rhs.array_[i];
    }
    delete[] array_;
    size_ = rhs.size_;
    realsize_ = rhs.realsize_;
    array_ = tmp;
    return *this;
  }
  T& operator[] (const size_t& index) {
    if (index >= size_) {
      throw GAURA_EXCEPTION_ACCESSFAIL;
    }
    return array_[index];
  }
  bool find(const T& element, size_t& index) {
    for (size_t i = 0; i < size_; i++)
      if (array_[i] == element) {
        index = i;
        return true;
      }
    return false;
  }
  void add(const T& element) {
    if (!reallocateBySize(size_ + 1)) return;
    array_[size_ - 1] = element;
  }
  void erase(const size_t& index) {
    if (index >= size_) return;
    for (size_t i = index + 1; i < size_; i++) {
      array_[i - 1] = array_[i];
    }
    reallocateBySize(size_ - 1);
  }
  void erase(const T& element) {
    size_t index;
    if (find(element, index))
      erase(index);
  }
  const size_t& size() {
    return size_;
  }
  const size_t& size() const {
    return size_;
  }

private:
  T* array_;
  size_t size_, realsize_;

  bool reallocateBySize(const size_t& size) {
    size_t ors = realsize_, minSize = min(size, size_);
    size_ = size;

    realsize_ = LIBGAURA_ARRAY_STEP;
    while (size_ > realsize_) realsize_ <<=LIBGAURA_ARRAY_POW;
    if (ors == realsize_) return 1;
    T* tmp = new T[realsize_];
    if (tmp == 0) return 0;
    for (size_t i = 0; i < minSize; i++) {
      tmp[i] = array_[i];
    }
    delete[] array_;
    array_ = tmp;
    return 1;
  }
};

template<typename T>
class Pointer {
public:
  explicit Pointer(T* pointer = 0) : pointer_(pointer) {}
  ~Pointer() {delete pointer_;}

  Pointer(const Pointer& rhs) throw() : pointer_(rhs.release()) {}
  Pointer& operator=(const Pointer& rhs) throw() {
    reset(rhs.release());
    return *this;
  }

  T* operator->() {return pointer_;}
  T* operator&() {return pointer_;}
  T& operator*() {return *pointer_;}
  T* operator->() const {return pointer_;}
  T* operator&() const {return pointer_;}
  T& operator*() const {return *pointer_;}

private:
  T* release() const {
    T* tmp = pointer_;
    pointer_ = 0;
    return tmp;
  }
  void reset(T* pointer = 0) {
    if (pointer != pointer_) {
      delete pointer_;
      pointer_ = pointer;
    }
  }
  mutable T* pointer_;
};

#endif // LIBGAURA_H
