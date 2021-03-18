import numpy as np
import matplotlib.pyplot as plt

pendientes = np.arange(500, 1000, 50)   #pendientes en A/s

N = 150
A = 25e-4
mu0 = 4 * 3.1415 * 1e-7
Linf = 12e-3
Vbus = 24

Yestimada = mu0 * N ** 2 * A * pendientes / (2 * Vbus - 2 * Linf * pendientes)

plt.plot(pendientes, Yestimada)
plt.show()