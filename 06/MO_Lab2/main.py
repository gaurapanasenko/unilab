#!/usr/bin/env python

# Вариант 47

import sys
from decimal import Decimal, getcontext
# Decimal - Дозволяє робити операції з float числами більш точно
import math

def func(x: Decimal) -> Decimal:
    return x.ln() - 2 * (x ** 2) # -> max

# Метод ділення навпіл

def half_divide(f, a, b, eps, delta):
    k = 1
    lamb = (a + b) / 2 - delta
    mu = (a + b) / 2 + delta
    x = (a + b) / 2
    while True:
        if b - a < eps:
            x = (a + b) / 2
            yield a, b, lamb, mu, x
            return
        lamb = (a + b) / 2 - delta
        mu = (a + b) / 2 + delta
        yield a, b, lamb, mu, x
        if f(lamb) <= f(mu):
            a, b, x = a, mu, lamb
        else:
            a, b, x = lamb, b, mu

# Метод золотого перерізу

GR1 = (3 - Decimal(5).sqrt()) / 2
GR2 = (Decimal(5).sqrt() - 1) / 2

def golden_section(f, a, b, eps, _):
    x = (a + b) / 2
    while True:
        lamb = a + (b - a) * GR1
        mu = a + (b - a) * GR2
        yield a, b, lamb, mu, x

        if abs(lamb - mu) < eps:
            x = (a + b) / 2
            yield a, b, lamb, mu, x
            return

        if f(lamb) <= f(mu):
            b, x = mu, lamb
        else:
            a, x = lamb, mu

# Метод Фібоначчі

def fibonacci(f, a, b, eps, _):
    fib = [Decimal(1), Decimal(1)]
    j = 1
    diff = b - a
    x = diff / eps
    while fib[-2] > x or x > fib[-1]:
        fib.append(fib[-2] + fib[-1])
    n = len(fib) - 3
    x = (a + b) / 2
    for i in range(n):
        if i == 0:
            lamb = a + (fib[n] / fib[n + 2]) * diff
            mu = a + (fib[n + 1] / fib[n + 2]) * diff
        yield a, b, lamb, mu, x
        if f(lamb) <= f(mu):
            b, mu, x = mu, lamb, lamb
            lamb = a + (fib[n - i - 1] / fib[n + 2]) * diff
        else:
            a, lamb, x = lamb, mu, mu
            mu = a + (fib[n - i] / fib[n + 2]) * diff
    x = (a + b) / 2
    yield a, b, lamb, mu, x


def main(args):
    a = Decimal(1) / Decimal(4)
    b = Decimal(2)
    margs = func, a, b, Decimal(10) ** -3, Decimal(10) ** -4
    methods = (half_divide, golden_section, fibonacci)
    data = []
    for i in methods:
        dat = []
        data.append(dat)
        for j in i(*margs):
            dat.append(j)
    out_data = []
    for i in data.copy():
        data_i = []
        out_data.append(data_i)
        for j in range(len(i) - 1):
            data_j1, data_j2 = [], []
            data_i.append((data_j1, data_j2))
            for k, v in enumerate((0, 0, 1, 1, 0)):
                val = i[j + v][k]
                data_j1.append(val)
                data_j2.append(func(val))
    for i in out_data:
        for j, v in enumerate(i):
            print(" & ".join(["%3i" % (j + 1)] + ["%8.5f" % k for k in v[0]]), "\\\\*")
            print(" & ".join(["   "] + ["%8.5f" % k for k in v[1]]), "\\\\\\hline")
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
