#!/usr/bin/env python3

from sympy import *
import numpy as np
from scipy.optimize import fsolve
from scipy import linalg
init_printing()

def calc(X, t: int = 0) -> np.ndarray:
    x1, x2, x3 = X
    return np.array([
        x1**2+4*x1**2*x2+x3**2-x2**2,
        x1**2-x2-2*x3,
        x1+4*x2-3*x3**2
    ])

## @brief Аналіз системи.
def analyze() -> None:
    t = symbols("t")
    x = symbols("x1 x2 x3")
    k = symbols("k1 k2 k3")
    #x = [i(t) for i in x]
    f = calc(x, t)
    B = np.array([[2,1],[1,2],[0,-1]])
    K = np.array([k,[0,0,0]])
    Bu = B.dot(K).dot(x)
    fBu = f+Bu
    pprint(Matrix(fBu))
    I = Matrix(fBu).jacobian(x)
    pprint("Якобіан:")
    pprint(I)
    #solutions = [list(i.values()) for i in solve(fBu, x)]
    solutions = [[0,0,0]]
    pprint("Особливі точки:")
    pprint(solutions)

    for X, Y, Z in solutions:
        print()
        print("Точка:", X, Y, Z)
        syst = lambdify(x, I, modules="sympy")(X, Y, Z)
        #eigenvals = syst.eigenvals()
        #eigenvals = list(eigenvals.keys())
        systl = lambdify(k, syst, modules="numpy")
        def eigenvals(k):
            ev = linalg.eigvals(systl(*k))
            return [i.real+1 for i in ev]
        pprint(eigenvals((-1,-1,-1)))
        sol = fsolve(eigenvals, (-1, -1, -1))
        print(sol, eigenvals(sol))
        #eigenvals = [complex(i) for i in eigenvals]
        #mat = Matrix(eigenvals)
        #pprint(mat)
        #ll = lambdify((k,), mat, modules="numpy")
        #rint(ll((1,1,1)), ll((2,2,2)))
        #sol = fsolve(ll, (1, 1, 1))
        #print(sol)

analyze()
