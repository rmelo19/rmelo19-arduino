#!/usr/bin/env python

"""Examples illustrating the use of plt.subplots().

This function creates a figure and a grid of subplots with a single call, while
providing reasonable control over how the individual plots are created.  For
very refined tuning of subplot creation, you can still use add_subplot()
directly on a new figure.
"""

# import matplotlib.pyplot as plt
# import numpy as np
import time;

# # Simple data to display in various forms
# # x = np.linspace(0, 2 * np.pi, 400)
# # y = np.sin(x ** 2)

# x = 1;
# y = 2*x;

# # fig=plt.figure()
# plt.axis([0,1000,0,1])
# plt.ion()

# f, ax = plt.subplots()
# plt.close('all')
# ax.set_title('Simple plot')

# plt.show();
# while True:
# 	# Just a figure and one subplot
# 	ax.plot(x, y)
# 	x = x + 1
# 	y = 2*x

# 	plt.draw()

# 	plt.pause(0.05)

import numpy as np
import matplotlib.pyplot as plt


plt.close('all')
plt.axis([0, 100, 0, 1]) # [xmin, xmax, ymin, ymax]
plt.ion() # interactive plotting

 # plt.scatter(x, y) # type of graph
 #    x += 1
 #    y = 2*x
 #    # plt.pause(0.05)
 #    time.sleep(0.05)

# for i in range(100):
#     plt.pause(0.01)

i = 0
while True:
	i = i + 1
	plt.scatter(i, 2*i)
	plt.pause(0.05)
