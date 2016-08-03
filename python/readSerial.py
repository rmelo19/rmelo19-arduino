#!/usr/bin/env python

import serial

sr = serial.Serial('/dev/ttyACM0', 115200)

currentLine = sr.readline()
while currentLine.find('PRESSURES: ')  == -1:
	currentLine = sr.readline()

while True:
	currentLine = sr.readline()	
	parsedLine = currentLine[currentLine.find('[')+1:currentLine.find(']')].split(',')
	intParsedLine = [int(element) for element in parsedLine]

	for element in intParsedLine:
		print element 

sr.close()