import random

fat = random.randint(0, 255)
car = random.randint(0, 255)
d_fat = {1 : 1, 2 : 1, 3 : 1, 6 : 1, 16 : 1, 18 : 1, 20 : 1}
d_car = {8 : 1, 9 : 1, 10 : 1, 15 : 1, 17 : 1, 19 : 1}

with open('first.log', 'w') as f_output:
	for i in range(21):
		if d_fat.get(i) != None:
			state = (random.randint(0, 255) << 8) | fat
			f_output.write(str(state) + '\n')
		elif d_car.get(i) != None:
			state = (random.randint(0, 255) << 8) | car
			f_output.write(str(state) + '\n')
		else:	
			state = (random.randint(0, 255) << 8) | 0
			f_output.write(str(state) + '\n')

with open('second.log', 'w') as f_output:
	for i in range(21):
		if d_fat.get(i) != None:
			state = (random.randint(0, 255) << 8) | fat
			f_output.write(str(state) + '\n')
		elif d_car.get(i) != None:
			state = (random.randint(0, 255) << 8) | car
			f_output.write(str(state) + '\n')
		else:	
			state = (random.randint(0, 255) << 8) | 0
			f_output.write(str(state) + '\n')
