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

# iteration - noise
number_iterations = []

# font
font = {'family': 'Times New Roman','weight': 'normal','size': 12,}

def analyzeData(ax_cu, sigma):
	print("sigma {}".format(sigma))
	path = "data_" + str(sigma) + "sigma/"

	# Plot
	#fig = plt.figure(figsize=(5.5,4))
	#ax = plt.subplot(111)
	#plt.subplots_adjust(left=0.15, bottom=0.13, right=0.97, top=0.9,wspace=None, hspace=None)

	# Error : distance
	ErrorTotal = 0
	ErrorAveraged = 0 
	ErrorVariance = 0
	ErrorStdDev = 0

	# iteration - noise
	number_iteration_cu = 0

	# results for all points
	filename = path+'log_OptimizationResults.txt'
	data = open(filename)
	lines = data.readlines()
	data.close()

	all_last_x = []
	all_last_y = []
	for line in lines:
		line = line.strip()
		eles = line.split()
		all_last_x.append(float(eles[1]))
		all_last_y.append(float(eles[2]))

	#
	for taID in range(NumberNodes):
		filename = path+'log_nodeResults_ManagerNode_'+str(taID)+'.txt'
		data = open(filename)
		lines = data.readlines()
		data.close()

		iter_x = []
		iter_y = []
		iter_x.append(0)
		iter_y.append(0)
		for line in lines:
			line = line.strip()
			eles = line.split()
			iter_x.append(float(eles[0]))
			iter_y.append(float(eles[1]))

		iter_x.append(all_last_x[taID])
		iter_y.append(all_last_y[taID])
	
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

		# iteration - noise
		number_iteration_cu = number_iteration_cu + len(iter_x)
	
		if ax_cu!=None:
			# Plot 
			ax_cu.plot(iter_x, iter_y, marker='o',markersize=4, color='k', linestyle='-',mec='m', mfc='w',label='node'+str(taID))
			ax_cu.plot(iter_last_x, iter_last_y, marker=None,markersize=4, color='k', linestyle='--',mec='m', mfc='w',label=None)
			ax_cu.plot(iter_true_x, iter_true_y, marker='s',markersize=6, color='k', linestyle=None,mec='m', mfc='w',label=None)

	# iteration - noise
	number_iterations.append(number_iteration_cu/float(NumberNodes))

	# Error
	ErrorAveraged = ErrorTotal/float(NumberNodes)
	if NumberNodes>1:
		ErrorVariance /= float(NumberNodes-1)
	ErrorStdDev = math.sqrt(ErrorVariance)
	list_Error.append(ErrorAveraged)
	list_StdDev.append(ErrorStdDev)

	if ax_cu!=None:
		# Plot
		color = ['tab:blue', 'tab:orange', 'tab:green']
		ax_cu.scatter(anchor_x, anchor_y, marker='s',s=150,c=color[0],alpha=0.8,edgecolors='none',label='Anchor')
		ax_cu.scatter(target_x, target_y, marker='o',s=150,c=color[1],alpha=0.7,edgecolors='none',label='Target')
	#	plt.legend(loc='lower right',frameon=True)
		ax_cu.set_xlabel('X / m', font)
		ax_cu.set_ylabel('Y / m', font)
	#	plt.xlabel('X / m',fontdict={'family' : 'Times New Roman', 'size': 12})
	#	plt.ylabel("Y / m",fontdict={'family' : 'Times New Roman', 'size': 12})
	#	plt.xlim(-105,170)
	#	plt.ylim(-110,150)
	#	plt.title('Node and Iteration Distribution with '+str(sigma)+' StdVev')
	#	plt.savefig('figure-NodeDistribution-'+str(sigma)+'sigma.png',dpi=300)

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
fig, ax = plt.subplots(figsize=(4,8),ncols=1,nrows=2)
plt.subplots_adjust(left=0.19, bottom=0.1, right=0.99, top=0.99,wspace=None, hspace=0.3)

left, width = .3, .5
bottom, height = -.16, .5
ax[0].text(left, bottom, "(a) $\sigma_{Ti,Aj}=0$", fontsize=16,horizontalalignment='left',verticalalignment='top',transform=ax[0].transAxes)
ax[1].text(left, bottom, "(b) $\sigma_{Ti,Aj}=12 dBm$", fontsize=16,horizontalalignment='left',verticalalignment='top',transform=ax[1].transAxes)

#
sampleSigma = [0,4,8,12]
#
analyzeData(ax[0],sampleSigma[0])

#
analyzeData(None,sampleSigma[1])

#
analyzeData(None,sampleSigma[2])

#
analyzeData(ax[1],sampleSigma[3])

plt.savefig('figure-NodeDistribution-All.png',dpi=300)

print("list_Error {}".format(list_Error))

# Plot: Error
fig = plt.figure(figsize=(4,3))
ax = plt.subplot(111)
plt.subplots_adjust(left=0.14, bottom=0.20, right=None, top=None,wspace=None, hspace=None)
plt.errorbar(sampleSigma,list_Error,yerr=list_StdDev,fmt='ko-',ecolor='k',elinewidth=3,ms=5,mfc='wheat',mec='salmon',capsize=3)
plt.xlabel(r'$\sigma_{Ti,Aj}\ /\ dBm$',fontdict={'family' : 'Times New Roman', 'size': 12})
plt.ylabel("Error / m",fontdict={'family' : 'Times New Roman', 'size': 12})
plt.savefig('figure-ErrorDistribution.png',dpi=300)
plt.show()

# write
f1=open('log_error_noise.txt','w')
counter = 0
while counter<len(sampleSigma):
	line = "{} {} {}\n".format(sampleSigma[counter],list_Error[counter],list_StdDev[counter])
	print(line)
	f1.write(line)
	counter += 1

f2=open('log_iteration_noise.txt','w')
print(number_iterations)

