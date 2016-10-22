import os
import sys

if (len(sys.argv) != 2):
	print("Usage: font_injector.py firmware.bin")
	exit()


firmware_file = open(sys.argv[1], 'br+')
font_file = open('util/font.bin', 'rb')

font_size = os.stat('font.bin').st_size
font_offset = 8 * 1024 - font_size

print("Font offset: ", font_offset)

firmware_file.seek(font_offset)
font_data = font_file.read()

firmware_file.write(font_data)
