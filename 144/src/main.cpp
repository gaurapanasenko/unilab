#include <cstdlib>
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
  bool operator() (const T& x, const T& y, const T& z) const {
    return (y <= z) ? y <= x && x <= z : z <= x && x <= y;
	}
};

template <class T> struct linear_function : ternary_function <T,T,T,T> {
  T operator() (const T& x, const T& a, const T& b) const {
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
  cout << "Choose option:\n"
       << "    0. Exit.\n"
       << "    1. Input array.\n"
       << "    2. Output array.\n"
       << "    3. Count elements that in value range.\n"
       << "    4. Smart sort vector.\n"
       << "    5. Transform by linear function.\n";
  while (answer != 0) {
    cout << ">>> ";
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
        cin.ignore(mstrm,'\n');
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
  }
  return 0;
}
