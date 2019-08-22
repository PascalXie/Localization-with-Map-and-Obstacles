import math
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

# main
print('Hello')

data = open("data/observations.txt")
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

# log_anchorDensity
data = open("log_anchorDensity.txt")
lines = data.readlines()
data.close()

distance = []
counts = []
for line in lines:
	line = line.strip()
	eles = line.split()
	distance.append(float(eles[0]))
	counts.append(float(eles[1]))
print(distance)
print(counts)


# plot
fig = plt.figure(figsize=(4,3))
color = ['tab:blue', 'tab:orange', 'tab:green']
plt.scatter(anchor_x, anchor_y, marker='s',s=150,c=color[0],alpha=0.8,edgecolors='none',label='Anchor')
plt.legend(frameon=True)
plt.xlabel('X / m',fontdict={'family' : 'Times New Roman', 'size': 12})
plt.ylabel("Y / m",fontdict={'family' : 'Times New Roman', 'size': 12})
plt.xlim(-105,170)
plt.ylim(-110,110)
plt.title('Anchor Distribution')
plt.savefig('figure-AnchorDistribution.png',dpi=300)

# font
font = {'family': 'Times New Roman','weight': 'normal','size': 12,}
# plot : log_anchorDensity
fig = plt.figure(figsize=(4,3))
ax = plt.subplot(111)
plt.subplots_adjust(left=0.25, bottom=0.20, right=0.95, top=None,wspace=None, hspace=None)
ax.plot(distance, counts, marker='o',markersize=4, color='k', linestyle='-',mec='k', mfc='w')
ax.set_xlabel(r'$d_{Ti,c}$ / m', font)
ax.set_ylabel(r'$\rho_{ring}$ / $m^{-2}$', font)
plt.savefig('figure-anchorDensity.png',dpi=300)

plt.show()
