#!/usr/bin/env python3

import cv2
import numpy as np

pmin, pmax, qmin, qmax = -2, 2, -2, 2
ppoints, qpoints = 500, 500
max_iterations = 10
infinity_border = 10

image = np.zeros((ppoints, qpoints), dtype="uint8")

pp = np.linspace(pmin, pmax, ppoints)
qq = np.linspace(qmin, qmax, qpoints)
print("hi")

for ip, p in enumerate(pp):
    for iq, q in enumerate(qq):
        z = 0
        c = p + 1j * q
        for k in range(max_iterations):
            z = z**2 + c
            if abs(z) > infinity_border:
                image[iq,ip] = k * 20
                break

cv2.imshow("img", image)

while True:
    k = cv2.waitKey(0) & 0xFF
    if k == ord("q"):
        break
