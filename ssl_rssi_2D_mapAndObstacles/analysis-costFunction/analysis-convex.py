import math
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm

import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# main
print('Hello')
print(math.log(2.7))

#
data = open("log_costFunction.txt")
lines = data.readlines()
data.close()

xs = []
ys = []
values = []
values_log = []

for line in lines:
	line = line.strip()
	eles = line.split()
	xs.append(float(eles[0]))
	ys.append(float(eles[1]))

	value = float(eles[2])
	values.append(value)

	value_log = -1.*math.log(value)
	values_log.append(value_log)



df = pd.DataFrame({'y':ys,'x': xs,'CostFunctionValues':values, 'logValue':values_log})
pt = df.pivot_table(index='y', columns='x', values='logValue', aggfunc=np.sum)

f, (ax1) = plt.subplots(figsize = (9,8),nrows=1)

cmap = sns.diverging_palette(200,20,sep=20,as_cmap=True)
#sns.heatmap(pt, linewidths = 0.00, ax = ax1, cmap=cmap)
sns_plot = sns.heatmap(pt, linewidths = 0.00, ax = ax1, cmap="RdPu", xticklabels=8, yticklabels=8)
sns_plot.tick_params(labelsize=10)

ax1.set_title('Cost Function Distribution')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')

plt.show()
#
