import matplotlib.pyplot as plt
import matplotlib
norm = matplotlib.colors.Normalize(vmin=0, vmax=1)
import numpy as np

X_locations=[]
Y_locations=[]

f1=open('DM_time_axes.txt','r')
for line in f1:
    s=[float(r) for r in line.split()]
    X_locations.append(s[0])
    Y_locations.append(s[1])

index=[]
label=[]

f2=open('cluster_core.txt','r')

plt.xlabel('Time (ms)')
plt.ylabel('Dispersion delay (ms)')
for line in f2:
    s=[int(r) for r in line.split()]
    index.append(s[0])
    label.append(float(s[1]))

print(max(index))
index = np.array(index)
X=np.array(X_locations)[index]
Y=np.array(Y_locations)[index]
#print(max(label))
label=np.array(label)/max(label)
print(len(X))

plt.scatter(X,Y)
plt.show()
plt.scatter(X_locations,Y_locations)
plt.show()

