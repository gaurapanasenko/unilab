#!/usr/bin/env python3

from sympy import *
import numpy as np

def calc(X, t: int = 0) -> np.ndarray:
    x1, x2 = X[0], X[1]
    return np.array([
        -sin(x1)-1/6*x1**3+x2,
        2*x1-x2,
    ])

## @brief Аналіз системи.
def analyze() -> None:
    t = symbols("t")
    x = symbols("x1 x2", cls=Function)
    x = [i(t) for i in x]
    f = calc(x, t)
    
    # ~ I = Matrix([[i.diff(j) for j in x] for i in f])
    print(f)
    I = Matrix(f).jacobian(x)
    print("Якобіан:")
    print(I)
    
    solutions = [(-3.5624684928397796, -7.126450252682265), (0, 0), (3.5624684928397796, 7.126450252682265)]
    print("Особливі точки:")
    print(solutions)

    for X, Y in solutions:
        print()
        print("Точка:", X, Y)
        print(I.subs(x[0], X).subs(x[1], Y).eigenvals())
        eigenvals = list(I.subs(x[0], X).subs(x[1], Y).eigenvals().keys())
        eigenvals = [complex(i) for i in eigenvals]
        print("Власні числа:", eigenvals)
        if all([i.imag != 0 for i in eigenvals]):
            if all([i.real == 0 for i in eigenvals]):
                print("Центр")
                print("Мабуть я у планетарії...")
            elif all([i.real < 0 for i in eigenvals]):
                print("Стійкий фокус")
                print("Голова паморочиться... Але рівновагу не втрачаю і голова на місці.")
            elif all([i.real > 0 for i in eigenvals]):
                print("Нестійкий фокус")
                print("Голова паморочиться... Зовсім рівновагу втрачаю! Ловіть мене!")
            else:
                print("Щось якось обертається, але не знаю як. Розповіси як зустрінимося.")
        elif all([i.imag == 0 for i in eigenvals]):
            if all([i.real < 0 for i in eigenvals]):
                print("Стійкий вузол")
                print("Очі розлазяться, але десь на одній лінії зустрічаються.")
            elif all([i.real > 0 for i in eigenvals]):
                print("Нестійкий вузол")
                print("Очі розлазяться у різні боки.")
            else:
                print("Сідло")
                print("Звичайне кінське сідло. Наскільки комфортне не пробував.")
        else:
            print("А що, так можна було? Комплексні числа у парі зазвичай.")
            print("Негайно передзвони та розповіси про такий випадок!")

analyze()
