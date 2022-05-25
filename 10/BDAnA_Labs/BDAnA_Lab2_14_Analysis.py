#!/usr/bin/env python3

from sympy import *
import numpy as np
init_printing(use_unicode=False)

def calc(X, t, k, K) -> np.ndarray:
    x, y, z = X
    k1,k2,k3,k4,k5,k6,k7,k8 = k
    K1, K2 = K
    return np.array([
        # ~ k1+k2*x-(k3*y+k4*z)*x/(x+K1),
        k1+k2*x-(k3*y)*x/(x+K1),
        k5*x-k6*y,
        k7*x-k8*z/(z+K2)
    ])

## @brief Аналіз системи.
def analyze() -> None:
    t = symbols("t")
    x = symbols("x y z", cls=Function)
    x = [i(t) for i in x]
    k = symbols("k1:9")
    K = symbols("K1:3")
    f = calc(x, t, k, K)
    
    # ~ I = Matrix([[i.diff(j) for j in x] for i in f])
    pprint(Matrix(f))
    I = Matrix(f).jacobian(x)
    pprint("Якобіан:")
    pprint(I)
    
    solutions = [list(i.values()) for i in solve(f)]
    pprint("Особливі точки:")
    pprint(solutions)
    solutions = [(0,0,0)]

    for X, Y, Z in solutions:
        print()
        print("Точка:", (X, Y, Z))
        res = lambdify(x, I, modules="sympy")(X, Y, Z).eigenvals()
        pprint(res)
        eigenvals = list(res.keys())
        pprint(eigenvals)
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
