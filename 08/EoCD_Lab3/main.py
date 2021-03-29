#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint

xx1 = np.linspace(-3.0, 3.0, 20)
xx2 = np.linspace(-3.0, 3.0, 20)
XX1, XX2 = np.meshgrid(xx1, xx2)
u, v = np.zeros(XX1.shape), np.zeros(XX2.shape)
NI, NJ = XX1.shape

def calc(X, t=0):
    x1, x2 = X[0], X[1]
    return np.array([
        x1**2-2*x1*x2+x2**2-9,
        4*x1**2+x1*x2+4*x2**2-18,
    ])

for i in range(NI):
    for j in range(NJ):
        u[i,j], v[i,j] = calc([XX1[i, j], XX2[i, j]])

starts = [
    ([0, 0], 0.6),
    ([2, -1.585], 100),
    ([-2.0, 1.0], 100),
    ([1, -2], 100),
    ([-1, 2], 100),
]

for y, t in starts:
    tspan = np.linspace(0, t, 2000)
    ys = odeint(calc, y, tspan)
    plt.plot(ys[:,0], ys[:,1])
    plt.plot([ys[0,0]], [ys[0,1]], 'o')
    plt.plot([ys[-1,0]], [ys[-1,1]], 's')
        
Q = plt.quiver(XX1, XX2, u, v, color='r')
plt.show()
