#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import odeint
from nolitsa import data, delay, noise, dimension

tEnd = 10
tSize = 100000
dSize = 1000
startPoint = [0.1, 0.1, 0.1]
epsillon = 0.1


def localmin(x):
    return (np.diff(np.sign(np.diff(x))) > 0).nonzero()[0] + 1


def sys(X, t=0) -> np.ndarray:
    y1, y2, y3 = X
    return np.array([
        # ~ 1000-3*x-1000*y**2+10*z**2,
        # ~ y+2*z+x*(y+z*4/3),
        # ~ -2*y+z+x*(-y*4/3+z)
        # ~ 3*x+x*(x-3)*(5*y**2-z**2)/(1+y**2+z**2),
        # ~ 2*y-14*z-5*(x-3)*y,
        # ~ 14*y+2*z+5*(x-3)*z
        # ~ -3*x+140*y**2-z**2,
        # ~ y-200*z-140*x*y,
        # ~ 200*y+z+x*z
        2*y1-20*y3+3*y1**2-2*y2**2-y3**2-2*y2*y3-2*y1*y3,
        -0.5*y2+4*y2**2+8*y1*y2+4*y2*y3+4*y1*y3,
        20*y1+2*y3+4*y1*y3+2*y2*y3+y3**2
    ])


def draw_attractor() -> None:
    tspan = np.linspace(0, tEnd, dSize)
    ys = odeint(sys, startPoint, tspan, rtol=0.0000000001, atol=0.0000000001)
    xx, yy = np.meshgrid(ys[:,0], ys[:,0])
    I = (abs(xx - yy) - epsillon) > 0
    
    tspan = np.linspace(0, tEnd, tSize)
    yso = ys
    ys = odeint(sys, startPoint, tspan)
    
    lag = np.arange(250)
    x = ys[:,0]
    r = delay.acorr(x, maxtau=250)
    i = delay.dmi(x, maxtau=250)

    i_delay = localmin(noise.sma(i, hwin=1)) + 1
    r_delay = np.argmax(r < 1.0 / np.e)
    
    print(r'Minima of delayed mutual information = %s' % i_delay)
    print(r'Autocorrelation time = %d' % r_delay)
    
    dim = np.arange(1, 15 + 1)
    tau_here = (localmin(noise.sma(delay.dmi(x, maxtau=250), hwin=1)) + 1)[0]
    tau_here = np.argmax(delay.acorr(yso[:,0], maxtau=250) < 1.0 / np.e)
    f = dimension.fnn(yso[:,0], tau=tau_here, dim=dim, window=0, metric='euclidean')

    fig = plt.figure(1)
    ax = plt.axes(projection='3d')
    ax.plot3D(ys[:,0], ys[:,1], ys[:,2], alpha=0.75)
    plt.figure(2)
    plt.imshow(I, cmap=plt.cm.gray, origin='lower')
    plt.figure(3)
    plt.subplot(211)
    plt.plot(tspan, ys[:,0])
    plt.subplot(212)
    plt.plot(tspan, ys[:,1])
    plt.figure(4)
    plt.subplot(121)
    plt.plot(ys[:,0], ys[:,1])
    plt.subplot(122)
    plt.plot(ys[:,0], ys[:,2])
    plt.figure(5)
    # ~ plt.subplot(121)
    # ~ plt.title(r'Time delay = %d' % r_delay)
    # ~ plt.xlabel(r'$x(t)$')
    # ~ plt.ylabel(r'$x(t + \tau)$')
    # ~ plt.plot(ys[:-r_delay,0], ys[r_delay:,0])
    # ~ plt.subplot(122)
    plt.title(r'Time delay = %d' % i_delay[0])
    plt.xlabel(r'$x(t)$')
    plt.ylabel(r'$x(t + \tau)$')
    plt.plot(ys[:-i_delay[0],0], ys[i_delay[0]:,0])
    plt.figure(6)
    plt.ylabel(r'Delayed mutual information')
    plt.plot(lag, i, i_delay, i[i_delay], 'o')
    plt.figure(7)
    plt.plot(dim, f[0], dim, f[1], dim, f[2])
    plt.xlabel(r'Embedding dimension $d$')
    plt.ylabel(r'FNN (%)')
    plt.figure(8)
    ax = plt.axes(projection='3d')
    ax.plot3D(ys[:-2*i_delay[0],0], ys[i_delay[0]:-i_delay[0],0], ys[2*i_delay[0]:,0], alpha=0.75)
    print(ys[-1])
    plt.show()


draw_attractor()
