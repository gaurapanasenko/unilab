#ifndef TCLASSES_H
#define TCLASSES_H
#include <vector>
#include <ostream>

template <typename T>
class Matrix {
protected:
	std::vector<std::vector<T>> matrix;
	size_t row, col;
public:
	Matrix();
	Matrix(size_t, size_t);
	void set(size_t, size_t, T);
	void set(size_t, std::vector<T>);
	T get(size_t, size_t);
	T get(size_t, size_t) const;
	size_t get_col() const;
	size_t get_row() const;
	std::vector<std::vector<T>> get_matrix() const;
	std::vector<T> get_row(size_t);
	void resize(size_t, size_t);
	void recreate(size_t, size_t);
	void transpose();
	Matrix& operator= (const Matrix &right);
	template <typename T2>
	friend const Matrix<T2> operator+(const Matrix<T2>&, const Matrix<T2>&);
	template <typename T2>
	friend const Matrix<T2> operator*(const Matrix<T2>&, const Matrix<T2>&);
	template <typename T2>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<T2>& m);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);

template <typename T>
const Matrix<T> operator+(const Matrix<T>&, const Matrix<T>&);

template <typename T>
const Matrix<T> operator*(const Matrix<T>&, const Matrix<T>&);

template<typename T>
class Rect {
private:
	T l, r, b, t;
public:
	Rect();
	Rect(T left, T right, T bottom, T top);
	void set(T left, T right, T bottom, T top);
	T get_left();
	T get_right();
	T get_bottom();
	T get_top();
};

#include "tclasses.tpp"
#endif
