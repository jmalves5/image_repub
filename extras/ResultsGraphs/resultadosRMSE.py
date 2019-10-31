import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
N = 7
ind = np.arange(N)  # the x locations for the groups
width = 0.2       # the width of the bars

fig = plt.figure()
ax = fig.add_subplot(111)

local_rmse = [1.68, 2.6, 4.69, 0.98, 0.35, 1.12, 2.46]
rects1 = ax.bar(ind+(ind+width)/2, local_rmse, width, color='darkorange', label='Local ORB-SLAM2')

png_rmse = [1.72, 2.46, 5.15, 0.86, 0.34, 1.11, 1.93]
rects2 = ax.bar(ind+width+(ind+width)/2, png_rmse, width, color='darkgreen', label='Remote ORB-SLAM2 - PNG(1)')

rvl_rmse = [1.63, 2.56, 4.7, 0.88, 0.35, 1.17, 1.83]
rects3 = ax.bar(ind+width*2+(ind+width)/2, rvl_rmse, width, color='royalblue', label='Remote ORB-SLAM2 - RVL')


ax.set_ylabel('RMSE (cm)')
ax.set_xticks(ind+2*width+3*ind*width-0.5*width*ind)
ax.set_xticklabels(('fr1desk', 'fr1desk2', 'fr1room', 'fr2desk', 'fr2xyz', 'fr3office', 'fr3nst'))

lines, labels = ax.get_legend_handles_labels()

ax.legend(lines,labels, loc='0')


def autolabel(rects):
    for rect in rects:
        h = rect.get_height()

autolabel(rects1)
autolabel(rects2)
autolabel(rects3)

plt.show()
