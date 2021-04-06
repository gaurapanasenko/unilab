#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint

def calc(X, t=0):
    x, y, z = X
    return np.array([
        3*x-y**2+z**2,
        -y-700*z+10*x*y,
        x+700*y-z-15*x*z
    ])

tspan = np.linspace(0, 10, 100000)
ys = odeint(calc, [50, 2, 2], tspan)
fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot3D(ys[:,0], ys[:,1], ys[:,2], alpha=0.75)
plt.show()
