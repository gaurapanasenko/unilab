import argparse
import math
import time
from collections import defaultdict

MAX_TIME = 10

def gen_sols(sol: tuple[int, ...], n, r):
    """Генерація всіх можливих сполучень для заданого n та r."""
    if len(sol) == r:
        if sum(sol) == n:
            yield sol
        return
    for i in range(n - sum(sol), -1, -1):
        for sol_j in gen_sols(sol + (i,), n, r):
            yield sol_j

def seconds_to_human_readable(seconds):
  """Перетворює кількість секунд у читабельний формат часу з урахуванням років та місяців.

  Args:
    seconds: Кількість секунд.

  Returns:
    Строка у форматі "X років, Y місяців, Z днів, A годин, B хвилин, C секунд".
  """

  # Отримуємо компоненти часу
  days, remainder = divmod(seconds, 24*60*60)
  hours, remainder = divmod(remainder, 3600)
  minutes, seconds = divmod(remainder, 60)

  # Розраховуємо роки та місяці
  years, remainder = divmod(days, 365.25)
  months, days = divmod(remainder, 30.44)

  # Округляємо роки та місяці до цілих чисел
  years, months, days = int(years), int(months), int(days)
  hours, minutes, seconds = int(hours), int(minutes), int(seconds)

  # Формуємо результат
  out = []
  if years:
    out.append(f"{years} років")
  if months:
    out.append(f"{months} місяців")
  if days:
    out.append(f"{days} днів")
  if hours:
    out.append(f"{hours} годин")
  if minutes:
    out.append(f"{minutes} хвилин")
  if seconds:
    out.append(f"{seconds} секунд")
  return ", ".join(out)


def solve_and_display(n, r, limits, args):
    """Обчислення і друк рішень для заданих параметрів."""
    print("=" * 50)
    print(f"Рівняння: {' + '.join([f'x_{i}' for i in range(1, r + 1)])} = {n}")
    print("Обмеження: " + ", ".join([f"x_{i} >= {limits[i]}" for i in range(r)]))
    print("=" * 50)
    
    n_adjusted = n - sum(limits)
    if n_adjusted < 0:
        print("Неможливо знайти рішення з такими обмеженнями.")
        return

    count = int(math.factorial(n_adjusted + r - 1) / (math.factorial(n_adjusted) * math.factorial(r - 1)))
    print(f"Кількість теоретичних сполучень: {count}")

    start = time.time()
    mixed_solutions = []
    
    for i in gen_sols(tuple(), n_adjusted, r):
        mixed_solutions.append(i)
        cur = time.time()
        if cur - start > MAX_TIME and not args.y:
            estimate = MAX_TIME * count / len(mixed_solutions)
            print(f"Програма працює 10 секунд. Згенеровано {len(mixed_solutions)} сполучень.")
            print(f"Очікуваний час виконання {seconds_to_human_readable(estimate)}.")
            print("Чи бажаєте продовжити?")
            confirm = input("Продовжити? (y/n): ").strip().lower()
            if confirm != "y":
                print("Операцію скасовано.")
                return

    assert len(mixed_solutions) == count, "Генерація комбінацій не відповідає формулі."
    print("Кількість створених сполучень відповідає формулі.")

    all_solutions = defaultdict(list)
    for comb in mixed_solutions:
        adjusted_comb = tuple(val + limits[j] for j, val in enumerate(comb))
        all_solutions[max(adjusted_comb)].append(adjusted_comb)

    print("Рішення згруповані за максимальним значенням змінних:")
    for max_val, group in all_solutions.items():
        print(f"Максимальне значення: {max_val}, Рішення: {group}")
    print()

def get_limits(r):
    """Отримати обмеження від користувача."""
    print("Оберіть тип обмежень:")
    print("1. Невід’ємні (x_i >= 0)")
    print("2. Додатні (x_i >= 1)")
    print("3. Ввести свої обмеження")
    choice = input("Ваш вибір (1/2/3): ").strip()
    if choice == "1":
        return [0] * r
    elif choice == "2":
        return [1] * r
    elif choice == "3":
        print("Введіть обмеження для кожної змінної через пробіл (наприклад, 2 3 0):")
        limits = list(map(int, input().split()))
        if len(limits) != r:
            raise ValueError("Кількість обмежень має дорівнювати кількості змінних r.")
        return limits
    else:
        raise ValueError("Неправильний вибір. Спробуйте ще раз.")

def main():
    parser = argparse.ArgumentParser(description="Розв'язання рівняння суми з обмеженнями.")
    parser.add_argument("n", type=int, nargs="?", help="Значення n")
    parser.add_argument("r", type=int, nargs="?", help="Кількість змінних r")
    parser.add_argument("l", type=int, nargs="*", help="Обмеження для x_i (наприклад, 0 1 2)")
    parser.add_argument("-y", action="store_true", help="Пропустити підтвердження для великих обчислень")
    parser.add_argument("-z", action="store_true", help="Взяти x_i невідємні")
    parser.add_argument("-p", action="store_true", help="Взяти x_i додатні")
    args = parser.parse_args()

    if args.n is None or args.r is None:
        n = int(input("Введіть n: "))
        r = int(input("Введіть r: "))
    else:
        n, r = args.n, args.r

    if n <= 0 or r <= 0:
        print("Помилка: n і r мають бути додатними цілими числами.")
        return

    if n > 100 or r > 100:
        print("Помилка: n і r мають бути меншими за 100.")
        return

    if args.l:
        if len(args.l) != r:
            print("Помилка: кількість обмежень має дорівнювати кількості змінних r.")
            return
        limits = args.l
    elif args.z:
        limits = [0] * r
    elif args.p:
        limits = [1] * r
    else:
        limits = get_limits(r)

    solve_and_display(n, r, limits, args)

if __name__ == "__main__":
    main()
