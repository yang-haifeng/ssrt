#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import sys

try:
  fname = sys.argv[1]
except IndexError:
  fname = "image.out"
f = open(fname)

Npix = int(f.readline())
inc  = float(f.readline())

data = np.loadtxt(f)
X = data[:,0]
Y = data[:,1]
I = data[:,2]
Q = data[:,3]
U = data[:,4]
V = data[:,5]
X.shape = Npix, Npix
Y.shape = Npix, Npix
I.shape = Npix, Npix
Q.shape = Npix, Npix
U.shape = Npix, Npix
V.shape = Npix, Npix
X=X.T
Y=Y.T
I=I.T
Q=Q.T
U=U.T
V=V.T

extent=[X[0,0],X[0,-1],Y[0,0],Y[-1,0]]

plt.figure(figsize=(15,8))
plt.subplot(231)
kwargs = {"headlength":0, "headaxislength":0, "headwidth":1, "pivot":'middle'}
Ip = np.sqrt(Q**2+U**2)
p=Ip/I
angle=0.5*np.arctan2(U,Q)
plt.imshow(Ip, extent=extent, origin="lower", cmap="Blues")
plt.colorbar()
sl = slice(None, None, Npix//10)
plt.quiver(X[sl,sl],Y[sl,sl],p[sl,sl]*np.cos(angle)[sl,sl],p[sl,sl]*np.sin(angle)[sl,sl],**kwargs)

plt.subplot(232)
plt.imshow(I, extent=extent, origin="lower", cmap="Blues")
plt.colorbar()

plt.subplot(233)
plt.imshow(p, extent=extent, origin="lower", cmap="plasma")
plt.colorbar()

plt.subplot(234)
assert Npix%2==0
pmaj = 0.5*(p[:,Npix//2]+p[:,Npix//2+1])
plt.plot(X[0,:], pmaj)

plt.subplot(235)
pmin = 0.5*(p[Npix//2,:]+p[Npix//2+1,:])
plt.plot(Y[:,0], pmin)

plt.subplot(236)
adeg = angle.copy()*180/np.pi
adeg.shape = -1
#plt.hist(adeg, bins=50)
plt.imshow(angle, extent=extent, origin="lower", cmap="plasma")
plt.colorbar()

plt.show()
