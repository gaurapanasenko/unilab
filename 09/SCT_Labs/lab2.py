#!/usr/bin/env python3

from sympy import *
from sympy.solvers.ode.ode import odesimp
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
from IPython.display import display, Latex
init_printing(use_latex='mathjax')
pprint = display

t, a, T = symbols("t, a, T")
x, u, psi = symbols("x, u, psi", cls=Function)
x = x(t)
psi = psi(x)

def sys(x, t, u, a, T):
    return u(x, a, T) - a * x
    
def find_u_sys(system, psi_fun):
    print("Розглянемо диференціальне рівняння:")
    pprint(Eq(diff(x,t), sys(x, t, u, a, T)))
    print("Родина стійких екстремалей визначається рівнянням:")
    eq = Eq(T*diff(psi, x)*sys(x, t, u, a, T) + psi, 0)
    pprint(eq)
    print("Візмемо:")
    pprint(Eq(psi, psi_fun))
    eq = eq.subs(psi, psi_fun)
    print("Підставимо у рівняння:")
    pprint(eq)
    eq = simplify(eq)
    pprint(eq)
    sol = solve(eq, u(x, a, T))[0]
    print("Знайдемо управління:")
    pprint(sol)
    return lambdify([x, a, T], sol, modules='sympy')

def find_x_sys(system, u):
    sol = sys(x, 0, u, a, T)
    print("Підставимо у рівняння отримане управління:")
    pprint(Eq(diff(x,t), sol))
    return lambdify((x, t, a, T), sol, modules='sympy')

u_sol = find_u_sys(sys, x)
x_sol = find_x_sys(sys, u_sol)

plt.figure(1)
tspan = np.linspace(0, 10, 1000)
plt.plot(tspan, odeint(x_sol, 50, tspan, args=(10, 1)))
plt.plot(tspan, odeint(x_sol, 10, tspan, args=(10, 1)))
plt.plot(tspan, odeint(x_sol, -10, tspan, args=(10, 1)))
plt.plot(tspan, odeint(x_sol, 10, tspan, args=(1, 0.1)))
plt.show(block=False)

print()
u_sol = find_u_sys(sys, tanh(x))
x_sol = find_x_sys(sys, u_sol)
x_sol = lambda x, t, a, T: -0.5*np.sinh(2*x)/T

tspan = np.linspace(0, 10, 1000)

plt.figure(2)
plt.plot(tspan, odeint(x_sol, 11, tspan, args=(10, 1)))
plt.plot(tspan, odeint(x_sol, 5, tspan, args=(10, 1)))
plt.plot(tspan, odeint(x_sol, 1, tspan, args=(10, 1)))
plt.plot(tspan, odeint(x_sol, -1, tspan, args=(10, 1)))
plt.plot(tspan, odeint(x_sol, 1, tspan, args=(1, 0.1)))
plt.show()
