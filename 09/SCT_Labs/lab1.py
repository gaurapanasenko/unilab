#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
from sympy import *
from IPython.display import display, Latex
init_printing(use_latex='mathjax')
pprint = display

a, m, c, t, T = symbols("a, m, c, t, T")
x, lamb, u = symbols("x, lambda, u", cls=Function)
x, lamb, u = x(t), lamb(t), u(t)

def solv(f0, arg=0):
    e = Eq(diff(x,t)+a*x, u)
    print("Розглянемо диференціальне рівняння:")
    pprint(e)
    print("Та супроводжуючий функціонал:")
    pprint(Eq(symbols("I1"),Integral(f0, (t, 0, oo))))
    F = f0 + lamb * (e.lhs - e.rhs)
    print("Використаємо метод множників Лагранжа та роздивимось функціонал:")
    pprint(Eq(symbols("J_lambda"),Integral(F, (t, symbols("t0"), T))))
    print("Для отримання екстремалей цього функціоналу вирішимо рівняння Эйлера:")
    eq1 = Eq(diff(F, x) - diff(diff(F, diff(x, t)), t), 0)
    eq2 = Eq(diff(F, u) - diff(diff(F, diff(u, t)), t), 0)
    sys = Matrix([eq1, eq2])
    pprint(sys)
    sys = sys.subs(u, e.lhs)
    print("Вираземо диференціальне рівняння з постановки задачі через u та підставимо у систему:")
    pprint(sys)
    sol = dsolve(sys, [x, lamb])
    print("Вирішимо систему:")
    pprint(sol[0])
    pprint(sol[1])
    xe = sol[0].rhs.args[arg]
    print("З розглянутого рішення потрібно залишити тільки сталу екстремаль, тому:")
    pprint(Eq(x, xe))
    dxe = diff(xe, t)
    print("Знайдемо похідну отриманого рішення:")
    pprint(Eq(diff(x,t), dxe))
    dxe = simplify(dxe/xe)*x
    print("Виразимо через x заміною")
    pprint(Eq(diff(x,t), dxe))
    sol = e.subs(diff(x, t), dxe)
    print("Підставимо отриманий рішення та похідну у дифференціальне рівняння з постановки задачі та отримаємо наше управління:")
    pprint(sol)
    sol = solve(sol, u)[0]
    sol = solve(e.subs(u, sol), diff(x,t))[0]
    print("Підставляючи управління до того ж самого рівняння отримаємо диференціальне рівняння з однією функцією x:")
    pprint(Eq(diff(x,t), sol))
    return lambdify([x, t, a, m, c], sol)


def draw() -> None:
    tspan = np.linspace(0, 10)

    plt.plot(tspan, odeint(sys1, 8.5, tspan, args=(0.1, 50, 50)))
    plt.plot(tspan, odeint(sys1, 3, tspan, args=(0.1, 50, 50)))
    plt.plot(tspan, odeint(sys1, -3, tspan, args=(0.1, 50, 50)))
    plt.plot(tspan, odeint(sys1, 0, tspan, args=(0.1, 50, 50)))
    plt.plot(tspan, odeint(sys1, -8.5, tspan, args=(0.1, 60, 30)))
    
    plt.xlabel("t")
    plt.ylabel("x")
    plt.show()


sys1 = solv((m**2-c**2*a**2) * x**2 - 2*a*c**2*u + c**2*u**2, 1)
print(" ")
print(" ")
sys2 = solv((m**2-c**2*a**2) * x**2 + c**2*u**2, 0)
print(" ")
print(" ")
print("Завдяки тому що ми отримали однаковий оптимальний розв'язок для обох супроводжуючи функціоналів, можна побудувати графік зміни значення x в залежності від часу t:")
draw()
