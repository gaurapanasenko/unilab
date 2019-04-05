#ifndef MODEL_H
#define MODEL_H

#include "matrix.h"
#include <cmath>

const real eps = std::pow(2, -56);

int gauss(Matrix::Minor a, Matrix::Column y, Matrix::Column x);

#endif //MODEL_H
