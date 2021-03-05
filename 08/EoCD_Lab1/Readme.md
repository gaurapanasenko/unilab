# Звіт з лабораторної роботи №1<br />за дисципліною "Елементи хаотичної динаміки"<br />студента групи ПА-17-2<br />Панасенка Єгора Сергійовича<br />Кафедра комп'ютерних технологій<br />ФПМ, ДНУ, 2017-2018 навч.р.<br />

# Постановка задачі

Розробити алгоритм, реалізуючий систему ітерованих функцій (СІФ) виду:

![T_i(x)=\begin{pmatrix}a_i&b_i\\c_i&d_i\end{pmatrix}\cdot x+\begin{pmatrix}e_i\\f_i\end{pmatrix};i=1,\dots,n.](https://render.githubusercontent.com/render/math?math=T_i%28x%29%3D%5Cbegin%7Bpmatrix%7Da_i%26b_i%5C%5Cc_i%26d_i%5Cend%7Bpmatrix%7D%5Ccdot%20x%2B)![](https://render.githubusercontent.com/render/math?math=%5Cbegin%7Bpmatrix%7De_i%5C%5Cf_i%5Cend%7Bpmatrix%7D%3Bi%3D1%2C%5Cdots%2Cn.)

Для даних ![T,i=1,\dots,n](https://render.githubusercontent.com/render/math?math=T,i=1,\dots,n) побудувати перетворення Хатчинсона 

![T(E)=T_1(E)\cup\dots\cup T_n(E)](https://render.githubusercontent.com/render/math?math=T(E)=T_1(E)\cup\dots\cup%20T_n(E))

де коіфіцієнти стискання выдображенб ![T_i](https://render.githubusercontent.com/render/math?math=T_i) вибираються строго меншими за 1. У якості вихідної множини E можна взяти будь яку плоску обмежену фігуру (трикутник, квадрат, прямокутник).

# Iлюстрації фракталів

![Figure_1.png](Figure_1.png)

![Figure_2.png](Figure_2.png)

![Figure_3.png](Figure_3.png)

![Figure_4.png](Figure_4.png)

![Figure_5.png](Figure_5.png)

# Код програми

```python
#!/usr/bin/env python3

fractals = [
    [[ # TRIANGLES
        [[[0.5,0], [0,0.5]], [0,   0  ]],
        [[[0.5,0], [0,0.5]], [0.5, 0  ]],
        [[[0.5,0], [0,0.5]], [0  , 0.5]],
    ], [[0,0], [1,0], [0,1], [0,0]], 6],
    [[ # QUADS
        [[[0.4,0], [0,0.4]], [0,   0  ]],
        [[[0.4,0], [0,0.4]], [0.6, 0  ]],
        [[[0.4,0], [0,0.4]], [0  , 0.6]],
        [[[0.4,0], [0,0.4]], [0.6, 0.6]],
    ], [[0,0], [1,0], [1,1], [0,1], [0,0]], 4],
    [[ # ARROW
        [[[0.3,0], [0,0.3]], [0,     0   ]],
        [[[0.3,0], [0,0.3]], [0.525, 0   ]],
        [[[0.3,0], [0,0.3]], [0.7,   0.35]],
        [[[0.3,0], [0,0.3]], [0,     0.7 ]],
        [[[0.3,0], [0,0.3]], [0.525, 0.7 ]],
    ], [[0,0], [0.75,0], [1,0.5], [0.75,1], [0,1], [0,0]], 4],
    [[ # DANILA
        [[[0.25,0], [0,0.25]], [0.225,  0    ]],
        [[[0.25,0], [0,0.25]], [0.525,  0    ]],
        [[[0.25,0], [0,0.25]], [0,      0.525]],
        [[[0.25,0], [0,0.25]], [0.75,   0.525]],
        [[[0.25,0], [0,0.25]], [0.75/2, 0.75 ]],
        [[[0.5 ,0], [0,0.4 ]], [0.5/2,  0.6/2]],
    ], [[0.5,1], [1,0.7], [0.7,0], [0.3,0], [0,0.7], [0.5,1],], 4],
    [[ # DANILA OUT
        [[[0.4,0], [0,0.4]], [ 0.3, -0.1]],
        [[[0.4,0], [0,0.4]], [-0.1,  0.1]],
        [[[0.4,0], [0,0.4]], [ 0.7,  0.1]],
        [[[0.4,0], [0,0.4]], [ 0.3,  0.3]],
        [[[0.4,0], [0,0.4]], [ 0.0,  0.6]],
        [[[0.4,0], [0,0.4]], [ 0.6,  0.6]],
    ], [[0.5,1], [1,0.7], [0.7,0], [0.3,0], [0,0.7], [0.5,1],], 4],
]

import matplotlib.pyplot as plt
import numpy as np

def arr(*args): return np.array(*args, dtype="float32")

def plot_fractal(shape, t, iterations):
    s = [shape]
    for i in range(iterations + 1):
        for j in s:
            plt.plot([k[0] for k in j], [k[1] for k in j], color="black")
        if i < iterations:
            s = [[l[0].dot(k) + l[1] for k in j] for l in t for j in s]
    plt.show()

for t, s, i in fractals:
    plot_fractal([arr(j) for j in s], [(arr(a), arr(b)) for a, b in t], i);
```
