import matplotlib.pyplot as plt

import numpy as np

import serial

import time

serdev = '/dev/ttyACM0'

t = np.arange(0,10,0.1) # time vector; create 100 samples between 0 and 10.0 sec.

s = serial.Serial(serdev,baudrate=115200)

x = np.arange(0,10,0.1)
y = np.arange(0,10,0.1)
z = np.arange(0,10,0.1)
tilt = np.arange(0,10,0.1)

for i in range(100):
     line=s.readline()
     
     x[i]=line.split()[0]
     y[i]=line.split()[1]
     z[i]=line.split()[2]
     tilt[i]=line.split()[3]

     

fig,ax = plt.subplots(2, 1)
for i in range(100):
    ax[1].plot([t[i],t[i]],[0,tilt[i]],color="blue", linewidth=1.5, linestyle="-")
    ax[1].scatter([t[i],], [tilt[i],],50 , color="red")
  #  plt.ax[1].ylabel("tilt")
  #  plt.ax[1].xlabel("Time")
plt.yticks([0, +1])
ax[0].plot(t, x, color="red", linewidth=2, linestyle="--", label="x")
ax[0].plot(t, y, color="green", linewidth=2, linestyle="-", label="y")
ax[0].plot(t, z, color="blue", linewidth=2, linestyle="-.", label="z")
ax[0].legend(loc='best')
#plt.ax[0].ylabel("Acc vector")
#plt.ax[0].xlabel("Time")
plt.show()