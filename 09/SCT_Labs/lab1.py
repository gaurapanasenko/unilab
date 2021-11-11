#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
from sympy import *
init_printing()

a, m, c, t, T = symbols("a, m, c, t, T")
x, lamb, u = symbols("x, lambda, u", cls=Function)
x, lamb, u = x(t), lamb(t), u(t)

def sys1(x, t=0, a=0.1, m=10, c=10) -> np.ndarray:
    return -(m/c-a)*x-a*x

def sys2(x, t=0, a=0.1, m=10, c=10) -> np.ndarray:
    return (3/2*a-np.sqrt(5*a**2*c**+4*m**2)/(2*c))*x-a*x
    

def solve():
    e = Eq(diff(x,t)-a*x, u)
    f0 = (m**2+c**2*a**2) * x**2 - 2*a*u*c**2 + c**2*u**2
    pprint(e)
    pprint(Eq(symbols("I1"),Integral(f0, (t, 0, oo))))
    F = f0 - lamb * (e.lhs - e.rhs)
    pprint(Eq(symbols("J_lambda"),Integral(F, (t, symbols("t0"), T))))
    eq = Eq(diff(F, x) - diff(diff(F, diff(x, t)), t), 0)
    pprint(eq)


def draw() -> None:
    tspan = np.linspace(0, 10)
    
    plt.figure(1)
    plt.plot(tspan, odeint(sys1, 10, tspan))
    plt.plot(tspan, odeint(sys1, 5, tspan))
    plt.plot(tspan, odeint(sys1, -5, tspan))
    plt.plot(tspan, odeint(sys1, 0, tspan))
    
    tspan = np.linspace(0, 2)
    
    plt.figure(2)
    plt.plot(tspan, odeint(sys2, 10, tspan))
    plt.plot(tspan, odeint(sys2, 5, tspan))
    plt.plot(tspan, odeint(sys2, -5, tspan))
    plt.plot(tspan, odeint(sys2, 0, tspan))
    plt.plot(tspan, odeint(sys2, 10, tspan, args=(0.1, 100, 10)))
    
    plt.show()


draw()
