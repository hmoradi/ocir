import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import collections
f = open('data1.txt')
width = 8
height = 8

data = {}
row = 0
index = 0
processed_frames = 0
for line in f.readlines():
	lines = line.split(' ')
	if len(lines) == 9:
		#print lines
		for col in range(0,8):
			index = (row * width) + col   
			if index not in data:
				data[index] = []
			data[index].append(int(lines[col]))
		row = row + 1
		if row == 8:
			processed_frames += 1
		row = row % 8

stds = {}
temps = {}

step = 10
t = 0
while (t < 25000 ):
	stds[t] = []
	temps[t] = []
	for key in data.keys():
		stds[t].append(np.std(data[key][t:t+step]))
		temps[t].append(np.mean(data[key][t:t+step]))

	t += step
print processed_frames
#for t in stds.keys():
	#plt.plot(stds[t],label=str(t))
	#plt.errorbar(data.keys(),temps[t], yerr=stds[t], fmt='-o',label=str(t))
#legend = plt.legend(loc='upper right', shadow=True)
#plt.show()
prekey = -1
cnt = 0

odtemps = collections.OrderedDict(sorted(temps.items()))
odstds = collections.OrderedDict(sorted(stds.items()))
stvals = []
last_presences = -10
for key in odstds.keys():
	if prekey == -1:
		prekey = key
		continue

	if(key - last_presences >= 20):
		thresh = 2
	else:
		thresh = 0.5
	if((np.mean(odtemps[key]) >= np.mean(odtemps[prekey]) + np.mean(odstds[prekey]) or True) and 
		(np.mean(odstds[key]) - np.mean(odstds[prekey])) >= thresh):
		last_presences = key
		print key, np.mean(odtemps[key]),np.mean(odstds[key]), np.mean(odtemps[prekey]) , np.mean(odstds[prekey]),np.mean(odstds[key]) -np.mean(odstds[prekey])
		stvals.append(np.mean(odstds[key]))
		cnt += 1
	prekey = key

print cnt
#plt.plot(stvals)
#plt.show()