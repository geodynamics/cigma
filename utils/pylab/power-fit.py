#!/usr/bin/env python

NOGUI = False
import matplotlib
if NOGUI:
    matplotlib.use('Agg')

from pylab import *
from numpy import *
from scipy.optimize import leastsq

###############################################################################
X = array([0.433013, 0.216506, 0.108253])
Y = array([0.00678936462458,  0.00173206436735, 0.000380014510509]) # L2
#Y = array([0.000224602863157, 5.8797196366e-05, 1.45623441192e-05]) # Linf
###############################################################################

# parametric function
fp = lambda v,x: v[0] * x ** v[1]

# error function
e = lambda v,x,y: (fp(v,x) - y)

# initial parameter value
v0 = (1,1)

# find optimal parameters
(v, success) = leastsq(e, v0, args=(X,Y), maxfev=1000)

# plot points and best-fit curve
def plot_fit():
    plot_fn = loglog
    print "Best fit: e =", v[0], "* h ^", v[1]
    plot_fn(X, Y, 'ro')
    plot_fn(X, fp(v,X))
    title("$e\\ =\\ %f\\ h^{%f}$" % (v[0],v[1]))

# main function
def main():
    plot_fit()
    if NOGUI:
        savefig('power-fit.png')
    else:
        show()

if __name__ == '__main__':
    main()
