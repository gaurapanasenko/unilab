#!/usr/bin/env python3

import random
import math
import time
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc

plt.rc('text', usetex=True)
plt.rc('font', **{'family': 'verdana'})
# ~ plt.rc('text.latex', unicode=True)
#plt.rc('text.latex', preamble=r'\usepackage[utf8x]{inputenc},\usepackage[russian]{babel},\usepackage{amsmath}')
plt.rcParams['text.latex.preamble'] = r'\usepackage[ukrainian]{babel}'

MAX_N = 1000
T = 10**11

def calc_b(bj):
    k = 2
    return ((7**(4*k+1)*bj)%T)
    
def aperiodic_test_i(a, x, N):
    arr = a[:x]
    for i in range(N):
        if not math.isclose(a[i%x], a[i]):
            return False
    return True
    

def aperiodic_test(arr):
    N = len(arr)
    x = 1
    cycle = False
    while not cycle and x < N:
        cycle = aperiodic_test_i(arr, x, N)
        x += 1
    return not cycle
    
def avg_calc(arr):
    N = len(arr)
    a=sum(arr)/N
    a2 = sum([(i-a)**2 for i in arr])/(N-1)
    return a, a2
    
def coincidence_of_moments_test(arr):
    N = len(arr)
    x=sum(arr)/N
    S2 = sum([(arr[i] - x)**2 for i in range(N)])/(N-1)
    C1 = x-1/2
    C2 = S2-1/12
    if math.sqrt(12*N)*abs(C1) > 1.96:
        return False
    if (N-1)/N*abs(C1)/math.sqrt(0.0056/N+0.0028/N**2-0.0083/N**3) > 1.96:
        return False
    return True
    

def pseudo(N):
    arr = []
    bj = time.time()*1000
    for i in range(N):
        bj = calc_b(bj)
        rj = (bj/T)
        arr.append(rj)
    return arr

def analyze_arr(arr):
    res = []
    for i in range(50, MAX_N, 50):
        myarr = arr[:i]
        test1 = aperiodic_test(myarr)
        test2 = coincidence_of_moments_test(myarr)
        a, a2 = avg_calc(arr[:i])
        res.append((i, a, a2, int(test1), int(test2)))
    res = np.array(res)
    plt.figure()
    plt.scatter(range(MAX_N), arr)
    plt.title(r"Значення випадкових чисел $x_i$ у послідовності")
    fig, axs = plt.subplots(2)
    axs[0].plot(res[:,0], res[:,1], label="Середнє")
    axs[0].set_title(r"Середнє $\overline{x}=\frac{1}{N}\sum_{i=0}^Nx_i$ ")
    axs[1].plot(res[:,0], res[:,2], label="Середньоквадратичне")
    axs[1].set_title(r"Середньоквадратичне $\frac{1}{N-1}\sum_{i=0}^N(x_i-\overline{x})^2$")
    for ax in axs.flat:
        ax.set(xlabel='N', ylabel='')
        ax.label_outer()
    plt.figure()
    plt.plot(res[:,0], res[:,3], label="Аперіодичний тест")
    plt.plot(res[:,0], res[:,4], label="Тест збігу моментів", alpha=0.75)
    plt.legend()
    plt.title(r"Результати тестів")

def main():
    arr = pseudo(MAX_N)
    analyze_arr(arr)
    arr = np.random.rand(MAX_N)
    analyze_arr(arr)
    plt.show()
    

main()
