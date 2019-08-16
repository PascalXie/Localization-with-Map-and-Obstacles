import math
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm

import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# main
print('Hello')

# brick wall
data = open("log_AttenuationAndPathLossExponent_brick.txt")
lines = data.readlines()
data.close()

Brick_distances = []
Brick_strengths_att = []
Brick_strengths_ple = []

for line in lines:
	line = line.strip()
	eles = line.split()
	Brick_distances.append(float(eles[0]))
	Brick_strengths_att.append(float(eles[1]))
	Brick_strengths_ple.append(float(eles[2]))

# air
data = open("log_AttenuationAndPathLossExponent_air.txt")
lines = data.readlines()
data.close()

Free_distances = []
Free_strengths_att = []
Free_strengths_ple = []

for line in lines:
	line = line.strip()
	eles = line.split()
	Free_distances.append(float(eles[0]))
	Free_strengths_att.append(float(eles[1]))
	Free_strengths_ple.append(float(eles[2]))

#
plt.figure(figsize=(6,4))
plt.plot(Brick_distances, Brick_strengths_att, marker='o',markersize=8, color='m', linestyle='-',mec='m', mfc='w',label='Attenuation-Brick Wall')
plt.plot(Brick_distances, Brick_strengths_ple, marker='+',markersize=8, color='m', linestyle='-',mec='m', mfc='w',label='Path Loss Exponent-Brick Wall')
plt.plot(Free_distances, Free_strengths_att,   marker='o',markersize=8, color='k', linestyle='-',mec='k', mfc='w',label='Attenuation-Free')
plt.plot(Free_distances, Free_strengths_ple,   marker='+',markersize=8, color='k', linestyle='-',mec='k', mfc='w',label='Path Loss Exponent-Free')
plt.legend()
plt.xlabel('Distance',fontdict={'family' : 'Times New Roman', 'size': 12})
plt.ylabel("Power Sterngth / dBm",fontdict={'family' : 'Times New Roman', 'size': 12})
#plt.xlim(-65,-10)
#plt.ylim(-20,20)
plt.title('Power Strength Distribution')
plt.show()
