import matplotlib.pyplot as plt
import numpy as np

samplingFreq = 50000
Ts = 1/samplingFreq
length = 16
time = np.arange(0, length, 1) 

def triangle(length, amplitude):
     section = length // 4
     for direction in (1, -1):
         for i in range(section):
             yield i * (amplitude / section) * direction
         for i in range(section):
             yield (amplitude - (i * (amplitude / section))) * direction

def triangle_v2(time, Ts):
	section = length / 2
	triang = np.zeros(length)
	m = 1
	amplitude = 5
	direction = 1
	for i in range(0, length):
		if direction is 1:
			triang[i] = m * time[i] * amplitude
		elif direction is -1:
			triang[i] =	amplitude * m * (1 - time[i]) + 75
		if i == section:
			direction *= -1
	return triang
			
			

ciclo = triangle_v2(time, Ts)
#ciclo = np.array(ciclo) + 2.5

señal = np.concatenate([ciclo, ciclo, ciclo, ciclo, ciclo])
print(ciclo)
def writeFile():
	with open("Core/Inc/triangular.h", 'w') as file:
		file.write('float triangular[] = {')
		for index in range(0, len(señal)):
			if index == len(señal) - 1:
				file.write(str(señal[index]))
			else:
				file.write(str(señal[index]) + ', ')
		file.write('};')
		
writeFile()

# plt.plot(señal)

# plt.show()

def derivar(actual, anterior, Ts):
	return (actual - anterior) / Ts
	
derivadas = []
for i in range(len(señal) - 1):
	derivadas.append(derivar(señal[i+1], señal[i], 1))


# plt.plot(derivadas)
# plt.show()	









