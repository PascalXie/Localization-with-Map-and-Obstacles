import math
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

# main
print('Hello')
plt.figure(figsize=(6,4))

data = open("observations.txt")
lines = data.readlines()
data.close()

line1 = lines[0]
line1 = line1.strip()
line1eles = line1.split()

print(line1eles)
NumberAnchors = int(line1eles[1])
NumberNodes = int(line1eles[3])

anchor_x = []
anchor_y = []
target_x = []
target_y = []

for anID in range(NumberAnchors):
	ID = anID*NumberNodes + 0 + 1
	line = lines[ID]
	line = line.strip()
	eles = line.split()
	anchor_x.append(float(eles[1]))
	anchor_y.append(float(eles[2]))

for taID in range(NumberNodes):
	ID = taID + 1
	line = lines[ID]
	line = line.strip()
	eles = line.split()
	target_x.append(float(eles[6]))
	target_y.append(float(eles[7]))

#
for taID in range(NumberNodes):
	filename = 'data/log_nodeResults_ManagerNode_'+str(taID)+'.txt'
	data = open(filename)
	lines = data.readlines()
	data.close()

	iter_x = []
	iter_y = []
	for line in lines:
		line = line.strip()
		eles = line.split()
		iter_x.append(float(eles[0]))
		iter_y.append(float(eles[1]))

	plt.plot(iter_x, iter_y, marker='o',markersize=4, color='k', linestyle='-',mec='m', mfc='w',label='Node'+str(taID))

	 
color = ['tab:blue', 'tab:orange', 'tab:green']
plt.scatter(anchor_x, anchor_y, marker='s',s=150,c=color[0],alpha=0.8,edgecolors='none',label='Anchor')
plt.scatter(target_x, target_y, marker='o',s=150,c=color[1],alpha=0.7,edgecolors='none',label='Target')
plt.legend(frameon=False)
plt.xlabel('X / m',fontdict={'family' : 'Times New Roman', 'size': 12})
plt.ylabel("Y / m",fontdict={'family' : 'Times New Roman', 'size': 12})
plt.xlim(-105,170)
plt.ylim(-110,110)
plt.title('Node and Iteration Distribution')
plt.savefig('figure-NodeDistribution.png',dpi=300)
plt.show()
