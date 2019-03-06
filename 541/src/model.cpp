#include "model.h"

int gauss(Matrix a, Vector y, SimpleStoreStuff::Column x) {
	size_t n = a.size(), k = 0, index, i, j;
	real max, temp;
	while (k < n) {
		// Поиск строки с максимальным a[i][k]
		max = std::abs(a[k][k]);
		index = k;
		for (i = k + 1; i < n; i++) {
			if (std::abs(a[i][k]) > max) {
				max = std::abs(a[i][k]);
				index = i;
			}
		}
		// Перестановка строк
		if (max < eps) {
			// нет ненулевых диагональных элементов
			return 1;
		}
		for (j = 0; j < n; j++) {
			temp = a[k][j];
			a[k][j] = a[index][j];
			a[index][j] = temp;
		}
		temp = y[k];
		y[k] = y[index];
		y[index] = temp;
		// Нормализация уравнений
		for (i = k; i < n; i++) {
			temp = a[i][k];
			if (std::abs(temp) < eps)
				continue;	// для нулевого коэффициента пропустить
			for (j = 0; j < n; j++)
				a[i][j] = a[i][j] / temp;
			y[i] = y[i] / temp;
			if (i == k)
				continue;	// уравнение не вычитать само из себя
			for (j = 0; j < n; j++)
				a[i][j] = a[i][j] - a[k][j];
			y[i] = y[i] - y[k];
		}
		k++;
	}
	fill(x.begin(), x.end(), 0);
	// обратная подстановка
	for (k = n - 1; k >= 0; k--) {
		x[k] = y[k];
		for (i = 0; i < k; i++)
			y[i] = y[i] - a[i][k] * x[k];
		if (k == 0) break;
	}
	return 0;
}

bool convertToComfortIterable(
  const Matrix& a, const Vector& f,
  Matrix& b, Vector& g
) {
  if (a.size() != f.size()) return 0;
  size_t n = a.size();
  b.resize(n);
  g = f;
  for (size_t i = 0; i < n; i++) {
    real x = a[i][i];
    b[i] = -a[i] / x;
    b[i][i] = 0;
    g[i] /= x;
  }
  return 1;
}

bool convertToComfortIterable2(
  const Matrix& a, const Vector& f,
  Matrix& b, Vector& g, const real alpha
) {
  if (a.size() != f.size() || alpha < 0 || alpha > 2) return 0;
  size_t n = a.size();
  Matrix aaa(n, 0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      aaa[i][j] = a[j][i];
    }
  }
  Matrix aa(aaa * a);
  Vector ff(aaa * f);

  Matrix e(n, 0);
  for (int i = 0; i < n; i++) {
    e[i][i] = 1;
  }

  real al = alpha / calculateMatrixNorm(aa);

  b = e - aa * al;
  g = ff * al;
  return 1;
}

real calculateMatrixNorm(const Matrix& a) {
  size_t n = a.size();
  real max = addElementsAbs(a[0]);
  for (size_t i = 1; i < n; i++) {
    real x = addElementsAbs(a[i]);
    if (x > max) max = x;
  }
  return max;
}

bool checkIsConverge(const Matrix& b) {
  if (calculateMatrixNorm(b) < 1) return true;
  else return true;
}

Vector makeIteration(
  const Matrix& b, const Vector& g, const Vector& x
) {
  return b * x + g;
}

Vector calculateDelta(
  const Matrix& a, const Vector& f, const Vector& x
) {
  return a * x - f;
}
