import math
import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

print("hello")

x = [1,2,3]
y = [2,4,6]

nx = [2,2,3]
ny = [3,4,5]


plt.hist2d(x,y)
plt.plot(nx,ny)
plt.show()
