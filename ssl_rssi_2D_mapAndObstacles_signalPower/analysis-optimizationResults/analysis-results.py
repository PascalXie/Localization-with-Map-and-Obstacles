import math
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

# main
print('Hello')

data = open("data/log_distanceError2d.txt")
lines = data.readlines()
data.close()

x = []
y = []
distance = []
for line in lines:
	line = line.strip()
	eles = line.split()
	x.append(float(eles[0]))
	y.append(float(eles[1]))
	distance.append(float(eles[2]))

df = pd.DataFrame({'x':x,'y': y,'Error':distance})
pt = df.pivot_table(index='y', columns='x', values='Error', aggfunc=np.sum)

f, (ax1) = plt.subplots(figsize = (9,8),nrows=1)
cmap = sns.diverging_palette(200,20,sep=20,as_cmap=True)
sns_plot = sns.heatmap(pt, linewidths = 0.00, ax = ax1, cmap=cmap, xticklabels=8, yticklabels=8)

ax1.set_title('Distance Error Distribution')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
plt.show()
