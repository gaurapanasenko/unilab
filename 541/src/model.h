#ifndef MODEL_H
#define MODEL_H

#include "treemodel.h"

int gauss(Matrix a, Vector y, SimpleStoreStuff::Column x);

bool convertToComfortIterable(
  const Matrix& a, const Vector& f,
  Matrix& b, Vector& g
);

bool convertToComfortIterable2(
  const Matrix& a, const Vector& f,
  Matrix& b, Vector& g, const real alpha
);

real calculateMatrixNorm(const Matrix& b);

bool checkIsConverge(const Matrix& b);

Vector makeIteration(
  const Matrix& b, const Vector& g, const Vector& x
);

Vector calculateDelta(
  const Matrix& a, const Vector& f, const Vector& x
);

#endif //MODEL_H
