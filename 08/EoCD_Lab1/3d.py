#!/usr/bin/env python3

import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl

print("ready")
transforms = [
    [
        [1,0,0,0],
        [0,1,0,0],
        [0,0,0.3,0],
        [0,0,0,1],
    ],
    # ~ [
        # ~ [1,0,0,0],
        # ~ [0,1,0,0],
        # ~ [0,0,0.3,0.7],
        # ~ [0,0,0,1],
    # ~ ],
    [
        [1,0,0,0],
        [0,0.3,0,0],
        [0,0,1,0],
        [0,0,0,1],
    ],
    # ~ [
        # ~ [1,0,0,0],
        # ~ [0,0.3,0,0.7],
        # ~ [0,0,1,0],
        # ~ [0,0,0,1],
    # ~ ],
    [
        [0.3,0,0,0],
        [0,1,0,0],
        [0,0,1,0],
        [0,0,0,1],
    ],
    # ~ [
        # ~ [0.3,0,0,0.7],
        # ~ [0,1,0,0],
        # ~ [0,0,1,0],
        # ~ [0,0,0,1],
    # ~ ]
]
transforms = [np.array(i,dtype=np.float32).transpose() for i in transforms]
ts_sz = len(transforms)
pts=np.array([
    [0,0,0,1],[0,0,1,1],[0,0,0,1],[0,1,0,1],[0,0,0,1],[1,0,0,1],
    [1,0,0,1],[1,1,0,1],[1,0,0,1],[1,0,1,1],
    [0,1,0,1],[1,1,0,1],[0,1,0,1],[0,1,1,1],
    [0,0,1,1],[1,0,1,1],[0,0,1,1],[0,1,1,1],
    [1,1,0,1],[1,1,1,1],
    [1,0,1,1],[1,1,1,1],
    [0,1,1,1],[1,1,1,1],
], dtype=np.float32)
for i in range(1):
    sp_sz = pts.shape[0]
    new_pts = np.zeros((sp_sz*ts_sz,4))
    for i in range(ts_sz):
        new_pts[i*sp_sz:(i+1)*sp_sz] = pts.dot(transforms[i])
    pts = new_pts

print("readed")
print(pts)

# ~ color = np.copy(pts)
# ~ color[:,:3] *= 0.6
# ~ color[:,3] = 0.5
color = (0.2, 0.8, 0.2, 0.3)

app = pg.mkQApp("GLScatterPlotItem Example")
w = gl.GLViewWidget()
w.opts['distance'] = 10
w.show()
w.setWindowTitle('pyqtgraph example: GLScatterPlotItem')
#sp1 = gl.GLLinePlotItem(pos=pts[:,:3], width=0.5, color=(0.2, 0.8, 0.2, 0.3), mode="lines")
sp1 = gl.GLLinePlotItem(pos=pts[:,:3], width=0.5, color=color, mode="lines")
#sp1 = gl.GLScatterPlotItem(pos=pts, size=2, color=(0.2, 0.8, 0.2, 0.8))
#sp1.translate(0.930293062763, 0.902966360950, 0)
w.addItem(sp1)

QtGui.QApplication.instance().exec_()
