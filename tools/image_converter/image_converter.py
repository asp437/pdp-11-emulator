from PIL import Image

im = Image.open("nyan_bw_big.png")
pix = im.load()
out_file = open('logo.bin', 'wb')

for y in range(0, 50):
    for x in range(0, 100):
        code = 0
        for x_s in range(0, 4):
            if (pix[4 * x + x_s, y] == (0, 0, 0)):
                code = code | 0x0
            elif (pix[4 * x + x_s, y] == (0x40, 0x40, 0x40)):
                code = code | 0x100
            elif (pix[4 * x + x_s, y] == (0x80, 0x80, 0x80)):
                code = code | 0x200
            elif (pix[4 * x + x_s, y] == (0xFF, 0xFF, 0xFF)):
                code = code | 0x300
            code = code >> 2

        s_byte = code.to_bytes(1, byteorder='big', signed=False)
        out_file.write(s_byte)

# 000000 404040 808080 FFFFFF

# for y_s in range(0, 4):
# 	for x_s in range(0, 26):
# 		for y in range(0, 8):
# 			for x in range(0, 2):
# 				code = 0
# 				if (pix[x_s * 8 + x*4, y_s * 8 + y] == (0, 255, 0)):
# 					code = code | 0x3
# 				if (pix[x_s * 8 + x*4 + 1, y_s * 8 + y] == (0, 255, 0)):
# 					code = code | 0xC
# 				if (pix[x_s * 8 + x*4 + 2, y_s * 8 + y] == (0, 255, 0)):
# 					code = code | 0x30
# 				if (pix[x_s * 8 + x*4 + 3, y_s * 8 + y] == (0, 255, 0)):
# 					code = code | 0xC0
# 				s_byte = code.to_bytes(1, byteorder='big', signed=False)
# 				out_file.write(s_byte)
