#ifndef GAURA_H
#define GAURA_H

#include <stddef.h>
#include <stdio.h>
#include <limits.h>

namespace gaura {

typedef unsigned long long streamsize;

struct MyException {
  MyException(const char * string) : string_(string) {}
  const char * what () const {
    return string_;
  }
  const char * string_;
};

class ostream {
public:
  ostream(FILE* file) : file_(file) {}

  ostream& operator<<(const char * value) {
    fprintf(file_, "%s", value);
    return *this;
  }

  ostream& operator<<(unsigned int value) {
    fprintf(file_, "%u", value);
    return *this;
  }

  ostream& operator<<(unsigned long value) {
    fprintf(file_, "%lu", value);
    return *this;
  }

private:
  FILE* file_;
};

static ostream cout(stdout);

class istream {
public:
  istream(FILE* file) : file_(file) {}

  istream& operator>>(unsigned int& value) {
    fscanf(file_, "%u", &value);
    return *this;
  }

  istream& operator>>(int& value) {
    fscanf(file_, "%i", &value);
    return *this;
  }

  void ignore(streamsize n = 1, int delim = EOF) {
    int c = 0;
    for (streamsize i = 0; i < n && c != delim; i++) {
      c = getc(file_);
    }
  }

private:
  FILE* file_;
};

static istream cin(stdin);

template<class T>
class vector;

typedef unsigned long size_type;

template<class InputIterator, class OutputIterator>
OutputIterator copy (InputIterator first, InputIterator last,
                     OutputIterator result) {
  while (first != last) {
    *result = *first;
    ++result; ++first;
  }
  return result;
}

template <class T> const T& min (const T& a, const T& b) {
  return !(b < a) ? a : b;
}

template<class T>
class VectorIterator {
public:
  typedef vector<T> vector_type;
  typedef VectorIterator<T> iterator;

  typedef size_type difference_type;
  typedef T value_type;
  typedef value_type& reference;
  typedef value_type* pointer;

  explicit VectorIterator(vector_type* vector = nullptr,
                          size_type index = 0) :
        vector_(vector), index_(index) {}

  iterator& operator++() {
    return *this += 1;
  }

  iterator operator++(int) {
    iterator out = *this;
    *this += 1;
    return out;
  }

  iterator& operator--() {
    return *this -= 1;
  }

  iterator operator--(int) {
    iterator out = *this;
    *this -= 1;
    return out;
  }

  iterator& operator+=(size_type index) {
    return *this = *this + index;
  }

  friend iterator operator+(const iterator& lhs, size_type rhs) {
    return iterator(lhs.vector_, lhs.index_ + rhs);
  }

  friend iterator operator+(size_type lhs, const iterator& rhs) {
    return rhs + lhs;
  }

  iterator& operator-=(size_type value) {
    return *this = *this - value;
  }

  friend iterator operator-(const iterator& lhs, size_type rhs) {
    return iterator(lhs.vector_, lhs.index_ - rhs);
  }

  friend difference_type operator-(const iterator& lhs,
                                   const iterator& rhs) {
    if (lhs.vector_ != rhs.vector_)
      throw MyException("Iterators from different vectors");
    return lhs.index_ - rhs.index_;
  }

  reference operator*(){
    return (*vector_)[index_];
  }

  pointer operator->() const {
    return &(*vector_)[index_];
  }

  friend bool operator==(const iterator& lhs, const iterator& rhs) {
    bool s = lhs.vector_ == rhs.vector_;
    bool i = lhs.index_ == rhs.index_;
    return s && i;
  }

  friend bool operator!=(const iterator& lhs, const iterator& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const iterator& lhs, const iterator& rhs) {
    bool s = lhs.vector_ == rhs.vector_;
    bool i = lhs.index_ < rhs.index_;
    return s && i;
  }

  friend bool operator>(const iterator& lhs, const iterator& rhs) {
    bool s = lhs.vector_ == rhs.vector_;
    bool i = lhs.index_ > rhs.index_;
    return s && i;
  }

  friend bool operator<=(const iterator& lhs, const iterator& rhs) {
    return lhs < rhs || lhs == rhs;
  }

  friend bool operator>=(const iterator& lhs, const iterator& rhs) {
    return lhs > rhs || lhs == rhs;
  }

private:
  vector_type* vector_;
  size_type index_;
};

template<class T>
class ConstVectorIterator {
public:
  typedef vector<T> vector_type;
  typedef ConstVectorIterator<T> iterator;

  typedef size_type difference_type;
  typedef T value_type;
  typedef value_type& reference;
  typedef value_type* pointer;

  explicit ConstVectorIterator(const vector_type* vector = nullptr,
                          size_type index = 0) :
    vector_(vector), index_(index) {}

  iterator& operator++() {
    return *this += 1;
  }

  iterator operator++(int) {
    iterator out = *this;
    *this += 1;
    return out;
  }

  iterator& operator--() {
    return *this -= 1;
  }

  iterator operator--(int) {
    iterator out = *this;
    *this -= 1;
    return out;
  }

  iterator& operator+=(size_type index) {
    return *this = *this + index;
  }

  friend iterator operator+(const iterator& lhs, size_type rhs) {
    return iterator(lhs.vector_, lhs.index_ + rhs);
  }

  friend iterator operator+(size_type lhs, const iterator& rhs) {
    return rhs + lhs;
  }

  iterator& operator-=(size_type value) {
    return *this = *this - value;
  }

  friend iterator operator-(const iterator& lhs, size_type rhs) {
    return iterator(lhs.vector_, lhs.index_ - rhs);
  }

  friend difference_type operator-(const iterator& lhs,
                                   const iterator& rhs) {
    if (lhs.vector_ != rhs.vector_)
      throw MyException("Iterators from different vectors");
    return lhs.index_ - rhs.index_;
  }

  value_type operator*(){
    return (*vector_)[index_];
  }

  pointer operator->() const {
    return &(*vector_)[index_];
  }

  friend bool operator==(const iterator& lhs, const iterator& rhs) {
    bool s = lhs.vector_ == rhs.vector_;
    bool i = lhs.index_ == rhs.index_;
    return s && i;
  }

  friend bool operator!=(const iterator& lhs, const iterator& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const iterator& lhs, const iterator& rhs) {
    bool s = lhs.vector_ == rhs.vector_;
    bool i = lhs.index_ < rhs.index_;
    return s && i;
  }

  friend bool operator>(const iterator& lhs, const iterator& rhs) {
    bool s = lhs.vector_ == rhs.vector_;
    bool i = lhs.index_ > rhs.index_;
    return s && i;
  }

  friend bool operator<=(const iterator& lhs, const iterator& rhs) {
    return lhs < rhs || lhs == rhs;
  }

  friend bool operator>=(const iterator& lhs, const iterator& rhs) {
    return lhs > rhs || lhs == rhs;
  }

private:
  const vector_type* vector_;
  size_type index_;
};

template<class T>
class vector {
public:
  typedef T value_type;
  typedef VectorIterator<T> iterator;
  typedef ConstVectorIterator<T> const_iterator;

  explicit vector(size_type size = 0, value_type value = value_type()) :
    size_(size), array_(nullptr) {
    array_ = new value_type[size];
    for (iterator i = begin(); i != end(); i++) {
      *i = value;
    }
  }

  vector(const vector& vector_) : size_(vector_.size_), array_(nullptr) {
    array_ = new value_type[size_];
    copy(vector_.begin(), vector_.end(), begin());
  }

  vector& operator=(const vector& vector_) {
    delete [] array_;
    size_ = vector_.size_;
    array_ = new value_type[size_];
    copy(vector_.begin(), vector_.end(), begin());
    return *this;
  }

  ~vector() {
    delete [] array_;
    array_ = nullptr;
  }

  void resize(size_type size, value_type value = value_type()) {
    vector vector_(size, value);
    copy(begin(), begin() + min(size, size_), vector_.begin());
    *this = vector_;
  }

  size_type size() const { return size_; }

  iterator begin() { return iterator(this, 0); }
  const_iterator begin() const { return const_iterator(this, 0); }

  iterator end() { return iterator(this, size_); }
  const_iterator end() const { return const_iterator(this, size_); }

  value_type& operator[](size_type index) {
    return array_[index];
  }

  value_type operator[](size_type index) const {
    return array_[index];
  }

private:
  size_type size_;
  T* array_;
};

template <class Arg, class Result>
struct unary_function {
  typedef Arg argument_type;
  typedef Result result_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function {
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
  typedef Result result_type;
};

template <class T>
struct numeric_limits;


template <>
struct numeric_limits<unsigned long long> {
  static unsigned long long max() { return ULLONG_MAX; }
};

template <class Iterator> struct iterator_traits {
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
};

template <class T> class iterator_traits<T*> {
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
};

template <class T> class iterator_traits<const T*> {
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef const value_type* pointer;
  typedef const value_type& reference;
};

template <class InputIterator, class UnaryPredicate>
typename iterator_traits<InputIterator>::difference_type
count_if (InputIterator first, InputIterator last, UnaryPredicate pred) {
  typename iterator_traits<InputIterator>::difference_type ret = 0;
  while (first!=last) {
    if (pred(*first)) ++ret;
    ++first;
  }
  return ret;
}

template <class RandomAccessIterator, class Compare>
void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
  for (RandomAccessIterator i = first; i != last; i++) {
    for (RandomAccessIterator j = i; j != last; j++) {
      if (comp(*j, *i)) {
        typename iterator_traits<RandomAccessIterator>::value_type tmp = *i;
        *i = *j;
        *j = tmp;
      }
    }
  }
}

template <class InputIterator, class OutputIterator, class UnaryOperator>
OutputIterator transform (InputIterator first1, InputIterator last1,
                          OutputIterator result, UnaryOperator op)
{
  while (first1 != last1) {
    *result = op(*first1);
    ++result; ++first1;
  }
  return result;
}

}

#endif // GAURA_H
