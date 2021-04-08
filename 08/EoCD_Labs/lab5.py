#!/usr/bin/env python3
## @package lab5
#  @brief Лабораторна робота 5
#  \image html Lab5_1.png
#  \image latex Lab5_1.png
#  \image html Lab5_2.png
#  \image latex Lab5_2.png
#  \image html Lab5_3.png
#  \image latex Lab5_3.png
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint

## @brief Час на якому траекторія зупиниться.
tEnd = 10
## @brief Кількість часових точок.
tSize = 100000
## @brief Початкова точка
startPoint = [50, 2, 2]


## @brief Обчислює похідну для системи.
# \f[\begin{cases}
# x'(t)=3x(t)-y(t)^2+z^2(t)\\
# y'(t)=-y(t)-700z(t)+10x(t)y(t)\\
# z'(t)=x(t)+700y(t)-z(t)-15x(t)z(t)
# \end{cases}\f]
# @param X точка системи
# @param t час
# @return вектор похідної
def sys(X, t=0) -> np.ndarray:
    x, y, z = X
    return np.array([
        3*x-y**2+z**2,
        -y-700*z+10*x*y,
        x+700*y-z-15*x*z
    ])


## @brief Побудувати аттрактор
def draw_attractor() -> None:
    tspan = np.linspace(0, tEnd, tSize)
    ys = odeint(sys, startPoint, tspan)
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.plot3D(ys[:,0], ys[:,1], ys[:,2], alpha=0.75)
    plt.show()


draw_attractor()
