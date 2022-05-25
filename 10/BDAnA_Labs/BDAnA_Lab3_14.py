#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
from sympy import *

k_COEFS = np.array([1, 0.3, 1, 0, 0.1, 0.29, 1, 100])
K_COEFS = [0.1, 0.1]

def calc(X, t: int = 0, k=k_COEFS, K=K_COEFS) -> np.ndarray:
    x, y, z = X
    k1,k2,k3,k4,k5,k6,k7,k8 = k
    K1, K2 = K
    return np.array([
        k1+k2*x-(k3*y)*x/(x+K1),
        k5*x-k6*y,
        k7*x-k8*z/(z+K2)
    ])
    
def bifurcation_diagram():
    t = symbols("t")
    x = symbols("x y z", cls=Function)
    x = [i(t) for i in x]
    k = symbols("k1:9")
    K = symbols("K1:3")
    sol = solve(calc(x, 0, k, K), x)
    sol = lambdify((k, K), sol)
    fig = plt.figure()
    for i in np.linspace(0.08, 0.11, 200):
        kk = k_COEFS.copy()
        kk[4] = i
        ans = sol(kk, K_COEFS)
        for j in ans:
            if -100 < j[1] and j[1] < 100:
                plt.plot(i, j[1], marker='o', color="b")


def plot():
    tspan = np.linspace(0, 1000, 20000)
    tr = odeint(calc, [.1, .1, .1], tspan)

    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.plot3D(tr[:, 0], tr[:, 1], tr[:, 2])

    fig = plt.figure()
    plt.plot(tspan, tr[:, 0], label='x')
    plt.plot(tspan, tr[:, 1], label='y')
    plt.plot(tspan, tr[:, 2], label='z')
    plt.legend()

bifurcation_diagram()
plot()
plt.show()