import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

import matplotlib as mpl
label_size = 26
mpl.rcParams['xtick.labelsize'] = label_size 
mpl.rcParams['ytick.labelsize'] = label_size 
def plot(df,sensor):
	N = 6
	feet0 = (df['Band1'][df['distance']==0] - df[sensor][df['distance']==0]).apply(lambda x:abs(x))
	feet1 = (df['Band1'][df['distance']==1] - df[sensor][df['distance']==1]).apply(lambda x:abs(x))
	feet2 = (df['Band1'][df['distance']==2] - df[sensor][df['distance']==2]).apply(lambda x:abs(x))
	feet3 = (df['Band1'][df['distance']==3] - df[sensor][df['distance']==3]).apply(lambda x:abs(x))
	feet4 = (df['Band1'][df['distance']==4] - df[sensor][df['distance']==4]).apply(lambda x:abs(x))
	feet5 = (df['Band1'][df['distance']==5] - df[sensor][df['distance']==5]).apply(lambda x:abs(x))

	ind = np.arange(N)  # the x locations for the groups
	width = 0.15       # the width of the bars

	fig, ax = plt.subplots()
	rects0 = ax.bar(ind, feet0, width, color='r') 
	rects1 = ax.bar(ind + width, feet1, width, color='y')
	rects2 = ax.bar(ind + 2*width, feet2, width, color='g')
	rects3 = ax.bar(ind + 3*width, feet3, width, color='b')
	rects4 = ax.bar(ind + 4*width, feet4, width, color='tan')
	rects5 = ax.bar(ind + 5*width, feet5, width, color='m')

	# add some text for labels, title and axes ticks
	ax.set_xlabel('Subject (#)',fontsize='46')
	ax.set_xticks(ind + 3*width)
	ax.set_xticklabels([1, 2, 3, 4, 5,6],fontsize='46')
	ax.set_ylabel('Temperature Difference with Ground-truth (C)',fontsize='46')
	ax.legend((rects0[0], rects1[1], rects2[0], rects3[0], rects4[0], rects5[0]), ('0 feet', '1 feet', '2 feets', '3 feets', '4 feets', '5 feets'),fontsize='12',
		loc='upper center', bbox_to_anchor=(0.5, 1.05),ncol=3, fancybox=True, shadow=True)
	#plt.ylim((0,2))
	plt.tight_layout()
	plt.savefig(sensor+".pdf");

df = pd.read_csv('bandinfo.csv')
plot(df,"Heiman")
plot(df,"GridEYE")