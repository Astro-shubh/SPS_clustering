import matplotlib.pyplot as plt
import numpy as np

X_locations=[]
Y_locations=[]

f1=open('DM_time_axes.txt','r')
for line in f1:
    s=[float(r) for r in line.split()]
    X_locations.append(s[0])
    Y_locations.append(s[1])

Edge1=[]
Edge2=[]
Weight=[]
f2=open('cluster_labels.txt','r')
plt.xlabel('Time (ms)')
plt.ylabel('Dispersion delay (ms)')
for line in f2:
    s=[float(r) for r in line.split()]
    plt.scatter(X_locations, Y_locations, c=s)


    plt.show()


