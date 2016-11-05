import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
label_size = 26
mpl.rcParams['xtick.labelsize'] = label_size 
mpl.rcParams['ytick.labelsize'] = label_size 
df1 = pd.read_csv('bodytemp1feet.txt')
df2 = pd.read_csv('bodytemp2feet.txt')
df3 = pd.read_csv('bodytemp3feet.txt')
df4 = pd.read_csv('bodytemp4feet.txt')

avgs = [[] for i in range(4)]
maxs = [[] for i in range(4)]

avgs[0] = df1['avg']/4
avgs[1] = df2['avg']/4
avgs[2] = df3['avg']/4
avgs[3] = df4['avg']/4

maxs[0] = df1['max']/4
maxs[1] = df2['max']/4
maxs[2] = df3['max']/4
maxs[3] = df4['max']/4

plt.boxplot(avgs)
plt.ylabel('Temperature',fontsize='46')
plt.xlabel('Distance to Sensor (ft)',fontsize='46')
plt.tight_layout()
plt.savefig("avgTempDistance.pdf")
plt.close()

plt.boxplot(maxs)
plt.ylabel('Temperature',fontsize='46')
plt.xlabel('Distance to Sensor',fontsize='46')
plt.tight_layout()
plt.savefig("maxTempDistance.pdf")
plt.close()

