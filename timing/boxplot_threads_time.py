import pandas as pd
import matplotlib.pyplot as plt

fig1 = plt.figure()
df = pd.read_csv("threads_time.csv")


plt.title('Performance average test')
plt.xlabel('Threads')
plt.ylabel('Time')

df.boxplot()

plt.savefig("boxplot_threads_time.png")

plt.show()
plt.close()
