# Звіт з лабораторної роботи №4<br/>за дисципліною "Програмування II"<br/>студента групи ПА-17-1<br/>Панасенка Єгора Сергійовича<br/>Кафедра комп'ютерних технологій<br/>ФПМ, ДНУ, 2017-2018 навч.р.

## Постановка задачі

Розробити програму – консольний застосунок для ОС Windows, що демонструє можливості мови C++, а також його стандартної бібліотеки по роботі з потоками, контейнерами та ітераторами.

Використання всіх вищезгаданих можливостей є обов’язковим в кожному з варіантів індивідуальних завдань.

В кожному з нижченаведених варіантів завдань Вами повинні бути реалізовані основні можливості бібліотеки стандартних шаблонів: ітератор(и), пошук, упорядкування, сума або щось аналогічне зі згоди викладача.

При написанні програми слід враховувати необхідність забезпечення її зборки у двох режимах:

1. Створення виконуваного файлу із підключенням/використанням типів і алгоритмів бібліотеки стандартних шаблонів (STL).
2. Створення виконуваного файлу без застосування STL.

Наприклад: Ваше завдання – втілити алгоритм швидкого сортування для контейнеру «вектор»; в такому разі Ви мусите продемонструвати, як Ваша програма працює із Вашою реалізацією вектора, а також як вона працює із стандартною реалізацією вектора з бібліотеки STL. Увага: Ви пишете одну програму, а не дві. Необхідні зміни вихідного тексту для переналаштування з/на STL бажано робити за допомогою директив умовної компіляції.

### Варінт 14

(18.10) Напишіть зв’язувач binder3() [1, 18.4.4], який би зв’язував другий та третій аргументи трьохаргументної функції для отримання унарного предиката. Наведіть приклад, де функція binder3() є корисною функцією.

## Опис розв’язку

Програма може компілюватись у двох режимах з використанням бібліотеки STL та без використання цієї бібліотеки, але головна програма при цьому не змінюється. Якщо программа компілюється без STL, то вона підключає файл gaura.h, який реалізовує деякі можливості STL, зокрема vector, cin, cout.

### Головна програма ([main.cpp](main.cpp))

Головна програма має такі класи та структури:

1. `struct ternary_function` - задає типи для тернарної функції.
2. `struct in_range` - структура-функтор, яка перевіряє знаходиться число у числовому діапазоні.
3. `struct linear_function` - структура-функтор, яка виводить значення функції a * x + b, x - значення функції, а a та b - константи лінійної функції.
4. `class binder3` - зв'язує другий та третій аргументи для утворення унарної функції.
5. `struct filter_compare` - структура-функтор, яка перевіряє чи знаходяться два числа у заданому діапазоні і перше число менше другого.

Також головна програма має такі функції:

1. `binder3<Operation> bind3` - виводить binder3
2. `int main()` - запускає головне меню.

#### Cтруктура ternary_function

Структура має такі внутрішні типи:
1. `typedef Arg1 first_argument_type;` - тип першого аргументу тернарної функції.
2. `typedef Arg2 second_argument_type;` - тип другого аргументу тернарної функції.
3. `typedef Arg3 third_argument_type;` - тип третього аргументу тернарної функції.
4. `typedef Result result_type;` - тип результату тернарної функції.

#### Cтруктура in_range

Структура має єдину функцію - оператор функтор:

`bool operator() (const T& x, const T& y, const T& z) const` - перевіряє чи знаходиться x у проміжку між y та z;

#### Cтруктура linear_function

Структура має єдину функцію - оператор функтор:

`T operator() (const T& x, const T& a, const T& b) const` - виводить результат функції a * x + b.

#### Клас binder3

Клас binder3 має єдине поле op, яке повинно бути класом з тернарним оператором-функтором і задає два внутрішніх типи:

1. `typename Operation::second_argument_type second;` - тип першого аргументу.
2. `typename Operation::third_argument_type third;` - тип другого аргументу.

Також клас має дві функції:

1. `binder3(const Operation& x, const second& y, const third& z);` - конструктор з параметрами, параметр x - клас з тернарним оператором-функтором і прив'язані другий та третій аргументи y та z.
2. `typename Operation::result_type operator() (const typename Operation::first_argument_type& x)` - оператор-функтор, який перетворює клас у унарну функцію.

#### Cтруктура filter_compare

Структура має поле унарного функтора f, та функції конструктор та оператор-функтор:

`bool operator() (const T& x, const T& y) const` - перевіряє чи функтор f виводить true для x та y і перевіряє чи x менше за y.

## Опис іинтерфейсу

Проект має єдину програму з таким меню:

```
Choose option:
    0. Exit.
    1. Input array.
    2. Output array.
    3. Count elements that in value range.
    4. Smart sort vector.
    5. Transform by linear function.
```

Після того як програма вивела меню, вона циклічно запитує команди з цього меню.

Опис пунктів меню:

1. `0. Exit` -  Вихід із програми
2. `1. Input array.` - ввести масив з клавіатури.
3. `2. Output array.` - вивести масив на екран.
4. `3. Count elements that in value range.` - запитує деякий діапазон чисел та рахує кількість елементів які задовольняють цьому діапазону.
5. `4. Smart sort vector.` - запитує деякий діапазон чисел та сортує вибірково за заданим діапазоном.
6. `5. Transform by linear function.` - запитує коефіцієнти a та b і перетворює кожний елемент за формулою a * x + b, де x - значення елемента.

## Опис тестових прикладів

```
Choose option:
    0. Exit.
    1. Input array.
    2. Output array.
    3. Count elements that in value range.
    4. Smart sort vector.
    5. Transform by linear function.
>>> 1
Type new size of array:
>>> 9
Type elements:
9 8 7 6 5 4 3 2 1
>>> 2
Array has 9 elements:
9 8 7 6 5 4 3 2 1
>>> 3
Type value interval:
>>> 4 7
Array has 4 elements between this interval
>>> 4
Type value interval:
>>> 3 6
>>> 2
Array has 9 elements:
9 8 7 3 4 5 6 2 1
>>> 5
Type cofficients a and b for linear function a * x + b:
>>> 2 3
>>> 2
Array has 9 elements:
21 19 17 9 11 13 15 7 5
>>> 0
```

## Вихідний текст програми розв’язку задачі

### Файл [main.cpp](main.cpp)

```cpp
#ifndef NOSTL
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <functional>

using namespace std;
#else
#include "gaura.h"

using namespace gaura;
#endif

typedef unsigned int ui;
template <class Arg1, class Arg2, class Arg3, class Result>
struct ternary_function {
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Arg3 third_argument_type;
	typedef Result result_type;
};

template <class T> struct in_range : ternary_function <T,T,T,bool> {
	bool operator() (const T& x, const T& y , const T& z) const {
    return (y <= z) ? y <= x && x <= z : z <= x && x <= y;
	}
};

template <class T> struct linear_function : ternary_function <T,T,T,T> {
  T operator() (const T& x, const T& a , const T& b) const {
    return a * x + b;
  }
};

template <class Operation>
class binder3
	: public unary_function <typename Operation::first_argument_type,
													 typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::second_argument_type second;
	typename Operation::third_argument_type third;
public:
	binder3 (const Operation& x,
					 const typename Operation::second_argument_type& y,
					 const typename Operation::third_argument_type& z) :
		op (x), second(y), third(z) {}
	typename Operation::result_type
	operator() (const typename Operation::first_argument_type& x) const {
		return op(x, second, third);
	}
};

template <class Operation, class T>
binder3<Operation> bind3 (const Operation& op, const T& x, const T& y) {
	typedef typename Operation::second_argument_type s;
	typedef typename Operation::third_argument_type t;
	return binder3<Operation>(op, s(x), t(y));
}

template <class F, class T>
struct filter_compare : binary_function <T,T,bool> {
  filter_compare(const F& filter) : f(filter) {}
  bool operator() (const T& x, const T& y) const {
    return f(x) and f(y) and x < y;
  }
  F f;
};

int main() {
  vector<ui> a(0, 0);
  int answer = -1;
  streamsize mstrm = numeric_limits<streamsize>::max();
  while (answer != 0) {
    cout << "Choose option:\n"
         << "    0. Exit.\n"
         << "    1. Input array.\n"
         << "    2. Output array.\n"
         << "    3. Count elements that in value range.\n"
         << "    4. Smart sort vector.\n"
         << "    5. Transform by linear function.\n"
         << ">>> ";
    cin >> answer;
    cin.ignore(mstrm,'\n');
    switch (answer) {
    case 0:
      return 0;
    case 1: {
        cout << "Type new size of array:\n>>> ";
        ui size;
        cin >> size;
        a.resize(size, 0);
        cout << "Type elements:\n";
        vector<ui>::iterator i;
        for (i = a.begin(); i != a.end(); i++) {
          cin >> (*i);
        }
        break;
      }
    case 2: {
        cout << "Array has " << a.size() << " elements:\n";
        vector<ui>::iterator i;
        for (i = a.begin(); i != a.end(); i++) {
          cout << *i;
          if (i + 1 != a.end()) {
            cout << " ";
          } else {
            cout << "\n";
          }
        }
        break;
      }
    case 3: {
        cout << "Type value interval:\n>>> ";
        ui b, e;
        cin >> b >> e;
        cout << "Array has "
             << count_if(a.begin(), a.end(), bind3(in_range<ui>(), b, e))
             << " elements between this interval\n";
        break;
      }
    case 4: {
        cout << "Type value interval:\n>>> ";
        ui b, e;
        cin >> b >> e;
        sort(a.begin(), a.end(), filter_compare<binder3<in_range<ui>>, ui>(bind3(in_range<ui>(), b, e)));
        continue;
      }
    case 5: {
        cout << "Type cofficients a and b for linear function a * x + b:\n>>> ";
        ui ca, cb;
        cin >> ca >> cb;
        transform(a.begin(), a.end(), a.begin(), bind3(linear_function<ui>(), ca, cb));
        break;
      }
    default :
        cout << "You typed wrong option, try again\n";
    }
    cout << "Press ENTER to continue\n";
    cin.ignore(mstrm,'\n');
    cout << "\n\n\n\n\n";
  }
  return 0;
}
```

### Файл [gaura.h](gaura.h)

```cpp
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
```
