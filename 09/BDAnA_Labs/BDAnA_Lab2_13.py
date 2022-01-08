#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np

## @brief Скільки перших ітерацій пропустити
firstIters = 1024
## @brief Скільки останніх ітерацій відобразити на діаграмі
lastIters = 50
## @brief Скільки значень \f$\alpha\f$ побудувати для діаграми.
aIters = 2000
## @brief Діапазон значень \f$\alpha\f$ [2.8, 3.8]
alphaRange = [-2.25, 0.25]
## @brief Стартове значення \f$x_0\f$.
#  Яке обчислено за допомогою рівняння:
#  \f[f'(x_0,\alpha)=0\f]
xStart = 0.5


## @brief Відображення з постановки задачі.
#  \f[x_{n+1}=\alpha + x_n^2 + x_n,\ n=0,1,2,\dots\f]
#  @param x значення \f$x_n\f$
#  @param a значення \f$\alpha\f$ 
def f(x: float, a: float) -> None:
    return a+x**2+x

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
        plt.scatter(As, i, 1, alpha=0.25)
    plt.show(block=False)


def draw_diagram2(a, epsillon) -> None:
    x = np.zeros(lastIters)
    x[0] = xStart
    
    for i in range(1, lastIters):
        x[i] = f(x[i-1], a)
        
    xx, yy = np.meshgrid(x, x)
    I = (abs(xx - yy) - epsillon) > 0
    
    fig, axs = plt.subplots(1,2)
    fig.suptitle(f"x*=ax(1-x), a={a}, e={epsillon}")
    axs[0].plot(range(lastIters), x)
    axs[1].imshow(I, cmap=plt.cm.gray, origin='lower')
    plt.show(block=False)


draw_diagram()
draw_diagram2(-1, 0.01)
draw_diagram2(-1.75, 0.01)
draw_diagram2(-2.25, 0.01)
plt.show()
