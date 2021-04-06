#!/usr/bin/env python3

from sympy import *
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint

def calc(X, t=0):
    x1, x2 = X[0], X[1]
    return np.array([
        x1**2-2*x1*x2+x2**2-9,
        4*x1**2+x1*x2+4*x2**2-18,
    ])

t = symbols("t")
x1, x2 = symbols("x1, x2", cls=Function)

f1, f2 = calc([x1(t), x2(t)])

I = Matrix([[diff(f1,x1(t)), diff(f1,x2(t))],
[diff(f2,x1(t)), diff(f2,x2(t))]])

for x, y in [[-2,1], [2,-1], [-1,2], [1,-2]]:
    print(x, y, I.subs(x1(t), x).subs(x2(t), y).eigenvals())

