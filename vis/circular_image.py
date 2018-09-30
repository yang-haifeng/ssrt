#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import sys

# Open the file with the first argument
# When no argument supplied, use default file name: image.out
try:
  fname = sys.argv[1]
except IndexError:
  fname = "image.out"
f = open(fname)

# The first line gives the Nr and Nph
line = f.readline()
Nr, Nph = [int(t) for t in line.split()]
inc = float(f.readline())

# Read in the data. Reshape each variables
data = np.loadtxt(f)
R = data[:,0]
phi = data[:,1]
I = data[:,2]
Q = data[:,3]
U = data[:,4]
R.shape = Nr, Nph
phi.shape = Nr, Nph
I.shape = Nr, Nph
Q.shape = Nr, Nph
U.shape = Nr, Nph

# Below expanding phi range from (0, pi) to (0, 2pi)
R1 = np.zeros((Nr, 2*Nph-1))
R1[:,:Nph] = R
R1[:,Nph:] = R[:,-2::-1] # R is simply copied
R = R1.copy()

I1 = np.zeros((Nr, 2*Nph-1))
I1[:,:Nph] = I
I1[:,Nph:] = I[:,-2::-1] # I is simply copied
I = I1.copy()

phi1 = np.zeros((Nr, 2*Nph-1))
phi1[:,:Nph] = phi
phi1[:,Nph:] = 2*np.pi-phi[:,-2::-1] # phi is reflected with pi in middle
phi = phi1.copy()

Q1 = np.zeros((Nr, 2*Nph-1))
Q1[:,:Nph] = Q
Q1[:,Nph:] = Q[:,-2::-1] # Q is copied
Q = Q1.copy()

U1 = np.zeros((Nr, 2*Nph-1))
U1[:,:Nph] = U
U1[:,Nph:] = -U[:,-2::-1] # U picks up a minus
U = U1.copy()

plt.contourf(R*np.cos(phi)*np.cos(inc), R*np.sin(phi), I, 50)
plt.gca().set_aspect(1)

angle = 0.5*np.arctan2(U, Q)
p = np.sqrt(Q**2+U**2)/I

kwargs = {"headlength":0, "headaxislength":0, "headwidth":1, "pivot":'middle'}
plt.figure()
plt.contourf(R*np.cos(phi)*np.cos(inc), R*np.sin(phi), p*I, 100)
plt.quiver(R*np.cos(phi)*np.cos(inc), R*np.sin(phi), p*np.cos(angle), p*np.sin(angle), **kwargs)
plt.gca().set_aspect(1)

plt.figure()
plt.contourf(R*np.cos(phi)*np.cos(inc), R*np.sin(phi), p, 50)
plt.colorbar()
plt.gca().set_aspect(1)

plt.show()
