from pylab import *
import numpy as np


nch = 2048
data = fromfile('test.dat', count=256*32*2048, dtype=np.float32).reshape((-1, 2048))
l, m = data.shape
print(l,m)

lmod =  l - (l % 256)
print(lmod)

data = data[:lmod, :].reshape(-1, 256, 2048).sum(axis=1)

imshow(np.log(data).T, aspect='auto', origin='lowleft')
#plot(data.sum(axis=0))
#xlim(1390, 1430)
show()
