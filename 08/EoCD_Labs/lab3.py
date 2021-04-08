#!/usr/bin/env python3
## @package lab3
#  @brief Лабораторна робота 3
#  \image html Lab3.png
#  \image latex Lab3.png

import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint

## @brief Задаємо діапазон `x` для побудови векторного поля та кількість векторів.
x = np.linspace(-3.0, 3.0, 20)
## @brief Задаємо діапазон `y` для побудови векторного поля та кількість векторів.
y = np.linspace(-3.0, 3.0, 20)
## @brief Задаємо декілька траєкторій. 
#  Для кожної траекторії задаємо початкові значення та час для траєкторії.
#  
#  Загалом у нас 5 траекторій:
#  * З початком у кординаті (0, 0) та часом 0.6
#  * З початком у кординаті (2, -1.585) та часом 100
#  * З початком у кординаті (-2.0, 1.0) та часом 100
#  * З початком у кординаті (1, -2) та часом 100
#  * З початком у кординаті (-1, 2) та часом 100
starts = [
    ([0, 0], 0.6),
    ([2, -1.585], 100),
    ([-2.0, 1.0], 100),
    ([1, -2], 100),
    ([-1, 2], 100),
]

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

## @brief Обчислює векторне поле у діапазонах [-3, 3] по обом осям.
#  @param x массив усіх можливих значень x
#  @param y массив усіх можливих значень y
def print_vector_field(x: np.ndarray, y: np.ndarray) -> None:
    X, Y = np.meshgrid(x, y)
    u, v = np.zeros(X.shape), np.zeros(Y.shape)
    NI, NJ = X.shape
    # Обчислює векторне поле по заданим значенням.
    for i in range(NI):
        for j in range(NJ):
            u[i, j], v[i, j] = calc([X[i, j], Y[i, j]])
    # Друкує на графік векторне поле.
    plt.quiver(X, Y, u, v, color='r')

## @brief Будуємо траекторії.
#  @param starts змінна `starts`, яка задає усі траекторії
def print_trajectories(starts: list) -> None:
    # Обчислюємо траєкторії задані у змінній `starts`
    for s, t in starts:
        # Задаємо діапазон часу [0, t] та ділить відрізок на 2000 частин.
        tspan = np.linspace(0, t, 2000)
        # Інтегрує систему до заданого часу `t` та початкових значень `s`
        tr = odeint(calc, s, tspan)
        # Друкуєму саму траєкторію
        plt.plot(tr[:,0], tr[:,1])
        # Друкуємо початок траєкторії у вигляді кружечка
        plt.plot([tr[0,0]], [tr[0,1]], 'o')
        # Друкуємо кінець траєкторії у вигляді квадратика
        plt.plot([tr[-1,0]], [tr[-1,1]], 's')


print_vector_field(x, y)
print_trajectories(starts)
# Друкуємо графік
plt.show()
