#!/usr/bin/env python3
## @package lab4
#  @brief Лабораторна робота 4
#  \image html Lab4.png
#  \image latex Lab4.png

import matplotlib.pyplot as plt
import numpy as np

## @brief Скільки перших ітерацій пропустити
firstIters = 1024
## @brief Скільки останніх ітерацій відобразити на діаграмі
lastIters = 256
## @brief Скільки значень \f$\alpha\f$ побудувати для діаграми.
aIters = 2000
## @brief Діапазон значень \f$\alpha\f$ [2.8, 3.8]
alphaRange = [2.8, 3.8]
## @brief Стартове значення \f$x_0\f$.
#  Яке обчислено за допомогою рівняння:
#  \f[f'(x_0,\alpha)=0\f]
xStart = 0.5


## @brief Відображення з постановки задачі.
#  \f[x_{n+1}=\alpha x_n^2(1-x^2_n),\ n=0,1,2,\dots\f]
#  @param x значення \f$x_n\f$
#  @param a значення \f$\alpha\f$ 
def f(x: float, a: float) -> None:
    return a*x**2*(1-x**2)

## @brief Друкує діаграму.
def draw_diagram() -> None:
    # Знаходимо усі можливі \f$\alpha\f$
    As = np.linspace(alphaRange[0], alphaRange[1], aIters)
    # Для кожної \f$\alpha\f$ зберегти останні ітерації
    grid = np.zeros((lastIters, aIters))

    # Робимо ітерації для кожної \f$\alpha\f$
    for i in range(aIters):
        x = xStart
        a = As[i]
        # Пропускаємо перші `firstIters` ітерації.
        for _ in range(firstIters):
            x = f(x, a)
        
        # Зберінаємо останні `lastIters` ітерацій.
        for j in range(lastIters):
            x = f(x, a)
            grid[j, i] = x

    # Друкуємо графік.
    for i in grid:
        plt.scatter(As, i, 1, alpha=0.25, color="black")
    plt.show()


draw_diagram()
