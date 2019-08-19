import math
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

NumberAnchors = 0
NumberNodes = 0

# Error
list_Error = []
list_StdDev = []

def analyzeData(sigma):
	print("sigma {}".format(sigma))
	path = "data_" + str(sigma) + "sigma/"

	# Plot
	fig = plt.figure(figsize=(5.5,4))
	ax = plt.subplot(111)
	plt.subplots_adjust(left=0.15, bottom=0.13, right=0.97, top=0.9,wspace=None, hspace=None)

	# Error : distance
	ErrorTotal = 0
	ErrorAveraged = 0 
	ErrorVariance = 0
	ErrorStdDev = 0

	for taID in range(NumberNodes):
		filename = path+'log_nodeResults_ManagerNode_'+str(taID)+'.txt'
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
	
		iter_last_x = []
		iter_last_y = []
		iter_last_x.append(iter_x[-1])
		iter_last_y.append(iter_y[-1])
		iter_last_x.append(target_x[taID])
		iter_last_y.append(target_y[taID])
	
		iter_true_x = []
		iter_true_y = []
		iter_true_x.append(target_x[taID])
		iter_true_y.append(target_y[taID])

		# Error 
		dx = iter_last_x[0]-iter_true_x[0]
		dy = iter_last_y[0]-iter_true_y[0]
		distance2 = dx**2 + dy**2
		ErrorTotal = ErrorTotal + math.sqrt(distance2)
		ErrorVariance = ErrorVariance + distance2
	
		# Plot 
		plt.plot(iter_x, iter_y, marker='o',markersize=4, color='k', linestyle='-',mec='m', mfc='w',label='node'+str(taID))
		plt.plot(iter_last_x, iter_last_y, marker=None,markersize=4, color='k', linestyle='--',mec='m', mfc='w',label=None)
		plt.plot(iter_true_x, iter_true_y, marker='o',markersize=6, color='k', linestyle=None,mec='m', mfc='w',label=None)
		plt.plot(iter_true_x, iter_true_y, marker='+',markersize=6, color='k', linestyle=None,mec='m', mfc='w',label=None)

	# Error
	ErrorAveraged = ErrorTotal/float(NumberNodes)
	ErrorVariance /= float(NumberNodes)
	ErrorStdDev = math.sqrt(ErrorVariance)
	list_Error.append(ErrorAveraged)
	list_StdDev.append(ErrorStdDev)

	# Plot
	color = ['tab:blue', 'tab:orange', 'tab:green']
	plt.scatter(anchor_x, anchor_y, marker='s',s=150,c=color[0],alpha=0.8,edgecolors='none',label='Anchor')
	plt.scatter(target_x, target_y, marker='o',s=150,c=color[1],alpha=0.7,edgecolors='none',label='Target')
	plt.legend(loc='lower right',frameon=True)
	plt.xlabel('X / m',fontdict={'family' : 'Times New Roman', 'size': 12})
	plt.ylabel("Y / m",fontdict={'family' : 'Times New Roman', 'size': 12})
	plt.xlim(-105,170)
	plt.ylim(-110,150)
	plt.title('Node and Iteration Distribution with '+str(sigma)+' StdVev')
	plt.savefig('figure-NodeDistribution-'+str(sigma)+'sigma.png',dpi=300)

	return 

#------------------
# main
#------------------
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

for taID in range(NumberNodes):
	ID = taID + 1
	line = lines[ID]
	line = line.strip()
	eles = line.split()
	target_x.append(float(eles[6]))
	target_y.append(float(eles[7]))

#
sampleSigma = [0,4,8,12]
#
analyzeData(sampleSigma[0])

#
analyzeData(sampleSigma[1])

#
analyzeData(sampleSigma[2])

#
analyzeData(sampleSigma[3])

print("list_Error {}".format(list_Error))

# Plot: Error
fig = plt.figure(figsize=(4,3))
ax = plt.subplot(111)
plt.subplots_adjust(left=0.14, bottom=0.20, right=None, top=None,wspace=None, hspace=None)
plt.errorbar(sampleSigma,list_Error,yerr=list_StdDev,fmt='ko-',ecolor='k',elinewidth=3,ms=5,mfc='wheat',mec='salmon',capsize=3)
plt.xlabel(r'$\sigma_{Ti,Aj}\ /\ dBm$',fontdict={'family' : 'Times New Roman', 'size': 12})
plt.ylabel("Distance / m",fontdict={'family' : 'Times New Roman', 'size': 12})
plt.savefig('figure-ErrorDistribution.png',dpi=300)
plt.show()
