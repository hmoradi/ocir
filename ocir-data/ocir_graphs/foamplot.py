import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
label_size = 26
mpl.rcParams['xtick.labelsize'] = label_size 
mpl.rcParams['ytick.labelsize'] = label_size 
f = open('foamlog.txt')

data = [[] for i in range(64)]
row = 0
col = 0
for line in f.readlines():
	if "T" in line:
		continue
	sline = line.strip().split(" ")
	if len(sline) <= 2:
		continue
	for element in sline:
		data[row*8+col].append(int(element)/4.0)
		col = (col + 1)%8
	row = (row + 1)%8


step = 1000
means = []
stds = []
x = []
index = 0

for pixel in range(0,64,1):
	x = []
	means = []
	stds = []
	index = 0
	for i in range(0,len(data[pixel]),step):
		print i
		means.append(np.mean(data[pixel][i:i+step]))
		stds.append(np.std(data[pixel][i:i+step]))
		x.append(index)
		index = index + 1
	plt.errorbar(x, means, yerr=stds)

plt.ylabel("temperature (C)",fontsize='46')
plt.xlabel("time (*1000)(s)",fontsize='46')
plt.tight_layout()
plt.savefig("foamtest.pdf")