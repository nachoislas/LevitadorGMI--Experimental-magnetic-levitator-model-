import numpy as np
import matplotlib.pyplot as plt
import csv


fs = 50000
Ts = 1/fs
muestras = np.genfromtxt('muestras.csv', delimiter=',') * 3.3 / 4095
N = len(muestras)
#print(muestras)



def derivar(xn, xn_1):
	return (xn - xn_1) / Ts

def filtrar(array, index, size):
	arr2 = array[index + size : index : -1]
	#print(arr2)
	sum = 0
	for indice in range(0, len(arr2)):
		sum += arr2[indice]
	return sum / size


derivada = np.zeros(N)
rectif = np.zeros(N)
sentido = 1
Yfiltrada = np.zeros(N)

for index in range(1, N):
	derivada[index - 1] = derivar(muestras[index], muestras[index - 1])
	if derivada[index - 1] < 0:
		sentido = -1
	else:
		sentido = 1
	rectif[index - 1] = derivada[index - 1] * sentido

for index in range(0, N):
	Yfiltrada[index] = filtrar(rectif, index, 50) 
	
pendiente_filt = 0
for index in range(0, N):
	pendiente_filt += rectif[index] / N
print(pendiente_filt)

	
#print(derivada)
ax1 = plt.subplot(3, 1, 1)
ax1.plot(muestras)
ax2 = plt.subplot(312)
ax2.plot(rectif)
ax3 = plt.subplot(313)
ax3.plot(Yfiltrada)
plt.show()


