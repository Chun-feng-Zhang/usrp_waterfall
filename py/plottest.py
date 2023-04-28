import numpy as np
import matplotlib.pylab as plt
import matplotlib.animation as animation
import time
from pylab import *
import os,sys

fcenter=1400e6
bw=56e6
nch=2048
rate=bw
dt=1/bw*nch
fmin=(fcenter-bw/2)/1e6
fmax=(fcenter+bw/2)/1e6
DS = 8
FDS = 64

fo = open(sys.argv[1],'r')
data = fromfile(fo, count=257*DS*nch, dtype=np.float32).reshape((-1, nch))
l, m = data.shape
print(l,m)

lmod =  l - (l % DS)
print(lmod)

data = data.reshape(-1,nch//FDS,FDS).sum(axis=-1)
data -= data.mean(axis=0).T
data = data[:lmod, :].reshape(-1, DS, nch//FDS).sum(axis=1)
t0 = 0
im = plt.imshow(data.T, aspect='auto', origin='lowleft', extent=[t0, dt*DS*lmod, fmin, fmax])
t0 += dt*DS*lmod
#plot(data.sum(axis=0))
#xlim(1390, 1430)

plt.xlabel('time (s)')
plt.ylabel('freq (Hz)')
plt.show()
while True:
    data = fromfile(fo, count=257*DS*nch, dtype=np.float32).reshape((-1, nch))

    l, m = data.shape
    #print(l,m)

    lmod =  l - (l % DS)
    #print(lmod)

    data = data.reshape(-1,nch//FDS,FDS).sum(axis=-1)
    data -= data.mean(axis=0).T
    data = data[:lmod, :].reshape(-1, DS, nch//FDS).sum(axis=1)

    im = plt.imshow(data.T, aspect='auto', origin='lowleft', extent=[t0, t0+ dt*DS*lmod, fmin, fmax])
    plt.xlabel('time (s)')
    plt.ylabel('freq (Hz)')
    plt.show()
    t0 += dt*DS*lmod
    time.sleep(0)
    

fo.close()
