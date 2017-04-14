
import sys
import math
from graphs import *

xPosPerc = .7
xNegPerc = .5
XPosThreshold = 14
XNegThreshold = -14
XConst = 0

yPosPerc = .8
yNegPerc = .2
YPosThreshold = 10
YNegThreshold = -10
YConst = 1

commandsX = list()
commandsY = list()

undesirableX = list()
undesirableY = list()

middleX = list()
middleY = list()


def main():
	d = int(sys.argv[1]) # +- derivitive range
	b = int(sys.argv[2]) # +- bias range
	try:
		openFilterInputFile(str(sys.argv[3])) # Input params file name
	except:
		pass
	for i in range(d):
		for n in range(b):
			# Desirable
			processXDerivative(i, n) # Moving right, pos right
			processXDerivative(-i, -n) # Moving left, pos left
			processYDerivative(i, n) # Moving up, pos up
			processYDerivative(-i, -n) # Moving down, pos down
			# Undesirable
			processXDerivative(i, -n, False) # Moving right, left pos
			processXDerivative(-i, n, False) # Moving left, right pos
			processYDerivative(i, -n, False) # Moving up, pos down
			processYDerivative(-i, n, False) # Moving down, pos up

	display()


def openFilterInputFile(filename):
	global XPosThreshold, XNegThreshold, xPosPerc, xNegPerc, XConst, YPosThreshold, YNegThreshold, yPosPerc, yNegPerc, YConst
	try:
		with open(filename, 'r') as file:
			for line in file:
				line = line.strip().split()
				if (str(line[0]) == 'xPT'):
					XPosThreshold = float(line[1])
				elif (str(line[0]) == 'xNT'):
					XNegThreshold = float(line[1])
				elif (str(line[0]) == 'xP'):
					xPosPerc = float(line[1])
				elif (str(line[0]) == 'xN'):
					xNegPerc = float(line[1])
				elif (str(line[0]) == 'xC'):
					XConst = float(line[1])
				elif (str(line[0]) == 'yPT'):
					YPosThreshold = float(line[1])
				elif (str(line[0]) == 'yNT'):
					YNegThreshold = float(line[1])
				elif (str(line[0]) == 'yP'):
					yPosPerc = float(line[1])
				elif (str(line[0]) == 'yN'):
					yNegPerc = float(line[1])
				elif (str(line[0]) == 'yC'):
					YConst = float(line[1])
		if (XNegThreshold > 0):
			XNegThreshold *= -1
		if (YNegThreshold > 0):
			YNegThreshold *= -1
	except:
		exit()


def processXDerivative(dx, xBias, desirable=True):
	global xPosPerc, xNegPerc, XPosThreshold, XNegThreshold, commandsX, XConst, undesirableX, middleX
	# Get Bias
	if (xBias > 0):
		xBias *= xPosPerc
	elif (xBias < 0):
		xBias *= xNegPerc

	# Calculate F
	f = dx + xBias + XConst

	# Append accordingly
	rOrL = 2 # 1 = right, 0 = left
	if (f > XPosThreshold):
		rOrL = 1
	elif (f < XNegThreshold):
		rOrL = 0

	# Error Correction NOTE: TAKES OUT ALL GOOD POINTS WHEN xBias == 0 -- This is the only bad part
	if ((f > XPosThreshold - 10 or f < XNegThreshold + 10) and (dx*xBias < 0 or xBias == 0)):
		f = 0

	# Graph
	if (desirable and rOrL < 2):
		commandsX.append(StampX(rOrL, f, desirable, dx, xBias)) # Desirable and sending commands
	elif (not desirable):
		undesirableX.append(StampX(rOrL, f, desirable, dx, xBias)) # Not desirable commands (Should all be in the middle -- aim)
	else:
		middleX.append(StampX(rOrL, f, desirable, dx, xBias)) # Desirable but in the middle of thresh

def processYDerivative(dy, yBias, desirable=True):
	global yPosPerc, yNegPerc, YPosThreshold, YNegThreshold, commandsY, YConst, undesirableY, middleY
	# Get Bias
	if (yBias > 0):
		yBias *= yPosPerc
	elif (yBias < 0):
		yBias *= yNegPerc

	# Calculate F
	f = dy + yBias + YConst

	# Append accordingly
	uOrD = 2 # 1 = up, 0 = down
	if (f > YPosThreshold):
		uOrD = 1
	elif (f < YNegThreshold):
		uOrD = 0

	# Error Correction NOTE: TAKES OUT ALL GOOD POINTS WHEN yBias == 0 -- This is the only bad part
	if ((f > YPosThreshold - 10 or f < YNegThreshold + 10) and (dy*yBias < 0 or yBias == 0)):
		f = 0

	# Graph
	if (desirable and uOrD < 2):
		commandsY.append(StampY(uOrD, f, desirable, dy, yBias)) # Desirable and sending commands
	elif (not desirable):
		undesirableY.append(StampY(uOrD, f, desirable, dy, yBias)) # Not desirable commands (Should all be in the middle -- aim)
	else:
		middleY.append(StampY(uOrD, f, desirable, dy, yBias)) # Desirable but in the middle of thresh


def display():
	global commandsX, commandsY, YPosThreshold, YNegThreshold, XPosThreshold, XNegThreshold, undesirableY, undesirableX, middleX, middleY
	graphFvsDX(commandsX, undesirableX, middleX, XPosThreshold, XNegThreshold)
	graphFvsDY(commandsY, undesirableY, middleY, YPosThreshold, YNegThreshold)
	graphFvsXbias(commandsX, undesirableX, middleX, XPosThreshold, XNegThreshold)
	graphFvsYbias(commandsY, undesirableY, middleY, YPosThreshold, YNegThreshold)
	graphXCommands(commandsX)
	graphYCommands(commandsY)
	graphPercentages(commandsX, commandsY, undesirableX, undesirableY, middleX, middleY)


class Stamp(object):
	def __init__(self, command, f, desirable):
		self.command = command
		self.f = f
		self.desirable = desirable

	def __repr__(self):
		return str(self.f)


class StampX(Stamp):
	def __init__(self, command, f, desirable, dx, xBias):
		super(StampX, self).__init__(command, f, desirable)
		self.dx = dx
		self.xBias = xBias


class StampY(Stamp):
	def __init__(self, command, f, desirable, dy, yBias):
		super(StampY, self).__init__(command, f, desirable)
		self.dy = dy
		self.yBias = yBias

if __name__ == '__main__':
	main()