import math
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

situations_sampleSigma = []
situations_list_Error = []
situations_list_StdDev = []

def analyzeError(filename):
	sampleSigma = []
	list_Error = []
	list_StdDev = []

	data = open(filename)
	lines = data.readlines()
	data.close()

	for line in lines:
		line = line.strip()
		eles = line.split()
		sampleSigma.append(float(eles[0]))
		list_Error.append(float(eles[1]))
		list_StdDev.append(float(eles[2]))

	print(sampleSigma)
	print(list_Error)
	print(list_StdDev)

	situations_sampleSigma.append(sampleSigma)
	situations_list_Error.append(list_Error)
	situations_list_StdDev.append(list_StdDev)

	return

# main
print("hello")

#
analyzeError("log_error_0weight.txt")
analyzeError("log_error.txt")

#
fig = plt.figure(figsize=(4,3))
ax = plt.subplot(111)
plt.subplots_adjust(left=0.14, bottom=0.20, right=None, top=None,wspace=None, hspace=None)

plt.errorbar(situations_sampleSigma[0],situations_list_Error[0],yerr=situations_list_StdDev[0],fmt='ko-',ecolor='k',elinewidth=5,ms=5,mfc='wheat',mec='salmon',capsize=3,label=r'$P_{Th} \to -\infty $')
plt.errorbar(situations_sampleSigma[1],situations_list_Error[1],yerr=situations_list_StdDev[1],fmt='ks--',ecolor='m',elinewidth=1.5,ms=5,mfc='wheat',mec='salmon',capsize=3,label='$P_{Th}=-10$')

plt.xlabel(r'$\sigma_{Ti,Aj}\ /\ dBm$',fontdict={'family' : 'Times New Roman', 'size': 12})
plt.ylabel("Error / m",fontdict={'family' : 'Times New Roman', 'size': 12})

#plt.legend(['Standard Deviation', 'Standard Error'],loc='upper left',numpoints=1,fancybox=True)
plt.legend()
#plt.savefig('figure-ErrorDistributionAll.png',dpi=300)
plt.show()
