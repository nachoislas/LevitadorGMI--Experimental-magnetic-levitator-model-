import csv
import numpy as np
import matplotlib.pyplot as plt

with open('Yestimada.txt', 'r') as txtFile:
	reader = csv.reader(txtFile)
	for row in reader:
		a = row


size = len(a)
Ts = 1/50000
Yestimada = np.zeros(size)
for i in range(0, size):
	Yestimada[i] = float(a[i]) / 1e3;

plt.plot(Yestimada)
plt.show()
