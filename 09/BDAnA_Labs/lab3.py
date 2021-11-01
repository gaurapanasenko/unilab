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
dSize = 1000
## @brief Початкова точка
startPoint = [50, 2, 2]
epsillon = 0.001


## @brief Обчислює похідну для системи.
# @param X точка системи
# @param t час
# @return вектор похідної
def sys(X, t=0) -> np.ndarray:
    x, y, z = X
    return np.array([
        1000-3*x-1000*y**2+10*z**2,
        y+2*z+x*(y+z*4/3),
        -2*y+z+x*(-y*4/3+z)
    ])


## @brief Побудувати аттрактор
def draw_attractor() -> None:
    tspan = np.linspace(0, tEnd, dSize)
    ys = odeint(sys, startPoint, tspan)
    xx, yy = np.meshgrid(ys, ys)
    I = (abs(xx - yy) - epsillon) > 0
    
    tspan = np.linspace(0, tEnd, tSize)
    ys = odeint(sys, startPoint, tspan)

    fig = plt.figure()
    ax = fig.add_subplot(1,2,1,projection='3d')
    ax.plot3D(ys[:,0], ys[:,1], ys[:,2], alpha=0.75)
    ax = fig.add_subplot(1,2,2)
    ax.imshow(I, cmap=plt.cm.gray, origin='lower')
    plt.show()


draw_attractor()
