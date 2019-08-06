import math
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm

import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# main
print('Hello')

#
data = open("log_mapOfConstants.txt")
lines = data.readlines()
data.close()

line1 = lines[0]
line1 = line1.strip()
line1 = line1.split()
binx = int(line1[0])
biny = int(line1[3])
binz = int(line1[6])

xs = []
ys = []
zs = []
constants = []
for ID in range(1,binx*biny*binz+1):
	line = lines[ID]
	line = line.strip()
	eles = line.split()
	xs.append(float(eles[0]))
	ys.append(float(eles[1]))
	zs.append(float(eles[2]))
	constants.append(float(eles[3]))

df = pd.DataFrame({'x':xs,'y': ys,'Constant':constants})
pt = df.pivot_table(index='y', columns='x', values='Constant', aggfunc=np.sum)

sns.heatmap(pt, linewidths = 0.00)
plt.show()
