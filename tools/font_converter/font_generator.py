from PIL import Image

im = Image.open("font.png")
pix = im.load()
out_file = open('font.bin', 'wb')

for y_s in range(0, 4):
	for x_s in range(0, 26):
		for y in range(0, 8):
			for x in range(0, 2):
				code = 0
				if (pix[x_s * 8 + x*4, y_s * 8 + y] == (0, 255, 0)):
					code = code | 0x3
				if (pix[x_s * 8 + x*4 + 1, y_s * 8 + y] == (0, 255, 0)):
					code = code | 0xC
				if (pix[x_s * 8 + x*4 + 2, y_s * 8 + y] == (0, 255, 0)):
					code = code | 0x30
				if (pix[x_s * 8 + x*4 + 3, y_s * 8 + y] == (0, 255, 0)):
					code = code | 0xC0
				s_byte = code.to_bytes(1, byteorder='big', signed=False)
				out_file.write(s_byte)
