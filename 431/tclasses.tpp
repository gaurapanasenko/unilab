// matrix.tpp

template <typename T>
Matrix<T>::Matrix() : matrix(0) {
	row = 0;
	col = 0;
}

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) : matrix(rows,
	std::vector<T>(cols, 0)) {
	row = rows;
	col = cols;
}

template <typename T>
void Matrix<T>::set(size_t i, size_t j, T item) {
	if (i >= row || j >= col) return;
	matrix[i][j] = item;
}

template <typename T>
void Matrix<T>::set(size_t i, std::vector<T> rowi) {
	if (i >= row || rowi.size() != col) return;
	matrix[i] = rowi;
}

template <typename T>
T Matrix<T>::get(size_t i, size_t j) {
	return matrix[i][j];
}

template <typename T>
T Matrix<T>::get(size_t i, size_t j) const {
	if (i >= row || j >= col) return 0;
	return matrix[i][j];
}

template <typename T>
std::vector<T> Matrix<T>::get_row(size_t i) {
	if (i >= row) return std::vector<T>(0);
	return matrix[i];
}

template <typename T>
void Matrix<T>::resize(size_t rows, size_t cols) {
	matrix.resize(rows,
	std::vector<T>(cols, 0));
	for (size_t i = 0; i < row; i++)
		matrix[i].resize(cols,0);
	row = rows;
	col = cols;
}

template <typename T>
void Matrix<T>::recreate(size_t rows, size_t cols) {
	matrix.clear();
	matrix.resize(rows,
	std::vector<T>(cols, 0));
	row = rows;
	col = cols;
}

template <typename T>
void Matrix<T>::transpose() {
	std::vector<std::vector<T>> vec(col,std::vector<T>(row,0));
	size_t tmp;
	for (size_t i = 0; i < row; i++)
		for (size_t j = 0; j < col; j++)
			vec[j][i] = get(i,j);
	tmp = row; row = col; col = tmp;
	matrix = vec;
}

template <typename T>
const Matrix<T> operator+(const Matrix<T>& left, const Matrix<T>& right) {
	if (left.col != right.col || left.row != right.row) return Matrix<T>(0,0);
	Matrix<T> new_mat(left.row, left.col);
	for (size_t i = 0; i < left.row; i++)
		for (size_t j = 0; j < left.col; j++)
			new_mat.set(i, j, left.get(i, j) + right.get(i, j));
	return new_mat;
}

template <typename T>
const Matrix<T> operator*(const Matrix<T>& left, const Matrix<T>& right) {
	if (left.col != right.row) return Matrix<T>(0,0);
	Matrix<T> new_mat(left.row, right.col);
	T x = 0;
	for (size_t i = 0; i < left.row; i++)
		for (size_t j = 0; j < right.col; j++) {
			x = 0;
			for (size_t k = 0; k < left.col; k++)
				x += left.get(i, k) * right.get(k, j);
			new_mat.set(i, j, x);
		}
	return new_mat;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
	os << "Matrix:\n";
	for (size_t i = 0; i < m.row; i++) {
		for (size_t j = 0; j < m.col; j++)
			os << m.get(i,j) << " ";
		os << "\n";
	}
	return os;
}

template <typename T>
Rect<T>::Rect() {l = 0; r = 100; b = 0; t = 100;}

template <typename T>
Rect<T>::Rect(T left, T right, T bottom, T top) {
	l = left; r = right; b = bottom; t = top;
}

template <typename T>
void Rect<T>::set(T left, T right, T bottom, T top) {
	l = left; r = right; b = bottom; t = top;
}

template <typename T>
T Rect<T>::get_left() {return l;}

template <typename T>
T Rect<T>::get_right() {return r;}

template <typename T>
T Rect<T>::get_bottom() {return b;}

template <typename T>
T Rect<T>::get_top() {return t;}
