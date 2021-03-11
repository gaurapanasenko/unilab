#!/usr/bin/env python3

from sympy import *

t, l = symbols("t l")
x1, x2 = symbols('x1 x2', cls=Function)

f1=x1(t)**2-2*x1(t)*x2(t)+x2(t)**2-9
f2=4*x1(t)**2+x1(t)*x2(t)+4*x2(t)**2-18

sys= [
    Eq(diff(x1(t), t), f1),
    Eq(diff(x2(t), t), f2),
]

I = Matrix([[diff(f1,x1(t)), diff(f1,x2(t))],
[diff(f2,x1(t)), diff(f2,x2(t))]])

print(I)
m = I - Matrix([[1,0],[0,1]])*l
print(m.det())
