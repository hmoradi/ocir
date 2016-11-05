import numpy as np
import matplotlib.pyplot as plt
from collections import OrderedDict

def plot_error_bar(avgs,stds):
	step = 100
	final_avgs = {}
	final_stds = {}

	last_key = 0
	avg_frame = 0
	for key in avgs.keys():
		avgs[key] = np.mean(avgs[key])
		stds[key] = np.mean(stds[key])
		if last_key == 0:
			last_key = key
		elif key - last_key >= step:
			last_key = key
		
		if last_key not in final_stds.keys():
			final_stds[last_key] = []
			final_avgs[last_key] = []
		
		final_avgs[last_key].append(avgs[key])
		final_stds[last_key].append(stds[key])

	ordered_avgs = OrderedDict(sorted(final_avgs.items(), key=lambda t: t[0]))
	ordered_stds = OrderedDict(sorted(final_stds.items(), key=lambda t: t[0]))

	for key in ordered_avgs.keys():
		ordered_avgs[key] = np.mean(ordered_avgs[key])
		ordered_stds[key] = np.mean(ordered_stds[key])
		print key,ordered_avgs[key],ordered_stds[key]  
	
	plt.errorbar(ordered_avgs.keys(), ordered_avgs.values(), yerr=ordered_stds.values(), fmt='-o')
	plt.show()

def plot_boxplot(avgs,stds):
	data = [[] for i in range(len(stds.keys()))]
	index = 0
	for key in stds.keys():
		
		data[index] = stds[key]
		index = index + 1
	print len(data)
	plt.boxplot(data[::120])
	plt.show()
f = open('noisestdlog.txt')
frameN = 0;
avgs = {}
stds = {}
step = 1
key = 0
for line in f.readlines():
	if len(line.split(" ")) != 2:
		continue
	#print line
	if "frame" in line:
		frameN = int(line.split(" ")[1])
	else:
	   if (frameN - key) == step or (key ==0):
	   		if key == 0:
	   			key = frameN 
	   		key = key+step 
	   if key not in avgs.keys():
	   		avgs[key] = []
	   		stds[key] = []
	   avgs[key].append(float(line.split(" ")[0]))
	   stds[key].append(float(line.split(" ")[1]))
plot_boxplot(avgs,stds)




