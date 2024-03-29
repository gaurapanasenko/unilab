#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import minimize


# Inspiration: https://pythonhosted.org/scikit-fuzzy/auto_examples/plot_tipping_problem.html

def mf(x, b, c):
    return 1/(1+((x-b)/c)**2)

data = (
    (15,  0, 4),
    (15,  5, 2),
    (15, 10, 0),
    (15, 20, 0),
    (15, 40, 0),
    
    (20,  0, 4),
    (20,  5, 2),
    (20, 10, 0),
    (20, 20, 0),
    (20, 40, 0),
    
    (25,  0, 9),
    (25,  5, 6),
    (25, 10, 4),
    (25, 20, 2),
    (25, 40, 0),
    
    (32,  0, 9),
    (32,  5, 9),
    (32, 10, 6),
    (32, 20, 4),
    (32, 40, 2),
    
    (40,  0, 9),
    (40,  5, 9),
    (40, 10, 9),
    (40, 20, 6),
    (40, 40, 4),
    
)

x_temp = np.linspace(15, 40, 200)
x_expr = np.linspace(0, 30, 200)
x_powr = np.linspace(0, 10, 200)

consts = np.array([15, 2, 25, 4, 40, 2, 0, 1, 10, 4, 30, 6, 0, 1, 4, 1, 9, 1], dtype="float32")

temp_lo_f = lambda x, c: mf(x, c[0], c[1])
temp_md_f = lambda x, c: mf(x, c[2], c[3])
temp_hi_f = lambda x, c: mf(x, c[4], c[5])
expr_sh_f = lambda x, c: mf(x, c[6], c[7])
expr_md_f = lambda x, c: mf(x, c[8], c[9])
expr_ln_f = lambda x, c: mf(x, c[10], c[11])
powr_lo_f = lambda x, c: mf(x, c[12], c[13])
powr_md_f = lambda x, c: mf(x, c[14], c[15])
powr_hi_f = lambda x, c: mf(x, c[16], c[17])

def mu_lo(x, c):
    return max([
        min(temp_lo_f(x[0], c), expr_md_f(x[1], c)),
        min(temp_lo_f(x[0], c), expr_ln_f(x[1], c)),
        min(temp_md_f(x[0], c), expr_ln_f(x[1], c)),
    ])
    

def mu_md(x, c):
    return max([
        min(temp_lo_f(x[0], c), expr_sh_f(x[1], c)),
        min(temp_md_f(x[0], c), expr_md_f(x[1], c)),
        min(temp_hi_f(x[0], c), expr_ln_f(x[1], c)),
    ])

def mu_hi(x, c):
    return max([
        min(temp_md_f(x[0], c), expr_sh_f(x[1], c)),
        min(temp_hi_f(x[0], c), expr_sh_f(x[1], c)),
        min(temp_hi_f(x[0], c), expr_md_f(x[1], c)),
    ])
    
def defuzz(x, c):
    # ~ print(mu_lo(x), mu_md(x), mu_hi(x))
    return (c[12]*mu_lo(x, c)+c[14]*mu_md(x, c)+c[16]*mu_hi(x, c))/(mu_lo(x, c)+mu_md(x, c)+mu_hi(x, c))

def solve(x, c):
    print("%2.f and %2.f -> %0.3f" % (x[0], x[1], defuzz(x, c)))
    
def J(c):
    return sum((defuzz([i[0], i[1]], c)-i[2])**2 for i in data)

res = minimize(J, consts, options={'disp': True})
fin_consts = res.x
print(fin_consts)

def show_result(c):
    print()
    for x1, x2, y in data:
        to = temp_lo_f(x1,c)
        tm = temp_md_f(x1,c)
        th = temp_hi_f(x1,c)
        eo = expr_sh_f(x2,c)
        em = expr_md_f(x2,c)
        eh = expr_ln_f(x2,c)
        s = defuzz((x1, x2), c)
        print("%2i,%2i,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f" % (x1, x2, to, tm, th, eo, em, eh, s))

    fig, (ax0, ax1, ax2) = plt.subplots(nrows=3, figsize=(8, 9))

    temp_lo = temp_lo_f(x_temp, c)
    temp_md = temp_md_f(x_temp, c)
    temp_hi = temp_hi_f(x_temp, c)
    expr_sh = expr_sh_f(x_expr, c)
    expr_md = expr_md_f(x_expr, c)
    expr_ln = expr_ln_f(x_expr, c)
    powr_lo = powr_lo_f(x_powr, c)
    powr_md = powr_md_f(x_powr, c)
    powr_hi = powr_hi_f(x_powr, c)

    ax0.plot(x_temp, temp_lo, 'b', linewidth=1.5, label='Low')
    ax0.plot(x_temp, temp_md, 'g', linewidth=1.5, label='Medium')
    ax0.plot(x_temp, temp_hi, 'r', linewidth=1.5, label='High')
    ax0.set_title('Temperature')
    ax0.legend()

    ax1.plot(x_expr, expr_sh, 'b', linewidth=1.5, label='Short')
    ax1.plot(x_expr, expr_md, 'g', linewidth=1.5, label='Agerage')
    ax1.plot(x_expr, expr_ln, 'r', linewidth=1.5, label='Long')
    ax1.set_title('Expiration days')
    ax1.legend()

    ax2.plot(x_powr, powr_lo, 'b', linewidth=1.5, label='Low')
    ax2.plot(x_powr, powr_md, 'g', linewidth=1.5, label='Medium')
    ax2.plot(x_powr, powr_hi, 'r', linewidth=1.5, label='High')
    ax2.set_title('Power')
    ax2.legend()
    plt.tight_layout()
    

show_result(consts)
show_result(fin_consts)
plt.show()
