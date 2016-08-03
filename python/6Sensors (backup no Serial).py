#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

nS = 7;

# configuring and preparing plots
plt.close('all')
plt.ion()
plt.ylim([0, 1023]);

# constants
descriptorSequence = ['bo', 'go', 'ro', 'co', 'mo', 'yo', 'ko', 'wo', 'bv', 'gv', 'rv', 'cv', 'mv', 'yv', 'kv', 'wv']
colorSequence = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w', 'b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']


# getting number of sensors
sequence = [np.random.randint(0, 1023) for i in range(0, nS)]
sequenceString = str(sequence)
recoveredSequence = sequenceString[1:-1].split(', ')
numSensors = len(recoveredSequence)

# defining limits and variables
maxsensor = [0]*numSensors
minsensor = [1023]*numSensors
sensor = [0]*numSensors
prevSensor = [0]*numSensors

n = 0

while True:
	sequence = [np.random.randint(0, 1023) for i in range(0, nS)]  # read data string
	sequenceString = str(sequence)                                # convert data to string
	recoveredSequence = sequenceString[1:-1].split(', ')          # parse data string into number list
	for i in range(0, len(recoveredSequence)):
		sensor[i] = int(recoveredSequence[i])
		maxsensor[i] = max(maxsensor[i], sensor[i])
		minsensor[i] = min(minsensor[i], sensor[i])
		# plt.plot(n, sensor[i], descriptorSequence[i])
		plt.plot([n-1, n], [prevSensor[i], sensor[i]], colorSequence[i])
		prevSensor[i] = sensor[i]
	legRange = ["IC" + str(i+1) + ": " + str([minsensor[i], maxsensor[i]]) for i in range(0, len(recoveredSequence))]
	plt.legend(legRange);
	plt.pause(0.05)
	n = n + 1