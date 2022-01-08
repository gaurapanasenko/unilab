#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
from math import sin

x = np.linspace(-5.0, 5.0, 20)
y = np.linspace(-9.0, 9.0, 20)

starts = [
    ([3, 7], 10),
    ([3, 8], 10),
    ([4, 7], 10),
    ([4, 8], 10),
    ([-0.5, -0.5], 3),
    ([-0.5, 0.5], 3),
    ([0.5, -0.5], 3),
    ([0.5, 0.5], 3),
    ([-3, -7], 10),
    ([-3, -8], 10),
    ([-4, -7], 10),
    ([-4, -8], 10),
]


def calc(X, t: int = 0) -> np.ndarray:
    x1, x2 = X[0], X[1]
    return np.array([
        -sin(x1)-1/6*x1**3+x2,
        2*x1-x2,
    ])


print(calc([3.5619, 3.5619*2]))


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
        plt.plot(tr[:, 0], tr[:, 1])
        # Друкуємо початок траєкторії у вигляді кружечка
        plt.plot([tr[0, 0]], [tr[0, 1]], 'o')
        # Друкуємо кінець траєкторії у вигляді квадратика
        plt.plot([tr[-1, 0]], [tr[-1, 1]], 's')
        print([tr[-1, 0]], [tr[-1, 1]])


print_vector_field(x, y)
print_trajectories(starts)
# Друкуємо графік
plt.show()
