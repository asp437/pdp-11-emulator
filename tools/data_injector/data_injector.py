import os
import sys

if (len(sys.argv) != 2):
    print("Usage: data_injector.py firmware.bin")
    exit()

firmware_file = open(sys.argv[1], 'br+')

font_file = open('util/font.bin', 'rb')
msg_file = open('util/msg.bin', 'rb')
logo_file = open('util/logo.bin', 'rb')

font_size = os.stat('util/font.bin').st_size
msg_size = os.stat('util/msg.bin').st_size
logo_size = os.stat('util/logo.bin').st_size

font_offset = 8 * 1024 - font_size
msg_offset = font_offset - msg_size
logo_offset = msg_offset - logo_size

print("Font offset: ", font_offset)
print("Msg offset: ", msg_offset)
print("Logo offset: ", logo_offset)

font_data = font_file.read()
msg_data = msg_file.read()
logo_data = logo_file.read()

firmware_file.seek(logo_offset)
firmware_file.write(logo_data)

firmware_file.seek(msg_offset)
firmware_file.write(msg_data)

firmware_file.seek(font_offset)
firmware_file.write(font_data)
