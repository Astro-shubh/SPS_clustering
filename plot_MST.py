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
f2=open('Spanning_tree.txt','r')
for line in f2:
    s=[float(r) for r in line.split()]
    Edge1.append(int(s[0]))
    Edge2.append(int(s[1]))
    Weight.append(s[2])

colors = plt.cm.hot(np.linspace(0, 1, len(Weight)))
max_W=max(Weight)
plt.xlabel('Time of arrival (ms)')
plt.ylabel('Dispersion delay (ms)')
for i in range(len(Weight)):
    X=[]
    Y=[]
    if(Edge1[i] == -1):
        X.append(X_locations[Edge1[0]])
        Y.append(Y_locations[Edge1[0]])
    else:
        X.append(X_locations[Edge1[i]])
        Y.append(Y_locations[Edge1[i]])
    if(Edge2[i] == -1):
        X.append(X_locations[Edge1[0]])
        Y.append(Y_locations[Edge1[0]])
    else:
        X.append(X_locations[Edge2[i]])
        Y.append(Y_locations[Edge2[i]])
    W=int((len(Weight)-1)*Weight[i]/max_W)
    plt.plot(X,Y)

plt.show()


