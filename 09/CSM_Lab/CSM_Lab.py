#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from datetime import tzinfo, timedelta, date

N = 96
h = 60*60*24*7
g = 9.81
H = 50

weeks = np.arange(0, N, 1)
y = np.array([793, 678, 517, 947, 882, 948, 877, 939, 916, 965, 772, 614, 652, 555, 849, 866, 914, 967, 758, 579, 769, 606, 678, 518, 946, 885, 953, 853, 878, 940, 910, 969, 742, 547, 875, 934, 929, 943, 899, 967, 755, 572, 793, 676, 519, 943, 897, 966, 763, 591, 727, 524, 933, 933, 935, 928, 946, 885, 952, 858, 891, 960, 808, 727, 524, 935, 929, 945, 892, 962, 795, 684, 513, 954, 844, 849, 865, 910, 969, 743, 548, 872, 928, 946, 887, 955, 838, 830, 804, 716, 513, 953, 850, 867, 915, 966])

start_date = date(2019, 1, 1)
dweek = timedelta(weeks=1)

for i in range(32):
    print("%s & %s & %s & %s & %s & %s \\\\\\hline"
        % ((start_date+dweek*i).isoformat(), y[i], (start_date+dweek*(i+32)).isoformat(), y[i],
           (start_date+dweek*(i+64)).isoformat(), y[i]))


volume_per_week = lambda H: y*h
volume = lambda H: np.add.accumulate(volume_per_week(H))

def e(H):
    e = np.zeros(N)
    for i in range(N-1):
        e[i+1] = e[i] + h * g * y[i] * H
    return e/1000/3600

def de(H):
    de = np.zeros(N)
    de[0] = g * y[0] * H

    for i in range(N-1):
        de[i+1] = g * y[i] * H
        
    return de/1000/3600
    
def dem(H):
    dev = de(H)
    return np.array([sum(dev[4*i:4*i+4]) for i in range(N//4)])

fig1 = plt.figure(1)
ax1 = fig1.add_subplot(1,1,1)
p1, = ax1.plot(weeks, volume(H))
plt.title('Об\'єм води витраченої за період 2019-2020 роки')
plt.xlabel('номер неділі від 2019 року')
plt.ylabel('$м^3$')

fig6 = plt.figure(6)
ax6 = fig6.add_subplot(1,1,1)
p6, = ax6.plot(weeks, volume_per_week(H))
plt.title('Сток води')
plt.xlabel('номер неділі від 2019 року')
plt.ylabel('$м^3$')

fig2 = plt.figure(2)
ax2 = fig2.add_subplot(1,1,1)
p2, = ax2.plot(weeks, y)
plt.title('Витрати води')
plt.xlabel('номер неділі від 2019 року')
plt.ylabel('$м^3/c$')

plt.subplots_adjust(bottom=0.25)

axfreq = plt.axes([0.25, 0.1, 0.65, 0.03])
height_slider = plt.Slider(
    ax=axfreq,
    label='Висота греблі (м)',
    valmin=1,
    valmax=100,
    valstep=1,
    valinit=H,
)

fig3 = plt.figure(3)
ax3 = fig3.add_subplot(1,1,1)
p3, = ax3.plot(weeks, e(H))
plt.title('Кількість виробленої энергії від 2019 року')
plt.xlabel('номер неділі від 2019 року')
plt.ylabel('(кВт/час)')

fig4 = plt.figure(4)
ax4 = fig4.add_subplot(1,1,1)
p4, = ax4.plot(weeks, de(H))
plt.title('Кількість виробленої энергії у неділю')
plt.xlabel('номер неділі від 2019 року')
plt.ylabel('(кВт/час)')

fig5 = plt.figure(5)
ax5 = fig5.add_subplot(1,1,1)
p5, = ax5.plot(range(N//4), dem(H))
plt.title('Кількість виробленої энергії у місяць')
plt.xlabel('номер місяця від 2019 року')
plt.ylabel('(кВт/час)')


def update(val):
    H = height_slider.val
    ax1.clear()
    p1, = ax1.plot(weeks, volume(H))
    fig1.canvas.draw_idle()
    ax3.clear()
    p3, = ax3.plot(weeks, e(H))
    fig3.canvas.draw_idle()
    ax4.clear()
    p4, = ax4.plot(weeks, de(H))
    fig4.canvas.draw_idle()
    ax5.clear()
    p5, = ax5.plot(range(N//4), dem(H))
    fig5.canvas.draw_idle()


height_slider.on_changed(update)

plt.show()
