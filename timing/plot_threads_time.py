#!/usr/bin/env python3

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

fig1 = plt.figure()
df = pd.read_csv("threads_time.csv")
arr = df.values

#compute means for all threads
means = []
stdev = []
index = []
i = 1
for column in arr.T:
    means.append(np.mean(column))
    stdev.append(np.std(column))
    index.append(i)
    i = i + 1

#graph setup
plt.title('Performance test')
plt.xlabel('Threads')
plt.ylabel('Time')
plt.xlim(1, len(means))
plt.grid(True)
plt.xticks(range(0,len(means) + 2))
plt.yticks(range(0,30))
plt.plot(index, means, label='mean')
plt.plot(index, stdev, label='standard deviation')

plt.savefig("plot_threads_time.png")

plt.legend()
plt.show()

plt.close()
