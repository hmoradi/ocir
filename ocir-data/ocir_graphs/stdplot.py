import pandas as pd

df = pd.read_csv('stdrates.txt')
print len(df)

#!/usr/bin/env python
# a bar plot with errorbars
import numpy as np
import matplotlib.pyplot as plt

N = 4
menMeans = df['fp']
#menStd = (2, 3, 4, 1, 2)

ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(ind, menMeans, width, color='r')

womenMeans = df['fn']
df['percent'] = (df['number']* 100 )/64
df['percent'] = df['percent'].apply(lambda x:int(x))
#womenStd = (3, 5, 2, 3, 3)
rects2 = ax.bar(ind + width, womenMeans, width, color='y')

# add some text for labels, title and axes ticks
ax.set_ylabel('Percenate (%)',fontsize='14')
#ax.set_title('Scores by group and gender')
ax.set_xticks(ind + width)
ax.set_xticklabels(df['percent'],fontsize='14')
ax.set_xlabel('Active Pixels (%)',fontsize='14')
ax.legend((rects1[0], rects2[0]), ('False Positive', 'False Negative'),loc=2,fontsize='14')


def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%d' % int(height),
                ha='center', va='bottom',fontsize='14')

autolabel(rects1)
autolabel(rects2)

plt.savefig("stdfpfn.pdf");