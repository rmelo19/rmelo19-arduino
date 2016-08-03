#!/usr/bin/env python

# import numpy as np
import matplotlib.pyplot as plt

plt.close('all')
plt.ion()
# v = [0, 100000, 0, 1023];
# plt.axis(v)
x = 1
y = 2*x

plt.ylim([0, 1023]);

plt.plot(x, y, 'ko')
plt.show()

while True:
	x = x + 1
	y = 2*x
	plt.plot(x, y, 'ko')
	plt.pause(0.05)