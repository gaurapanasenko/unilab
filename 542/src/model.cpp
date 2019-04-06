#include "model.h"
#include <iostream>
#include <iomanip>

void print(Matrix::Minor a, Matrix::Vector y) {
  sizeType size = a.size();
  for (sizeType i = 0; i < size; i++) {
    for (sizeType j = 0; j < size; j++) {
      std::cout << std::setw(13) << std::setprecision(10)
                << a[i][j] << " * p" << size - j;
      if (j + 1 < size) std::cout << " + ";
    }
    std::cout << " = " << std::setw(13)
              << std::setprecision(10) << y[i] << "\n";
  }
  std::cout << "\n";
}

int gauss(Matrix::Minor a, Matrix::Vector y, Matrix::Vector x) {
  std::cout << "Gauss:\n";
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
    if (index != k) {
      print(a, y);
    }
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
      print(a, y);
			for (j = 0; j < n; j++)
				a[i][j] = a[i][j] - a[k][j];
      y[i] = y[i] - y[k];
      print(a, y);
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
