import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

fig1 = plt.figure()
df = pd.read_csv("threads_time.csv")

maxY = df.max().max()

plt.title('Performance test')
plt.xlabel('Threads')
plt.ylabel('Time')
plt.yticks(range(0, maxY.astype(np.int64) + 2))


df.boxplot()

plt.savefig("boxplot_threads_time.png")

plt.show()
plt.close()
