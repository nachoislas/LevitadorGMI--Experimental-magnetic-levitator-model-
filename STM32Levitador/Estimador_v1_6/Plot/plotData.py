import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('data.csv')
print(df.head())

fig, (ax1, ax2, ax3) = plt.subplots(3,1)
ax1.plot(df.muestras)
ax2.plot(df.derivadas)
ax3.plot(df.estimacion)
plt.show()