#! /usr/bin/env python

import os, sys, glob, subprocess, argparse

if os.path.exists('./.run.py'):
    exec(open("./.run.py").read())
else:
    resultsDir = "."
    threads=None

if not threads:
    parser = argparse.ArgumentParser()
    parser.add_argument('threads', type=int)
    args = parser.parse_args()
    threads = args.threads

t = []
x = []
speedup = []

def readTemps(n):
    s = os.path.join(resultsDir, 'temps_' + str(n) + '.dat')
        
    with open(s) as f:
        l = f.readline().split()
        return float(l[1])
        
    return 0.0

x0 = readTemps(0)

for i in range(1,threads+1):
    u = readTemps(i)
    t.append(i)
    x.append(u)
    speedup.append(x0/u)

print(x)
print(speedup)

import matplotlib.pyplot as plt


fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10,16))

ax1.plot(t, x, 'o-')
ax1.set_ylabel('Temps CPU')
ax1.grid()

ax2.plot(t, speedup, 'o-')
ax2.plot(t, t, '-')
plt.ylim(0, threads)
ax2.legend(['mesuré', 'théorique'])
ax2.set_xlabel('Threads')
ax2.set_ylabel('Speedup')
ax2.grid()

fig.set_size_inches(5, 7)
plt.show()


