#!/usr/bin/env python3

from sympy import *
import numpy as np
init_printing(use_latex='mathjax')
from IPython.display import display
pprint = display

def calc(X, t, k, K) -> np.ndarray:
    x, y, z = X
    k1,k2,k3,k4,k5,k6,k7,k8 = k
    K1, K2 = K
    return np.array([
        # ~ k1+k2*x-(k3*y+k4*z)*x/(x+K1),
        k1+k2*x-(k3*y)*x/(x+K1),
        k5*x-k6*y,
        k7*x-k8*z/(z+K2)
    ])

## @brief Аналіз системи.
def analyze() -> None:
    t = symbols("t")
    x = symbols("x y z", cls=Function)
    x = [i(t) for i in x]
    k = symbols("k1:9")
    K = symbols("K1:3")
    f = calc(x, t, k, K)
    
    # ~ I = Matrix([[i.diff(j) for j in x] for i in f])
    pprint(Matrix(f))
    I = Matrix(f).jacobian(x)
    print("Якобіан:")
    pprint(Matrix(I))
    
    solutions = [list(i.values()) for i in solve(f)]
    print("Особливі точки:")
    pprint(Matrix(solutions))

analyze()
