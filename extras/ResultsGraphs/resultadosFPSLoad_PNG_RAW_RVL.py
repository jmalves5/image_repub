import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
N = 4
ind = np.arange(N)  # the x locations for the groups
width = 0.07       # the width of the bars

fig = plt.figure()
ax = fig.add_subplot(111)
ax2 = ax.twinx()

fr1desk_fps = [30, 29.96, 17.48, 0.85]
rects1 = ax.bar(ind+(ind+width)/2, fr1desk_fps, width, color='darkorange', label='FPS')
fr1desk2_fps = [30, 29.79, 17.18, 0.84]
rects2 = ax.bar(ind+width+(ind+width)/2, fr1desk2_fps, width, color='darkorange')
fr1room_fps = [30, 28.26, 19.51, 0.9]
rects3 = ax.bar(ind+width*2+(ind+width)/2, fr1room_fps, width, color='darkorange')
fr2desk_fps = [30, 27.99, 17.69, 0.88]
rects4 = ax.bar(ind+width*3+(ind+width)/2, fr2desk_fps, width, color='darkorange')
fr2xyz_fps = [30, 29.67, 17.49, 0.89]
rects5 = ax.bar(ind+width*4+(ind+width)/2, fr2xyz_fps, width, color='darkorange')
fr3office_fps = [30, 26.28, 18.16, 0.93]
rects6 = ax.bar(ind+width*5+(ind+width)/2, fr3office_fps, width, color='darkorange')
fr3nst_fps = [30, 28.58, 21.75, 1.1]
rects7 = ax.bar(ind+width*6+(ind+width)/2, fr3nst_fps, width, color='darkorange')

fr1desk_load = [303.81, 52.04, 52.46, 21.42]
rects8 = ax2.bar(ind+width*7+(ind+width)/2, fr1desk_load, width, color='royalblue', label='Network Usage (Mbps)')
fr1desk2_load = [302.87, 51.14, 50.93, 19.5]
rects9 = ax2.bar(ind+width*8+(ind+width)/2, fr1desk2_load, width, color='royalblue')
fr1room_load = [303.2, 51.84, 52.7, 19.62]
rects10 = ax2.bar(ind+width*9+(ind+width)/2, fr1room_load, width, color='royalblue')
fr2desk_load = [304.22, 56.99, 52.14, 23.48]
rects11 = ax2.bar(ind+width*10+(ind+width)/2, fr2desk_load, width, color='royalblue')
fr2xyz_load = [305.02, 53.73, 51.32, 22.14]
rects12 = ax2.bar(ind+width*11+(ind+width)/2, fr2xyz_load, width, color='royalblue')
fr3office_load = [291.85, 54.36, 49.93, 19.94]
rects13 = ax2.bar(ind+width*12+(ind+width)/2, fr3office_load, width, color='royalblue')
fr3nst_load = [292.48, 51.08, 45.62, 17.7]
rects14 = ax2.bar(ind+width*13+(ind+width)/2, fr3nst_load, width, color='royalblue')


ax.set_ylabel('FPS')
ax2.set_ylabel('Network Usage (Mbps)')
ax2.set_xticks(ind+1.05+(ind-1)*7*width)
ax2.set_xticklabels(('Raw', 'RVL', 'PNG (1)', 'PNG(9)'))

lines, labels = ax.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax2.legend(lines + lines2, labels + labels2, loc=0)

ax.legend(['FPS', 'Network Usage (Mbps)'])



def autolabel(rects):
    for rect in rects:
        h = rect.get_height()

autolabel(rects1)
autolabel(rects2)
autolabel(rects3)
autolabel(rects4)
autolabel(rects5)
autolabel(rects6)
autolabel(rects7)
autolabel(rects8)
autolabel(rects9)
autolabel(rects10)
autolabel(rects11)
autolabel(rects12)
autolabel(rects13)
autolabel(rects14)



plt.show()
