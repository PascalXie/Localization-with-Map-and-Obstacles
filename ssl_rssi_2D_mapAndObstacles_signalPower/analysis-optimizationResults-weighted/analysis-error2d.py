import math
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

# main
print('Hello')

#
data = open("log_distanceError2d_InftyWeight.txt")
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
pt1 = df.pivot_table(index='y', columns='x', values='Error', aggfunc=np.sum)

#
data = open("log_distanceError2d.txt")
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
pt2 = df.pivot_table(index='y', columns='x', values='Error', aggfunc=np.sum)

#
f, ax = plt.subplots(figsize = (4,8),nrows=2)
plt.subplots_adjust(left=0.15, bottom=0.10, right=0.99, top=0.99,wspace=None, hspace=0.3)

# color
cmap = sns.diverging_palette(200,20,sep=20,as_cmap=True)

# heatmap
sns_plot = sns.heatmap(pt1, linewidths = 0.00, ax = ax[0], cmap=cmap, xticklabels=8, yticklabels=8,annot=True)
sns_plot = sns.heatmap(pt2, linewidths = 0.00, ax = ax[1], cmap=cmap, xticklabels=8, yticklabels=8,annot=True)

# font
font = {'family': 'Times New Roman','weight': 'normal','size': 10,}

# axis settings
ax[0].set_xlabel('X / m',font)
ax[0].set_ylabel('Y / m',font)

ax[1].set_xlabel('X / m',font)
ax[1].set_ylabel('Y / m',font)

# text
left, width = .3, .5
bottom, height = -.16, .5
ax[0].text(left, bottom, r"(a) $P_{Th} \to -\infty$", fontsize=14,horizontalalignment='left',verticalalignment='top',transform=ax[0].transAxes)
ax[1].text(left, bottom, r"(a) $P_{Th} = 0 dBm$", fontsize=14,horizontalalignment='left',verticalalignment='top',transform=ax[1].transAxes)

plt.show()
