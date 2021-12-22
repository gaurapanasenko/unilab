import matplotlib.pyplot as plt
import numpy as np

# Inspiration: https://pythonhosted.org/scikit-fuzzy/auto_examples/plot_tipping_problem.html

def mf(x, b, c):
    return 1/(1+((x-b)/c)**2)
    
x3_ = 0
x31 = 2
x3m = 4
x32 = 7
x33 = 9

x_temp = np.arange(15, 40, 1)
x_expr = np.arange(0, 30, 1)
x_powr = np.arange(0, 10, 1)

temp_lo_f = lambda x: mf(x, 15, 2)
temp_md_f = lambda x: mf(x, 25, 4)
temp_hi_f = lambda x: mf(x, 40, 2)
expr_sh_f = lambda x: mf(x, 0, 1)
expr_md_f = lambda x: mf(x, 10, 4)
expr_ln_f = lambda x: mf(x, 30, 6)
powr_lo_f = lambda x: mf(x, 0, 1)
powr_md_f = lambda x: mf(x, 4, 1)
powr_hi_f = lambda x: mf(x, 9, 1)

temp_lo = temp_lo_f(x_temp)
temp_md = temp_md_f(x_temp)
temp_hi = temp_hi_f(x_temp)
expr_sh = expr_sh_f(x_expr)
expr_md = expr_md_f(x_expr)
expr_ln = expr_ln_f(x_expr)
powr_lo = powr_lo_f(x_powr)
powr_md = powr_md_f(x_powr)
powr_hi = powr_hi_f(x_powr)

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

def mu_lo(x):
    return max([
        min(temp_lo_f(x[0]), expr_md_f(x[1])),
        min(temp_lo_f(x[0]), expr_ln_f(x[1])),
        min(temp_md_f(x[0]), expr_ln_f(x[1])),
    ])
    

def mu_md(x):
    return max([
        min(temp_lo_f(x[0]), expr_sh_f(x[1])),
        min(temp_md_f(x[0]), expr_md_f(x[1])),
        min(temp_hi_f(x[0]), expr_ln_f(x[1])),
    ])

def mu_hi(x):
    return max([
        min(temp_md_f(x[0]), expr_sh_f(x[1])),
        min(temp_hi_f(x[0]), expr_sh_f(x[1])),
        min(temp_hi_f(x[0]), expr_md_f(x[1])),
    ])
    
def defuzz(x):
    # ~ print(mu_lo(x), mu_md(x), mu_hi(x))
    return (x3_*mu_lo(x)+x3m*mu_md(x)+x33*mu_hi(x))/(mu_lo(x)+mu_md(x)+mu_hi(x))

def solve(x):
    print("%2.f and %2.f -> %0.3f" % (x[0], x[1], defuzz(x)))

solve((15, 0))
solve((25, 10))
solve((25, 10))
solve((25, 20))
solve((40, 30))
solve((15, 30))

plt.show()
