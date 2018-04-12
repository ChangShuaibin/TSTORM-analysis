from scipy.optimize import leastsq
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import tifffile
import ctypes
import file_reader as reader

def func(x,y,parameters):
    [X,Y]=np.meshgrid(x,y)
    x0,y0,sigmax,sigmay,value=parameters
    return value*np.exp(-(X-x0)**2/2/sigmax**2-(Y-y0)**2/2/sigmay**2)

def residual(parameters,z,x,y):
    return z-func(x,y,parameters)

def fitting(x,y,z):
    p0 = [1, 1, 2, 2, 5]
    plsq = leastsq(residual, p0, args=(z, x, y))
    return(plsq[0])

def check_fitting(x,y,z,p):
    [X,Y]=np.meshgrid(x,y)
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.view_init(elev=50, azim=80)
    ax.plot_surface(X, Y, func(x, y, p))
    ax.plot_surface(X,Y,z)
    plt.show()

class painter():
    def __init__(self,filename):
        super().__init()
        self.file=tifffile.TiffWriter('D:\\Data\\reconstruction\\test.tif')
        self.find_peak=ctypes.cdll.LoadLibrary('')
        self.filename=filename

    def read_image(self,filename):
        data=reader.TifReader(filename)
        return data.loadAFrame()

    def find_peaks(self):
        images=self.read_image(self.filename)
        peaks=self.find_peak.find_peaks(images)
        self.param=[]
        for i in peaks:
            z=[]
            x=np.arange(i.xp-i.xw,i.xp+i.xw+1)
            y=np.arange(i.yp-i.yw,i.yp+i.yw+1)
            for j in y:
                z+=[images[i.image][x[0]:x[-1]+1]]
            z=np.array(z)
            self.param+=[fitting(x,y,z)]

    def paint(self):
        array=np.ascontiguousarray(np.zeros((2048,2048)))
        for i in self.param:
            array[i[0]][i[1]]=255
        self.file.imwrite(array)
        self.file.close()

