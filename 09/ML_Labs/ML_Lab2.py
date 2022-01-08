#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import minimize


# Inspiration: https://pythonhosted.org/scikit-fuzzy/auto_examples/plot_tipping_problem.html

def mf(x, b, c):
    return 1/(1+((x-b)/c)**2)

data = (
    (15,  0, 4),
    (25, 10, 3),
    (25, 10, 3),
    (25, 20, 1),
    (40, 30, 4),
    (15, 30, 0),
)

x3_ = 0
x31 = 2
x3m = 4
x32 = 7
x33 = 9

x_temp = np.arange(15, 40, 1)
x_expr = np.arange(0, 30, 1)
x_powr = np.arange(0, 10, 1)

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

temp_lo = temp_lo_f(x_temp, consts)
temp_md = temp_md_f(x_temp, consts)
temp_hi = temp_hi_f(x_temp, consts)
expr_sh = expr_sh_f(x_expr, consts)
expr_md = expr_md_f(x_expr, consts)
expr_ln = expr_ln_f(x_expr, consts)
powr_lo = powr_lo_f(x_powr, consts)
powr_md = powr_md_f(x_powr, consts)
powr_hi = powr_hi_f(x_powr, consts)

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
    return (x3_*mu_lo(x, c)+x3m*mu_md(x, c)+x33*mu_hi(x, c))/(mu_lo(x, c)+mu_md(x, c)+mu_hi(x, c))

def solve(x, c):
    print("%2.f and %2.f -> %0.3f" % (x[0], x[1], defuzz(x, c)))
    
def J(c):
    return sum((defuzz([i[0], i[1]], c)-i[2])**2 for i in data)

res = minimize(J, consts, options={'disp': True})
fin_consts = res.x
print(fin_consts)

def show_result(c):
    solve((15, 0), c)
    solve((25, 10), c)
    solve((25, 10), c)
    solve((25, 20), c)
    solve((40, 30), c)
    solve((15, 30), c)

    fig, (ax0, ax1, ax2) = plt.subplots(nrows=3, figsize=(8, 9))

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
