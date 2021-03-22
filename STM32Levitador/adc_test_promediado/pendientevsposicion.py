import numpy as np
import matplotlib.pyplot as plt


N = 150
A = 25e-4
mu0 = 4 * 3.1415 * 1e-7
Linf = 8e-3
Vbus = 24

Ym = np.arange(1, 6, 1) * 1e-3		#posiciones en m
#inductancia_y = N ** 2 	* A * mu0 / (2 * Ym)		#incutancia en Hy
inductancia_y = np.array([3.49e-2, 2.33e-2, 1.84e-2, 1.63e-2, 1.44e-2, 1.33e-2])   #incutancia en Hy


pendientes = Vbus / inductancia_y #pendientes en A/s



Yestimada = mu0 * N ** 2 * A * pendientes / (2 * Vbus - 2 * Linf * pendientes)


print(pendientes)
plt.plot(pendientes, Yestimada)
plt.show()