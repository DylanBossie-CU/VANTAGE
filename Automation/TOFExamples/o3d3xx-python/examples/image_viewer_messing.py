from __future__ import (absolute_import, division, print_function, unicode_literals)
from builtins import *
import o3d3xx
import sys
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pypcd
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
        print(len(self.Z))

    def readNextFrame(self):
        result = self.data.readNextFrame()
        self.X = np.frombuffer(result['x'],dtype='uint16') #Gives a Key to a library to pull the appropriate data
        #self.X = self.X.reshape(imageHeight,imageWidth)
        self.Y = np.frombuffer(result['y'],dtype='uint16')
        self.Z = np.frombuffer(result['z'],dtype='uint16')
        for key,value in result.iteritems():
        	print(key)
        #self.L=np.concatenate([self.X,self.Y,self.Z])
        #for i in range(len(self.X)):
        #	self.L[i] = (self.X[i],self.Y[i],self.Z[i])
        self.L = [self.X,self.Y,self.Z]
        self.L = np.transpose(self.L)
        
        #self.L = self.L.astype(np.float32)
    	#self.pc_data = self.L.view(np.dtype([('x', np.int16), ('y', np.int16),('z', np.int16)]))
    	# print(self.pc_data)
    	# print(len(self.pc_data))
    	pdb.set_trace()
        self.pc=pypcd.make_xyz_point_cloud(self.L)
       #print(self.pc.pc_data[:,:]) #This indicates that somewhere something is only taking the first column of a thing
        pypcd.save_point_cloud(self.pc, "apcd.pcd")
        # print(len(self.pc.pc_data[:,:]))
        # print(num)
        # self.pc.save("apcd.pcd")

        
       

def main():
    address = sys.argv[1]
    camData = o3d3xx.ImageClient(address, 50010)

    
    grabber = GrabO3D300(camData)
    for x in range(1):
    	grabber.readNextFrame()
   	
    

if __name__ == '__main__':
    main()

