#include "model.h"

int gauss(Matrix::Minor a, Matrix::Vector y, Matrix::Vector x) {
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
