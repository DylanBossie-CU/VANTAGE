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

imageWidth = 176
imageHeight = 132

num=imageWidth*imageHeight

class GrabO3D300():
    def __init__(self,data):
        self.data = data
        #self.X = np.zeros(num)
        #self.Distance = np.zeros(num)

    def readNextFrame(self):
        result = self.data.readNextFrame()
        self.X = np.frombuffer(result['x'],dtype='uint16') #Gives a Key to a library to pull the appropriate data
        #self.X = self.X.reshape(imageHeight,imageWidth)
        self.Y = np.frombuffer(result['y'],dtype='uint16')
        self.Z = np.frombuffer(result['z'],dtype='uint16')
        print(self.Z)
        print(num/3)
        self.L=np.array([self.X,self.Y,self.Z])
        print(len(self.L[1]))
        self.pc=pypcd.make_xyz_point_cloud(self.L)
        pypcd.save_point_cloud(self.pc, "apcd.pcd")
        # print(len(self.pc.pc_data[:,:]))
        # print(num)
        # self.pc.save("apcd.pcd")

        
       

def main():
    address = sys.argv[1]
    camData = o3d3xx.ImageClient(address, 50010)

    
    grabber = GrabO3D300(camData)
    for x in range(12):
    	grabber.readNextFrame()
   	
    

if __name__ == '__main__':
    main()

