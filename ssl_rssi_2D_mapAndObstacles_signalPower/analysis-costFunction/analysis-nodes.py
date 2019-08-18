import math
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm

import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

print("hello")

# read file 1
data_obs = open("observations.txt")
lines = data_obs.readlines()
data_obs.close()

line1 = lines[0]
line1 = line1.strip()
line1 = line1.split()
NumberNodes = int(line1[3])
print(NumberNodes)

# read file 2
data_res = open("log_nodeResults.txt")
lines = data_res.readlines()
data_res.close()

xs = []
ys = []
for i in range(NumberNodes):
	xs.append([])
	ys.append([])

#IerationIDs = []
#NodeIDs = []
#nxs = []
#nys = []
#phases = []
counter = 0
for line in lines:
	line = line.strip()
	eles = line.split()
	print(eles)

	for i in range(NumberNodes):
		ID = i
		xID = 0 + 2*i
		yID = 1 + 2*i
		x = float(eles[xID])
		y = float(eles[yID])

		xs[ID].append(x)
		ys[ID].append(y)
	counter += 1

print(xs)
print()
print(ys)

plt.plot(xs[0], ys[0], marker='o', mec='r', mfc='w',label='Iterations')
plt.plot(xs[0][-1], ys[0][-1], marker='+', mec='r', mfc='w',label='Convergence Point')
plt.plot(-57.24, 2.58, marker='v', mec='r', mfc='w',label='Ground Truth')
plt.legend()
plt.xlabel('X')
plt.ylabel("Y")
plt.xlim(-65,-10)
plt.ylim(-20,20)
plt.title('Iteration Results')

plt.savefig('test.png', format='png', bbox_inches='tight', transparent=True, dpi=600)
plt.show()

