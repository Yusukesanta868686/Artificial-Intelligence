from matplotlib import pyplot as plt
import pandas as pd
from matplotlib.ticker import MaxNLocator

files1 = ["astar_result.csv", "bfs_result.csv", "dfs_result.csv"]
files2 = ["astar_result2.csv", "bfs_result2.csv", "dfs_result2.csv"]
files3 = ["astar_result3.csv", "bfs_result3.csv", "dfs_result3.csv"]

##################1######################
plt.figure()
for file in files1:
    data = pd.read_csv(file, usecols=[0], header = None)
    plt.plot(data, label = file)

plt.legend()
plt.xlabel("Number of couples")
plt.ylabel("Number of children")
ax = plt.gca()
ax.xaxis.set_major_locator(MaxNLocator(integer = True))
plt.savefig("one.png")

################2#########################
plt.figure()
for file in files2:
    data = pd.read_csv(file, usecols=[0], header = None)
    plt.plot(data, label = file)

plt.legend()
plt.xlabel("Number of couples")
plt.ylabel("Number of children")
ax = plt.gca()
ax.xaxis.set_major_locator(MaxNLocator(integer = True))
plt.savefig("two.png")



##################3#########################
plt.figure()
for file in files3:
    data = pd.read_csv(file, usecols=[0], header = None)
    plt.plot(data, label = file)

plt.legend()
plt.xlabel("Number of couples")
plt.ylabel("Number of children")
ax = plt.gca()
ax.xaxis.set_major_locator(MaxNLocator(integer = True))
plt.savefig("three.png")
plt.show()

