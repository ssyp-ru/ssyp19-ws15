while True:
	data = input()
	if (data[0] == '='):
		print(data)
	else:
		data = int(data)
		print(str((data & 0xFF00) >> 8) + ' ' + str(data & 0x00FF))
