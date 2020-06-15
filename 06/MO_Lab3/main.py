#!/usr/bin/env python3

# Вариант 47

import sys
from sympy import symbols, diff, Matrix, Rational, nonlinsolve, latex

ZERO = Rational(0)
ONE = Rational(1)
MAX_SIZE = 2

def my_mult(a, b):
    if isinstance(a, Polinomial):
        return a * b
    return b * a

def to_mat(itr):
    return Matrix(list(itr))

X1, X2, ALP = symbols('x1 x2 alpha')
VARS = (X1, X2)

def calc(f, x):
    return f.subs(list(zip(VARS, x)))

def check_posneg_def(arr):
    pos_def = True
    neg_def = True
    minors = []
    for i in range(1, len(arr[0,:]) + 1):
        minor = arr[:i,:i]
        det = minor.det()
        minors.append((minor, det, ">" if det > 0 else "<"))
        if det <= 0:
            pos_def = False
        if (i % 2 == 0 and det <= 0) or (i % 2 == 1 and det >= 0):
            neg_def = False
    return minors, pos_def, neg_def # pos_def is min, neg_def is max


def classic_method(f):
    df = [diff(f, i) for i in VARS]
    sol = nonlinsolve(df, VARS)
    gesian = to_mat([diff(i, j) for j in VARS] for i in df)
    full_gesian = calc(gesian, list(sol)[0])
    minors, pos_def, neg_def = check_posneg_def(full_gesian)
    return df, sol, gesian, full_gesian, minors, pos_def, neg_def, calc(f, list(sol)[0])


def fast_fall(pol, x_0):
    x_prev = x_i = x_0
    f_prev = f_i = calc(pol, x_i)
    grad = to_mat(diff(pol, i) for i in VARS)
    for i in range(1, 5):
        df_i = calc(grad, x_i)
        x_i_pol = x_i - ALP * df_i
        g_i = calc(pol, x_i_pol)
        dg_i = diff(g_i, ALP)
        alp_i = - dg_i.subs(ALP, 0) / dg_i.coeff(ALP)
        f_prev, x_prev = f_i, x_i
        x_i = x_i_pol.subs(ALP, alp_i)
        f_i = calc(pol, x_i)
        yield i, grad, df_i, x_i_pol, g_i, dg_i, alp_i, x_i, f_i, f_prev, x_prev
        if f_i >= f_prev:
            return x_prev, f_prev

def split_step(f, x_0, eps, lamb=0.5, beta=1):
    x_prev = x_k = x_0
    f_prev = f_k = calc(f, x_k)
    grad = to_mat(diff(f, i) for i in VARS)
    alp = beta
    k = 0
    while True:
        x_k_f = x_k - ALP * calc(grad, x_k)
        while True:
            f_x = x_k_f.subs(ALP, alp)
            # print(alp, lamb, f_x)
            if calc(f, f_x) < calc(f, x_k):
                break
            alp = lamb * alp
        f_prev, x_prev = f_k, x_k
        x_k = f_x
        f_k = calc(f, x_k)
        k += 1
        norm = (x_k - x_prev).norm()
        yield k, x_k, f_k, -calc(grad, x_k), norm
        if norm < eps:
            break


def newton(f, x_0, eps):
    x_prev = x_k = x_0
    f_prev = f_k = calc(f, x_k)
    grad = to_mat(diff(f, i) for i in VARS)
    gesian = to_mat([diff(f, i, j) for j in VARS] for i in VARS)
    gesian_inv = gesian.inv()
    k = 0
    while True:
        f_prev, x_prev = f_k, x_k
        alp_k = calc(gesian_inv, x_k)
        f_prev, x_prev = f_k, x_k
        df_k = calc(grad, x_k)
        x_k = x_k - alp_k * df_k
        f_k = calc(f, x_k)
        k += 1
        norm = (x_k - x_prev).norm()
        if norm < eps:
            break
        yield gesian, gesian_inv, k, alp_k, f_prev, x_prev, x_k, f_k, df_k, norm

def vec_to_str(vec):
    return "\\left(\\begin{array}{c}%s\\end{array}\\right)" % "\\\\".join("%10.4f" % float(i) for i in vec)

def classic_latex(f, name):
    df, sol, gesian, full_gesian, minors, pos_def, neg_def, f_sol = classic_method(f)
    if pos_def:
        type_def = "додатно"
        min_or_max = "мінімуму"
    else:
        type_def = "негативно"
        min_or_max = "максимуму"
    latdf = [latex(i) for i in df]
    return (r"""
\subsubsection{Функція $""" + name + """(x)$}

Знайдемо точку мінімуму функції $""" + name + r"""(x)$ класичним методом.
Градієнт цієї функції має вигляд:""" +
    r"\[" + name + r"'(x)=\left\{\begin{array}{l}" + r"\\".join(latdf) + r"""\end{array}\right.\]

За необхідної умови мінімуму маємо:
\[\left\{\begin{array}{l}""" + r"=0\\".join(latdf) + r"""\end{array}\right.\to\left\{\begin{array}{l}""" +
    r"".join([r"%s=%s\\" % (latex(i[0]), i[1]) for i in zip(VARS, tuple(sol)[0])]) +
    r"""\end{array}\right.\]

Будуємо матрицю других похідних (гесіан):
\[""" + name + r"''(x)=" + latex(gesian, mat_delim="(") + r""".\]

Кутові мінори """ +
    r",".join(r"$M_%s = \left|%s\right| = %s %s 0$" % (i + 1, latex(minor[0], mat_delim=""), minor[1], minor[2]) for i, minor in enumerate(minors)) +
    f". Отже, гесіан є {type_def} визначений, тобто точка $x^* = \left(" + ",".join(str(i) for i in list(sol)[0]) +
    r"\right)$ є точкою " + min_or_max + " і $f(x^*) = " + str(f_sol) + "$.\n")


def fast_fall_latex(f, name, x_0, eps, data):
    return (r"""
\subsubsection{Функція $""" + name + r"""(x)$}

Знайдемо наближення до точки мінімуму функції $""" + name + r"""(x)$ за методом найшвидшого градієнтного спуску.
За початкове наближення візьмемо точку $x^{(0)}=(0,0)^T$.""" +
    "".join(r"""

Ітерація %s.

\[x_{%s} = %s - \alpha_%s %s = %s\]

Для визначення $\alpha_%s$ побудуємо функцію $g(\alpha_%s)$ і знайдемо мінімум цієї функції, тобто:
\[g(\alpha_%s)=%s \to min \sep \alpha_%s \geq 0\]

Знайдемо розв'язок одновимірної задачі оптимізації класичним методом.
\[g'(\alpha_%s)=0 \sep %s=0 \sep \alpha_%s=%s\]

Таким чином, $x^{(%s)} = %s$, $f(x^{(%s)})=%s$.
Умова монотонності виконується, так як $f(x^{(%s)})<f(x^{(%s)})$."""
% (i, i, latex(x_prev, mat_delim="("), i - 1, latex(df_i, mat_delim="("),
latex(x_i_pol, mat_delim="("), i - 1, i - 1, i - 1, latex(g_i, mat_delim="("), i - 1, i - 1,
latex(dg_i, mat_delim="("), i - 1, alp_i, i,
latex(x_i, mat_delim="("), i, f_i, i, i - 1)
for i, grad, df_i, x_i_pol, g_i, dg_i, alp_i, x_i, f_i, f_prev, x_prev in data))


def split_step_latex(f, name, x_0, eps, data):
    return (r"""
\subsubsection{Функція $""" + name + r"""(x)$}

Знайдемо наближення до точки мінімуму функції $""" + name + r"""(x)$ за градієнтним методом с дробленням кроку.
Результати чисельного експерименту наведено у таблиці при початковому наближенні $x^{(0)}=(0,0)^T \sep \lambda = \cfrac15 \sep \beta = 1$.
Ітераційна процедура була зупинена при виконанні умови $\vert x^{(k+1)} - x^{k}\vert \leq \epsilon \ (\epsilon = 10^{""" + str(eps) + r"""})$.

\[\begin{array}{*5{|c}|}\hline
   k & x^{(k)} & f(x^{(k)}) & -f'(x^{k}) & \vert x^{(k+1)} - x^{k}\vert \\\hline""" +
    "\n".join([r"%4s & %s & %10.4f & %s & %10.4f \\\hline" %
    (k, vec_to_str(x_k), float(f_k), vec_to_str(ff_k), norm) for k, x_k, f_k, ff_k, norm in data]) +
    r"""
\end{array}\]
""")


def newton_latex(f, name, x_0, eps, data):
    return (r"""
\subsubsection{Функція $""" + name + r"""(x)$}

Знайдемо наближення до точки мінімуму функції $""" + name + r"""(x)$ за методом Ньютона.
За початкове наближення візьмемо точку $x^{(0)}=(0,0)^T$.
Ітераційна процедура була зупинена при виконанні умови $\vert x^{(k+1)} - x^{k}\vert \leq \epsilon \ (\epsilon = 10^{""" + str(eps) + r"""})$.
Матриця других похідних для квадратичної функції є постійною і для заданої функції має вигляд:
\[""" + name + r"''(x)=" + latex(data[0][0], mat_delim="(") + r""".\]

Обернена до матриці Гессе $f''(x)$ матриця є
\[(f''(x))^{-1}=""" + latex(data[0][1], mat_delim="(") + r""".\]""" +
    "".join(r"""

Ітерація %s.

\[x_{%s} = %s - %s %s = %s\]"""
% (k, k, latex(x_prev, mat_delim="("), latex(alp_k, mat_delim="("), latex(df_k, mat_delim="("), latex(x_k, mat_delim="("))
for gesian, gesian_inv, k, alp_k, f_prev, x_prev, x_k, f_k, df_k, norm in data))


def fast_fall_plot(data):
    return "\n".join([r"    (%s,%s)" % (",".join(str(j) for j in i[-4]), i[-3]) for i in data])


def split_step_plot(data):
    return "\n".join([r"    (%s,%s)" % (",".join(str(i) for i in x_k), f_k) for k, x_k, f_k, ff_k, norm in data])


def newton_plot(data):
    return "\n".join([r"    (%s,%s)" % (",".join(str(i) for i in x_k), f_k) for gesian, gesian_inv, k, alp_k, f_prev, x_prev, x_k, f_k, df_k, norm in data])


def geom_inter_latex(fff, ssf, nf):
    return (r"""\section{Геометрична інтерпретація}
\begin{center}
\begin{tikzpicture}
\begin{axis}[view={0}{90}]
\addplot3 [
    very thick,
    contour gnuplot={
        levels={-3,-4,-5,-6},
        draw color=black
    },
    samples=60
] {2*x^2 + x*y + y^2 - 2*x - 5*y};
\addplot3[color=red] coordinates {
""" + fast_fall_plot(fff) + r"""
};
\addplot3[color=green] coordinates {
""" + split_step_plot(ssf) + r"""
};
\addplot3[color=blue] coordinates {
""" + newton_plot(nf) + r"""
};
\legend{$f(x)$,швид.,дроб.,ньют.};
\end{axis}
\end{tikzpicture}
\end{center}""")

def main(args):
    exm = 7 * X1**2 + 4 * X1 * X2 + 2 * X2 ** 2 - 10 * X1
    f = 2 * X1**2 + X1 * X2 + X2**2 - 2 * X1 - 5 * X2
    g = 100 * (X2 - X1**2)**2 + (- X1 + 1)**2
    x_0 = Matrix([0, 0])
    fff = list(fast_fall(f, x_0))
    ssf = list(split_step(f, x_0, 10**-3))
    ssg = list(split_step(g, x_0, 10**-2))
    nf = list(newton(f, x_0, 10**-4))
    ng = list(newton(g, x_0, 10**-4))


    print(r"\subsection{Класичний метод}")
    print(classic_latex(f, "f"))
    print(classic_latex(g, "g"))

    print(r"\subsection{Метод найшвидшого спуску}")
    print(fast_fall_latex(f, "f", x_0, -4, fff))

    print(r"\subsection{Метод з дробленням кроку}")
    print(split_step_latex(f, "f", x_0, -3, ssf))
    print(split_step_latex(g, "g", x_0, -2, ssg))

    print(r"\subsection{Метод Ньютона}")
    print(newton_latex(f, "f", x_0, -4, nf))
    print(newton_latex(g, "g", x_0, -4, ng))

    print(geom_inter_latex(fff, ssf, nf))

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
