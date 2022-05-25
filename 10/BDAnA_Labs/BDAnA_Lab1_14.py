#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
from math import sin

x = np.linspace(-1.0, 1.0, 5)
y = np.linspace(-1.0, 1.0, 5)
z = np.linspace(-1.0, 1.0, 5)

starts = [
    ([0.8, 0.8, 0.8], 10),
    # ~ ([0.8, 0.8, -0.8], 10),
    # ~ ([0.8, -0.8, 0.8], 10),
    # ~ ([0.8, -0.8, -0.8], 10),
    ([-0.8, 0.8, 0.8], 10),
    ([-0.8, 0.8, -0.8], 10),
    ([-0.8, -0.8, 0.8], 10),
    # ~ ([-0.8/4, -0.8/4, -0.8/4], 10),
]


def calc(X, t: int = 0) -> np.ndarray:
    x1, x2, x3 = X[0], X[1], X[2]
    k=(0.26081334,  0.47837333, -1.12976005)
    k=(-0.98538269, -0.02923461, -1.91960482)
    # ~ k=(-1,-1,-1)
    return np.array([
        x1**2+4*x1**2*x2+x3**2-x2**2+2*k[0]*x1 + 2*k[1]*x2 + 2*k[2]*x3,
        x1**2-x2-2*x3+k[0]*x1 + k[1]*x2 + k[2]*x3,
        x1+4*x2-3*x3**2
    ])


def print_vector_field(ax, x, y, z) -> None:
    X, Y, Z = np.meshgrid(x, y, z)
    u, v, w = np.zeros(X.shape), np.zeros(Y.shape), np.zeros(Z.shape)
    u, v, w = calc([X, Y, Z])
    # Друкує на графік векторне поле.
    ax.quiver(X, Y, Z, u, v, w, color='r', length=0.1)


## @brief Будуємо траекторії.
#  @param starts змінна `starts`, яка задає усі траекторії
def print_trajectories(ax, starts: list) -> None:
    # Обчислюємо траєкторії задані у змінній `starts`
    for s, t in starts:
        # Задаємо діапазон часу [0, t] та ділить відрізок на 2000 частин.
        tspan = np.linspace(0, t, 2000)
        # Інтегрує систему до заданого часу `t` та початкових значень `s`
        tr = odeint(calc, s, tspan)
        # Друкуєму саму траєкторію
        ax.plot3D(tr[:, 0], tr[:, 1], tr[:, 2])
        # Друкуємо початок траєкторії у вигляді кружечка
        ax.plot3D([tr[0, 0]], [tr[0, 1]], [tr[0, 2]], 'o')
        # Друкуємо кінець траєкторії у вигляді квадратика
        ax.plot3D([tr[-1, 0]], [tr[-1, 1]], [tr[-1, 2]], 's')


fig = plt.figure()
ax = fig.gca(projection='3d')
print_vector_field(ax, x, y, z)
print_trajectories(ax, starts)
plt.show()
