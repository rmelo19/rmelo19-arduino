#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import serial
import time

sr = open('workfile', 'r')
info = sr.read()
info = info.split('\n')[:-1]
history = list()
for element in info:
    parsedLine = element[element.find('[')+1:element.find(']')].split(',')
    sequence = [int(num) for num in parsedLine]
    history.append(sequence)

# constants
descriptorSequence = ['bo', 'go', 'ro', 'co', 'mo', 'yo', 'ko', 'wo', 'bv', 'gv', 'rv', 'cv', 'mv', 'yv', 'kv', 'wv']
colorSequence = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w', 'b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']


# getting number of sensors
numSensors = len(history[0])

# defining limits and variables
maxsensor = [0]*numSensors
minsensor = [1023]*numSensors
sensor = [0]*numSensors
prevSensor = [0]*numSensors


print "Number of sensors: " + str(numSensors)
print "Number of lines: " + str(len(history))
n = 0

# configuring and preparing plots
# plt.close('all')
# plt.ion()
# plt.ylim([0, 1023]);
# for recoveredSequence in history:
# 	for i in range(0, numSensors):
# 		sensor[i] = int(recoveredSequence[i])
# 		maxsensor[i] = max(maxsensor[i], sensor[i])
# 		minsensor[i] = min(minsensor[i], sensor[i])
# 		# plt.plot(n, sensor[i], descriptorSequence[i])
# 		plt.plot([n-1, n], [prevSensor[i], sensor[i]], colorSequence[i])
# 		prevSensor[i] = sensor[i]
# 	legRange = ["IC" + str(i+1) + ": " + str([minsensor[i], maxsensor[i]]) for i in range(0, len(recoveredSequence))]
# 	plt.legend(legRange)
# 	plt.pause(0.05)
# 	n = n + 1

plt.close('all')
plt.ylim([0, 1023])
plt.plot(history)
plt.show()

sr.close()
