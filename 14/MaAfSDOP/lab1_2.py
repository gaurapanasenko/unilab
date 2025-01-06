import sys
import time
import argparse
import numpy as np
from scipy.optimize import curve_fit

# Precomputed n values for exponential fitting
PRECOMPUTED_N = np.array([20, 30, 40])

def calc_sum(coefficients):
    """Calculate the sum \u2211(i * x_i)."""
    return sum((i + 1) * coefficients[i] for i in range(len(coefficients)))

def build_tree(n, coefficients=None):
    """Recursively build solutions to \u2211(i * x_i) = n."""
    if coefficients is None:
        coefficients = []

    if len(coefficients) == n:
        return [coefficients] if calc_sum(coefficients) == n else []

    if calc_sum(coefficients) > n:
        return []

    return build_tree(n, coefficients + [0]) + build_tree(n, coefficients + [1])

def measure_precomputed_times():
    """Measure actual calculation times for precomputed n values."""
    precomputed_times = []
    for n in PRECOMPUTED_N:
        start_time = time.time()
        build_tree(n)
        end_time = time.time()
        precomputed_times.append(end_time - start_time)
    return np.array(precomputed_times)

def exponential_fit(x, a, b):
    """Exponential function for fitting: y = a * exp(b * x)."""
    return a * 2 ** (b * x)

def estimate_calculation_time(n, params):
    """Estimate the time required for the calculation using the exponential fit."""
    a, b = params
    return exponential_fit(n, a, b)

def format_solution(solution):
    """Format a solution as a formula."""
    terms = [f"{i + 1}*x_{i + 1}" for i, x in enumerate(solution) if x == 1]
    return " + ".join(terms) + f" = {sum((i + 1) * solution[i] for i in range(len(solution)))}"

def main():
    parser = argparse.ArgumentParser(description="Знайти розв'язки рівняння \u2211(i * x_i) = n.")
    parser.add_argument("n", type=int, nargs="?", help="Цільове значення n для рівняння.")
    parser.add_argument("-y", action="store_true", help="Пропустити підтвердження для великих значень n.")
    args = parser.parse_args()

    # Get target value n from arguments or input
    if args.n is not None:
        n = args.n
    else:
        try:
            n = int(input("Введіть цільове значення n: "))
        except ValueError:
            print("Неправильне введення. Будь ласка, введіть ціле число.")
            sys.exit(1)

    # Validate n
    if n < 0:
        print("Значення n не може бути від'ємним.")
        sys.exit(1)

    print(f"Кількість варіантів розв'язків: {2 ** n}")

    # Measure precomputed times and fit exponential curve
    precomputed_times = measure_precomputed_times()
    params, _ = curve_fit(exponential_fit, PRECOMPUTED_N, precomputed_times)

    # Estimate calculation time
    estimated_time = estimate_calculation_time(n, params)
    print(f"Оцінений час для обчислення: {estimated_time:.2f} секунд.")

    # Confirm calculation if time is large
    if estimated_time >= 10 and not args.y:
        print(f"Обчислення для n = {n} оцінюється в {estimated_time:.2f} секунд.\nВи впевнені, що хочете продовжити? (y/n)")
        confirm = input().strip().lower()
        if confirm != 'y':
            print("Обчислення скасовано.")
            sys.exit(0)

    # Perform calculation
    start_time = time.time()
    solutions = build_tree(n)
    end_time = time.time()

    # Output results
    print(f"Знайдено {len(solutions)} розв'язків для n = {n}:")
    for solution in solutions:
        print(format_solution(solution))

    print(f"Обчислення завершено за {end_time - start_time:.2f} секунд.")

if __name__ == "__main__":
    main()
