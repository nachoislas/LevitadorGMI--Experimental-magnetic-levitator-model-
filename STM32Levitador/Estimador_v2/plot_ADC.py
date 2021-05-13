import csv
import numpy as np
import matplotlib.pyplot as plt

list = []
with open('muestras_corriente.txt', 'r') as txtFile:
	list = txtFile.readline().split(',')

listADC = []
with open('muestrasADC.txt', 'r') as txtFile:
	listADC = txtFile.readline().split(',')
	
	
size = len(list)
sizeADC = len(listADC)
muestras = np.zeros(size)
muestrasADC = np.zeros(sizeADC)

for index,value in enumerate(list):
	muestras[index] = float(value)

for index,value in enumerate(listADC):
	muestrasADC[index] = float(value) / 1e4	

plt.plot(muestrasADC)
plt.show()

times = 4
repeatArray = np.zeros(times * size)
for index in range(0, times*size, size):
	repeatArray[index:index+size] = muestras

plt.plot(repeatArray)
plt.show()