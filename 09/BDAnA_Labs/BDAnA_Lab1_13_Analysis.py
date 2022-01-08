#!/usr/bin/env python3

from sympy import *
import numpy as np
init_printing(use_unicode=False)

def calc(X, t: int = 0) -> np.ndarray:
    x1, x2, x3 = X[0], X[1], X[2]
    return np.array([
        -x1+2*x2, -2*x2, -2*x1-2*x2-x3
    ])

## @brief Аналіз системи.
def analyze() -> None:
    t = symbols("t")
    x = symbols("x1 x2 x3", cls=Function)
    x = [i(t) for i in x]
    f = calc(x, t)
    
    # ~ I = Matrix([[i.diff(j) for j in x] for i in f])
    pprint(Matrix(f))
    I = Matrix(f).jacobian(x)
    pprint("Якобіан:")
    pprint(I)
    
    solutions = [list(i.values()) for i in solve(f)]
    pprint("Особливі точки:")
    pprint(solutions)

    for X, Y, Z in solutions:
        print()
        print("Точка:", X, Y, Z)
        pprint(lambdify(x, I, modules="sympy")(X, Y, Z).eigenvals())
        eigenvals = list(lambdify(x, I, modules="sympy")(X, Y, Z).eigenvals().keys())
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
