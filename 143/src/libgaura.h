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

#include <cstddef>
#include <exception>
#include <cmath>

const        size_t LIBGAURA_ARRAY_STEP = 16;
const        size_t LIBGAURA_ARRAY_POW  = 1;

struct GauraException : public std::exception {
  const char * what () const noexcept override;
};

struct GauraExceptionMemfail : public GauraException {
  const char * what () const noexcept override;
};


struct GauraExceptionAccess : public GauraException {
  const char * what () const noexcept override;
};

template <typename T> const T& min (const T& a, const T& b) {
  return !(b < a) ? a : b;
}

template <typename T> const T& max (const T& a, const T& b) {
  return (a < b) ? b : a;
}

template <typename T> const T floor (const T& a) {
  return T(std::floor(double(a)));
}

template <typename T> const T pow (const T& a, const T& b) {
  return T(std::pow(double(a), double(b)));
}

template<typename T>
class Pointer {
public:
  explicit Pointer(T* pointer = 0) : pointer_(pointer) {}
  ~Pointer() {delete pointer_;}

  Pointer(const Pointer& rhs) : pointer_(rhs.release()) {}
  Pointer& operator=(const Pointer& rhs) {
    reset(rhs.release());
    return *this;
  }
  operator bool() {
    return pointer_;
  }

  Pointer(Pointer&& rhs) noexcept : pointer_(rhs.release()) {}
  Pointer& operator=(Pointer&& rhs) noexcept {
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
