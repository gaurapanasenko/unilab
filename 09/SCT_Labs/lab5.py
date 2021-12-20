#!/usr/bin/env python3

from sympy import *
from sympy.solvers.ode.ode import odesimp
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
init_printing()

lamb, T, mu = symbols("lamb, T, mu")
x = symbols("x:2:2")
x = [x[:2], x[2:]]
u = symbols("u:2")
alpha = 1+lamb*T**2/2-(lamb*T**2+lamb**2*T**4/4)


def psiv(psi, k):
    p = (psi(k+1)+alpha*psi(k))
    return p.subs(x[0][k+1], x[0][k]+T*x[1][k]).subs(x[1][k+1], x[1][k]+T*u[k])
    
def sol(psi, muv, lambv, Tv, starts):
    xx = np.zeros((3000, 2))
    xx[0] = starts
    sol = lambdify(((x[0][0], x[1][0]), mu, lamb, T), solve(psiv(psi, 0),u[0])[0])

    for i in range(2999):
        Tv = 0.001
        uu = sol(xx[i], 0, 1300000, Tv)
        xx[i+1] = 1*xx[i][0]+Tv*xx[i][1], 1*xx[i][1]+Tv*uu
    
    return xx


Tv = 0.001
lambv = 1300000
muv = 0
print("Альфа", alpha.subs(lamb, 1000000).subs(T, 0.001))

plt.figure(1)
xx = sol(lambda k: x[1][k]-mu+x[0][k]**2, muv, lambv, Tv, [10, 10])
plt.plot(xx[:,0], xx[:,1])
xx = sol(lambda k: x[1][k]-mu+x[0][k]**2, muv, lambv, Tv, [5, 10])
plt.plot(xx[:,0], xx[:,1])
xx = sol(lambda k: x[1][k]-mu+x[0][k]**2, muv, lambv, Tv, [5, -10])
plt.plot(xx[:,0], xx[:,1])
plt.show(block=False)

plt.figure(2)
xx = sol(lambda k: x[1][k]-mu*x[0][k]+x[0][k]**2, muv, lambv, Tv, [10, 10])
plt.plot(xx[:,0], xx[:,1])
xx = sol(lambda k: x[1][k]-mu*x[0][k]+x[0][k]**2, muv, lambv, Tv, [5, 10])
plt.plot(xx[:,0], xx[:,1])
xx = sol(lambda k: x[1][k]-mu*x[0][k]+x[0][k]**2, muv, lambv, Tv, [5, -10])
plt.plot(xx[:,0], xx[:,1])
plt.show(block=False)

plt.figure(3)
xx = sol(lambda k: x[1][k]-mu*x[0][k]+x[0][k]**3, muv, lambv, Tv, [10, 10])
plt.plot(xx[:,0], xx[:,1])
xx = sol(lambda k: x[1][k]-mu*x[0][k]+x[0][k]**3, muv, lambv, Tv, [10, -10])
plt.plot(xx[:,0], xx[:,1])
xx = sol(lambda k: x[1][k]-mu*x[0][k]+x[0][k]**3, muv, lambv, Tv, [-10, -10])
plt.plot(xx[:,0], xx[:,1])
plt.show()
