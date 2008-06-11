#!/usr/bin/env python

NOGUI = False
import matplotlib
if NOGUI:
    matplotlib.use('Agg')

from pylab import *
from numpy import *
from scipy.optimize import leastsq

X = None
Y = None

S = [False for i in range(8)]
S[0] = True

###############################################################################
# Results from ~/w/laplace/D04
if S[0]:
    X = [0.433013, 0.216506, 0.108253]
    Y = [0.00678936462458,  0.00173206436735, 0.000380014510509] # L2
    #Y = [0.000224602863157, 5.8797196366e-05, 1.45623441192e-05] # Linf


###############################################################################
# Results from ~/w/laplace/L04
if S[1]:
    pass

###############################################################################
# Results from ~/w/elastic/test3/axialtract.cfg
if S[2]:
    pass

###############################################################################
# Results from ~/w/elastic/test4/shearxy.cfg
if S[3]:
    pass

###############################################################################
# Results from ~/w/elastic/test4/dislocation.cfg
if S[4]:
    pass

###############################################################################
# Results from ~/w/mc/test1/case*.vtr
if S[5]:
    pass

###############################################################################
# Results from ~/w/gale/test1/pressure2.vts
if S[6]:
    pass

###############################################################################
# Results
if S[7]:
    pass

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
    x = array(X)
    y = array(Y)
    plot_fn(x, y, 'ro')
    plot_fn(x, fp(v,x))
    title(r"$\varepsilon\ =\ %f\ h^{%f}$" % (v[0],v[1]))

# main function
def main():
    plot_fit()
    if NOGUI:
        savefig('power-fit.png')
    else:
        show()

if __name__ == '__main__':
    main()
