from __future__ import (absolute_import, division, print_function, unicode_literals)
from builtins import *
import o3d3xx
import sys
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pypcd
import pdb
#ourstanding issue: being able to set image dimensions to that of the O3D313
#How to format this data
#This is not quite working yet.

imageWidth = 352
imageHeight = 264

num=imageWidth*imageHeight

class GrabO3D300():
    def __init__(self,data):
        self.data = data
        self.X = np.zeros(num)
        self.Y = np.zeros(num)
        self.Z = np.zeros(num)
        self.Amplitude = np.zeros((imageHeight,imageWidth))
        self.Distance = np.zeros((imageHeight,imageWidth))
        print(len(self.Z))

    def readNextFrame(self,frame):
        result = self.data.readNextFrame()
        self.Amplitude = np.frombuffer(result['amplitude'],dtype='uint16')
        self.Amplitude = self.Amplitude.reshape(imageHeight,imageWidth)
        self.Distance = np.frombuffer(result['distance'],dtype='uint16')
        self.Distance = self.Distance.reshape(imageHeight,imageWidth)
        self.illuTemp = 20.0
        self.X = np.frombuffer(result['x'],dtype='uint16') #Gives a Key to a library to pull the appropriate data
        #self.X = self.X.reshape(imageHeight,imageWidth)
        self.Y = np.frombuffer(result['y'],dtype='uint16')
        self.Z = np.frombuffer(result['z'],dtype='uint16')
        #self.L=np.concatenate([self.X,self.Y,self.Z])
        #for i in range(len(self.X)):
        #	self.L[i] = (self.X[i],self.Y[i],self.Z[i])
        self.L = [self.X,self.Y,self.Z]
        self.L = np.transpose(self.L)
        #self.L = self.L.astype(np.float32)
    	#self.pc_data = self.L.view(np.dtype([('x', np.int16), ('y', np.int16),('z', np.int16)]))
    	# print(self.pc_data)
    	# print(len(self.pc_data))
        self.pc=pypcd.make_xyz_point_cloud(self.L)
       #print(self.pc.pc_data[:,:]) #This indicates that somewhere something is only taking the first column of a thing
        pypcd.save_point_cloud(self.pc, "TOF_PointClouds/testframe_" + str(frame) + ".pcd")
        # print(len(self.pc.pc_data[:,:]))
        # print(num)
        # self.pc.save("apcd.pcd")

def updatefig(*args):
    g = args[1]
    g.readNextFrame(args[4]);
    imAmp = args[2]
    amp_max = float(max(np.max(g.Amplitude),1));
    imAmp.set_array(g.Amplitude/ amp_max)


    imDist = args[3]
    dist_max = float(max(np.max(g.Distance),1));
    imDist.set_array(g.Distance/ dist_max)
    
    return imAmp,imDist,
       

def main():
    address = sys.argv[1]
    camData = o3d3xx.ImageClient(address, 50010)

    
    grabber = GrabO3D300(camData)
    for x in range(0,5):
		grabber.readNextFrame(x)
		#fig = plt.figure()
		#ax1 = fig.add_subplot(1, 2, 1)
		#ax2 = fig.add_subplot(1, 2, 2)
		#imAmplitude = ax1.imshow(np.random.rand(imageHeight,imageWidth))
		#imDistance = ax2.imshow(np.random.rand(imageHeight,imageWidth))
		#ani = animation.FuncAnimation(fig, updatefig, blit=True, fargs = [grabber,imAmplitude,imDistance,x])
		#plt.show()
		#plt.close()

if __name__ == '__main__':
    main()

