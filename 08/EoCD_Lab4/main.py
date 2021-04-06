#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

def f(x, a):
    return a*x**2*(1-x**2)
    
x = np.linspace(0, 1, 200)
y = np.array([f(i, 4) for i in x])

plt.plot(x, y)
plt.show()

firstIters = 1024
lastIters = 256
aIters = 2000

As = np.linspace(2.8, 3.8, aIters)
grid = np.zeros((lastIters, aIters))

for i in range(aIters):
    x = 0.5
    a = As[i]
    for _ in range(firstIters):
        x = f(x, a)
    for j in range(lastIters):
        x = f(x, a)
        grid[j, i] = x

for i in grid:
    plt.scatter(As, i, 1, alpha=0.25, color="black")
    #plt.plot(As, i, alpha=0.25)
plt.show()
