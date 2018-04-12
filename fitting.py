from scipy.optimize import leastsq
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def func(x,y,parameters):
    [X,Y]=np.meshgrid(x,y)
    x0,y0,sigmax,sigmay,value=parameters
    return value*np.exp(-(X-x0)**2/2/sigmax**2-(Y-y0)**2/2/sigmay**2)

def residual(parameters,z,x,y):
    return z-func(x,y,parameters)

def fitting(x,y):
    p0 = [1, 1, 2, 2, 5]
    plsq = leastsq(residual, p0, args=(y, x))
    print(plsq[0])

def check_fitting(x,y,z,p):
    [X,Y]=np.meshgrid(x,y)
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.view_init(elev=50, azim=80)
    ax.plot_surface(X, Y, func(x, y, p))
    ax.plot_surface(X,Y,z)
    plt.show()


