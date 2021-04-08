#!/usr/bin/env python3
## @package lab3_analysis
#  @brief Лабораторна робота 3 - Аналіз
#
#  Результат роботи програми:
#  ```
#  Якобіан:
#  Matrix([[2*x1(t) - 2*x2(t), -2*x1(t) + 2*x2(t)], [8*x1(t) + x2(t), x1(t) + 8*x2(t)]])
#  Особливі точки:
#  [(-2, 1), (-1, 2), (1, -2), (2, -1)]
#  
#  Точка: -2 1
#  Власні числа: [-7.3484692283495345j, 7.3484692283495345j]
#  -7.3484692283495345j  - центр
#  7.3484692283495345j  - центр
#  
#  Точка: -1 2
#  Власні числа: [(-4.116843969807043+0j), (13.116843969807043+0j)]
#  (-4.116843969807043+0j)  - сідло
#  (13.116843969807043+0j)  - сідло
#  
#  Точка: 1 -2
#  Власні числа: [(-13.116843969807043+0j), (4.116843969807043+0j)]
#  (-13.116843969807043+0j)  - сідло
#  (4.116843969807043+0j)  - сідло
#  
#  Точка: 2 -1
#  Власні числа: [-7.3484692283495345j, 7.3484692283495345j]
#  -7.3484692283495345j  - центр
#  7.3484692283495345j  - центр
#  ```

from sympy import *
import numpy as np

## @brief Обчислює похідну системи по її значенням.
#  \f[\left\{\begin{array}{l}x_1'(t)=x_1(t)^2-2x_1(t)x_2(t)+x2(t)^2-9\\
#  x_2'(t)=4x_1(t)^2+x_1x_2(t)+4x_2(t)^2-18
#  \end{array}\right.\f]
# @param X точка системи
# @param t час
# @return вектор похідної
def calc(X, t: int = 0) -> np.ndarray:
    x1, x2 = X[0], X[1]
    return np.array([
        x1**2-2*x1*x2+x2**2-9,
        4*x1**2+x1*x2+4*x2**2-18,
    ])

## @brief Аналіз системи.
def analyze() -> None:
    t = symbols("t")
    x = symbols("x1 x2", cls=Function)
    x = [i(t) for i in x]
    f = calc(x, t)
    
    I = Matrix([[i.diff(j) for j in x] for i in f])
    print("Якобіан:")
    print(I)
    
    solutions = solve(f, x)
    print("Особливі точки:")
    print(solutions)

    for X, Y in solutions:
        print()
        print("Точка:", X, Y)
        eigenvals = list(I.subs(x[0], X).subs(x[1], Y).eigenvals().keys())
        eigenvals = [complex(i) for i in eigenvals]
        print("Власні числа:", eigenvals)
        for i in eigenvals:
            if i.imag == 0:
                print(i, " - сідло")
            elif i.imag != 0:
                if i.real == 0:
                    print(i, " - центр")

analyze()
