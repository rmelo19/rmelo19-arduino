#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

plt.close('all')
plt.ion()


plt.ylim([0, 1023]);


sequence = [np.random.randint(0, 1023) for i in range(0, 6)]
sequenceString = str(sequence)
recoveredSequence = sequenceString[1:-1].split(', ')
sensor = [0 for i in range(0, len(recoveredSequence))]
for i in range(0, len(recoveredSequence)):
	sensor[i] = int(recoveredSequence[i])
n = 0
plt.plot(n, sensor[0], 'ko', n, sensor[1], 'ro', n, sensor[2], "bo", n, sensor[3], "go", n, sensor[4], "co", n, sensor[5], "mo")
plt.legend(["IC1", "IC2", "IC3", "IC4", "IC5", "IC6"]);
plt.show()

while True:
	n = n + 1
	sequence = [np.random.randint(0, 1023) for i in range(0, 6)]
	sequenceString = str(sequence)
	recoveredSequence = sequenceString[1:-1].split(', ')
	sensor = [0 for i in range(0, len(recoveredSequence))]
	for i in range(0, len(recoveredSequence)):
		sensor[i] = int(recoveredSequence[i])
	
	plt.plot(n, sensor[0], 'ko', n, sensor[1], 'ro', n, sensor[2], "bo", n, sensor[3], "go", n, sensor[4], "co", n, sensor[5], "mo")
	plt.pause(0.05)