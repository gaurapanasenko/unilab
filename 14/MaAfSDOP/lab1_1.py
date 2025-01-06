from collections import defaultdict
import math

def gen_sols(sol: tuple[int, ...], n, r) -> tuple[tuple[int, ...], ...]:
    """Генерація всіх можливих сполучень для заданого n та r."""
    if len(sol) == r:
        return (sol,) if sum(sol) == n else tuple()
    out: tuple[tuple[int, ...], ...] = tuple()
    for i in range(n - sum(sol), -1, -1):
        out = out + gen_sols(sol + (i,), n, r)
    return out

def solve_and_display(n, r, limits):
    """Обчислення і друк рішень для заданих параметрів."""
    print("=" * 50)
    print(f"Рівняння: {' + '.join([f'x_{i}' for i in range(1, r + 1)])} = {n}")
    print("Обмеження: " + ", ".join([f"x_{i} >= {limits[i - 1]}" for i in range(1, r + 1)]))
    print("=" * 50)
    
    # Перевірка кількості комбінацій за допомогою формули
    n_adjusted = n - sum(limits)
    count = int(math.factorial(n_adjusted + r - 1) / (math.factorial(n_adjusted) * math.factorial(r - 1)))
    print(f"Кількість теоретичних сполучень: {count}")
    if count > 10**10:
        print(f"Неможливо перебрати всі рішення, бо кількість > {10**10}")
        return

    # Генерація всіх можливих комбінацій
    mixed_solutions = gen_sols(tuple(), n_adjusted, r)
    assert len(mixed_solutions) == count, "Генерація комбінацій не відповідає формулі."
    print("Кількість створених сполучень відповідає формулі.")

    # Приведення результатів до врахування обмежень
    all_solutions = defaultdict(list)
    for comb in mixed_solutions:
        adjusted_comb = tuple(val + limits[j] for j, val in enumerate(comb))
        all_solutions[max(adjusted_comb)].append(adjusted_comb)

    # Друк результатів
    print("Рішення згруповані за максимальним значенням змінних:")
    for max_val, group in all_solutions.items():
        print(f"Максимальне значення: {max_val}, Рішення: {group}")
    print()

# Параметри задачі
n = int(input("Введіть n: "))
r = int(input("Введіть r: "))

if int(r) <= 0 and int(n) <= 0:
    print("Ввели r<=0 або n<=0.")
    exit(0)


# Виконання програми для обох випадків обмежень
solve_and_display(n, r, [0] * r)  # Невід'ємні змінні
solve_and_display(n, r, [1] * r)  # Додатні змінні
