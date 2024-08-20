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
plt.xlabel('Time (ms)')
plt.ylabel('Dispersion delay (ms)')

plt.scatter(X_locations, Y_locations, c='k', alpha=0.1)
f2=open('cluster_labels1.txt','r')
#plt.xlabel('Time (ms)')
#plt.ylabel('Dispersion delay (ms)')
i=0
for line in f2:
    s=[int(r) for r in line.split()]
    X=np.array(X_locations)[s]
    Y=np.array(Y_locations)[s]
    plt.scatter(X,Y);
plt.show()


