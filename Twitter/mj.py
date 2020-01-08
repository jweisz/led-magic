#!/usr/bin/env python

import sys

from xbee import XBee
import serial


# TODO we probably need to change how these colors are specified since I dont think the 
# xbee understands hex

COLORS = {
	'red':    '#FF0000',
	'green':  '#00FF00',
	'blue':   '#0000FF',
	'cyan':   '#00FFFF',
	'yellow': '#FFFF00',
	'magenta':'#FF00FF',
	'white':  '#FFFFFF',
	# woohoo it speaks farsi too!
	'ghermez': '#FF0000',
	'sabz':    '#00FF00',
	'abee':    '#0000FF',
	'zard':    '#FFFF00',
	'seefeed': '#FFFFFF',
}


def sendToXbee(xbee, color):
	# TODO place code to write to the xbee here!
	# xbee.send('at', frame='A', command='MY', parameter=None)
	return

def color2hex(str):
	lc = str.lower()
	if COLORS.has_key(lc):
		return COLORS[lc]
	return None

if __name__ == '__main__':
	# connect to xbee
	xbee = None
	ser = None
	
	print "connecting to xbee..."
	
	try:
		ser = serial.Serial('/dev/tty.usbserial-A901J8SO', 9600)
		xbee = XBee(ser)
	except OSError:
		print "warning: could not connect to xbee!"
	
	while True:
		sys.stdout.write("pick a color: ")
		try:
			line = sys.stdin.readline().strip()
		except KeyboardInterrupt:
			break
		if not line:
			break
		
		# quit?
		lc = line.lower()
		if lc == "none" or lc == "done" or lc == "quit":
			break
		
		# help?
		if lc == "help" or lc == "?":
			print "available colors: %s" % (', '.join(sorted(COLORS.keys())))
			continue
		
		# process the color
		color = color2hex(lc)
		if color != None:
			print "sending %s [%s] to the xbee..." % (line, color)
			sendToXbee(xbee, color)
		else:
			print "unknown color"
	
	if ser != None:
		print "disconnecting from xbee"
		ser.close()
	print "done!"
