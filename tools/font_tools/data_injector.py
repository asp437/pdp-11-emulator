import os
import sys

if (len(sys.argv) != 2):
    print("Usage: font_injector.py firmware.bin")
    exit()

firmware_file = open(sys.argv[1], 'br+')
font_file = open('util/font.bin', 'rb')
msg_file = open('util/msg.bin', 'rb')

font_size = os.stat('util/font.bin').st_size
msg_size = os.stat('util/msg.bin').st_size

font_offset = 8 * 1024 - font_size
msg_offset = font_offset - msg_size

print("Font offset: ", font_offset)
print("Msg offset: ", msg_offset)

font_data = font_file.read()
msg_data = msg_file.read()

firmware_file.seek(msg_offset)
firmware_file.write(msg_data)

firmware_file.seek(font_offset)
firmware_file.write(font_data)
