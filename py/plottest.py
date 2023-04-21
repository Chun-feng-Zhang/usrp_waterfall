from pylab import *
import numpy as np


nch = 2048
data = fromfile('test.dat', dtype=np.float32).reshape((-1, 2048))
l, m = data.shape
print(l,m)

lmod =  l - (l % 32)
print(lmod)

data = data[:lmod, :].reshape(-1, 32, 2048).sum(axis=1)

imshow(np.log(data), aspect='auto')
#plot(data.sum(axis=1))
show()
