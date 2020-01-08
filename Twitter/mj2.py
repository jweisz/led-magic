#!/usr/bin/env python

import sys

from xbee import XBee
import serial

from twython import Twython


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

# TODO need to change these for TheWorldsHeart once we are able to create a new application for it...
API_KEY = 'nan115JZEGnKO8RKuGrtw'
API_SECRET = 'SKSVF7lT20YKAmIUds8gwyh6OQN4Rnep9tHs6doYE'
OAUTH_TOKEN = '44389014-xVYshiE5rxTRKcZWSTFYDZ1wdlbo2eEpeBHbNVqA8'
OAUTH_SECRET = 'IVR5ro95TnfdF8Qp9Sui72h9OZ6ZdaraUqChoaWVKo'
TWITTER = Twython(API_KEY, API_SECRET, OAUTH_TOKEN, OAUTH_SECRET)


def sendToXbee(xbee, color):
	# TODO place code to write to the xbee here!
	# xbee.send('at', frame='A', command='MY', parameter=None)
	return

def color2hex(str):
	lc = str.lower()
	if COLORS.has_key(lc):
		return COLORS[lc]
	return None

def findColors(str):
	# whitespace tokenizer
	tokens = str.split()
	# find color words
	colors = [t.lower() for t in tokens if t.lower() in set(COLORS.keys())]
	if len(colors) > 0:
		print "  finding color words in tweet [%s] -> [%s]" % (str, ", ".join(colors))
	else:
		print "  didn't find any color words in tweet [%s]" % (str)
	return colors

def getColorsFromTweets(tweets):
	allcolors = []
	for t in tweets:
		colors = findColors(t['text'])
		allcolors.extend(colors)
	return allcolors

def getColorsFromTwitterMentions():
	mentions = TWITTER.get_mentions_timeline()
	return getColorsFromTweets(mentions)

def getColorFromTwitterTimeline():
	timeline = TWITTER.get_home_timeline()
	return getColorsFromTweets(timeline)


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
			print "or try 'twitter-timeline' or 'twitter-mentions' to get a color from twitter"
			continue
		
		# twitter?
		if lc == "twitter-timeline":
			colorWords = getColorFromTwitterTimeline()
		elif lc == "twitter-mentions":
			colorWords = getColorsFromTwitterMentions()
		else:
			colorWords = [lc]
		
		# did we get anything
		if len(colorWords) == 0:
			print "didn't find any colors on twitter!"
			continue
		
		# pick only one color if more than one was found
		if len(colorWords) > 1:
			print "found %d colors: %s" % (", ".join(colorWords))
			print "using the first: %s" % (colorWords[0])
		
		colorWord = colorWords[0]
		
		# process the color
		color = color2hex(colorWord)
		if color != None:
			print "sending %s [%s] to the xbee..." % (line, color)
			sendToXbee(xbee, color)
		else:
			print "unknown color"
	
	if ser != None:
		print "disconnecting from xbee"
		ser.close()
	print "done!"
