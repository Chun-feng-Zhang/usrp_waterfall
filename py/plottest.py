import numpy as np
import matplotlib.pylab as plt
import matplotlib.animation as animation
import time
from pylab import *

fcenter=1400e6
bw=40e6
nch=2048
rate=bw
dt=1/bw*nch
fmin=(fcenter-bw/2)/1e6
fmax=(fcenter+bw/2)/1e6

nch = 2048
fo = open('test.dat','r')
data = fromfile(fo, count=1024*32*2048, dtype=np.float32).reshape((-1, 2048))
l, m = data.shape
print(l,m)

lmod =  l - (l % 16)
print(lmod)

data = data[:lmod, :].reshape(-1, 16, 2048).sum(axis=1)
t0 = 0
im = plt.imshow(data.T, aspect='auto', origin='lowleft', extent=[t0, dt*16*lmod, fmin, fmax])
t0 += dt*16*lmod
#plot(data.sum(axis=0))
#xlim(1390, 1430)

plt.xlabel('time (s)')
plt.ylabel('freq (Hz)')
plt.show()
while True:
    data = fromfile(fo, count=1024*32*2048, dtype=np.float32).reshape((-1, 2048))

    l, m = data.shape
    #print(l,m)

    lmod =  l - (l % 16)
    #print(lmod)

    data = data[:lmod, :].reshape(-1, 16, 2048).sum(axis=1)

    im = plt.imshow(data.T, aspect='auto', origin='lowleft', extent=[t0, t0+ dt*16*lmod, fmin, fmax])
    plt.xlabel('time (s)')
    plt.ylabel('freq (Hz)')
    plt.show()
    time.sleep(0)
    

fo.close()
