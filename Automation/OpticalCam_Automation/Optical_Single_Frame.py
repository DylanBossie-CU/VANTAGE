from pyueye import ueye
import numpy as np
import cv2
import sys
import time

#---------------------------------------------------------------------------------------------------------------------------------------

#Variables
hCam = ueye.HIDS(0)             #0: first available camera;  1-254: The camera with the specified camera ID
sInfo = ueye.SENSORINFO()
cInfo = ueye.CAMINFO()
pcImageMemory = ueye.c_mem_p()
MemID = ueye.int()
rectAOI = ueye.IS_RECT()
pitch = ueye.INT()
nBitsPerPixel = ueye.INT(8)    #24: bits per pixel for color mode; take 8 bits per pixel for monochrome
channels = 1                    #3: channels for color mode(RGB); take 1 channel for monochrome
m_nColorMode = ueye.INT()		# Y8/RGB16/RGB24/REG32
bytes_per_pixel = int(nBitsPerPixel / 8)
#---------------------------------------------------------------------------------------------------------------------------------------
print("START")
print()


# Starts the driver and establishes the connection to the camera
nRet = ueye.is_InitCamera(hCam, None)
if nRet != ueye.IS_SUCCESS:
    print("is_InitCamera ERROR")

# Reads out the data hard-coded in the non-volatile camera memory and writes it to the data structure that cInfo points to
nRet = ueye.is_GetCameraInfo(hCam, cInfo)
if nRet != ueye.IS_SUCCESS:
    print("is_GetCameraInfo ERROR")

# You can query additional information about the sensor type used in the camera
nRet = ueye.is_GetSensorInfo(hCam, sInfo)
if nRet != ueye.IS_SUCCESS:
    print("is_GetSensorInfo ERROR")

nRet = ueye.is_ResetToDefault( hCam)
if nRet != ueye.IS_SUCCESS:
    print("is_ResetToDefault ERROR")

# Set display mode to DIB
nRet = ueye.is_SetDisplayMode(hCam, ueye.IS_SET_DM_DIB)


m_nColorMode = ueye.IS_CM_MONO8
nBitsPerPixel = ueye.INT(8)
bytes_per_pixel = int(nBitsPerPixel / 8)
print("else")

# Can be used to set the size and position of an "area of interest"(AOI) within an image
nRet = ueye.is_AOI(hCam, ueye.IS_AOI_IMAGE_GET_AOI, rectAOI, ueye.sizeof(rectAOI))
if nRet != ueye.IS_SUCCESS:
    print("is_AOI ERROR")

width = rectAOI.s32Width
height = rectAOI.s32Height

# Prints out some information about the camera and the sensor
print("Camera model:\t\t", sInfo.strSensorName.decode('utf-8'))
print("Camera serial no.:\t", cInfo.SerNo.decode('utf-8'))
print("Maximum image width:\t", width)
print("Maximum image height:\t", height)
print()

#---------------------------------------------------------------------------------------------------------------------------------------

# Allocates an image memory for an image having its dimensions defined by width and height and its color depth defined by nBitsPerPixel
nRet = ueye.is_AllocImageMem(hCam, width, height, nBitsPerPixel, pcImageMemory, MemID)
if nRet != ueye.IS_SUCCESS:
    print("is_AllocImageMem ERROR")
else:
    # Makes the specified image memory the active memory
    nRet = ueye.is_SetImageMem(hCam, pcImageMemory, MemID)
    if nRet != ueye.IS_SUCCESS:
        print("is_SetImageMem ERROR")
    else:
        # Set the desired color mode
        nRet = ueye.is_SetColorMode(hCam, m_nColorMode)



# Activates the camera's live video mode (free run mode)

# Enables the queue mode for existing image memory sequences
nRet = ueye.is_InquireImageMem(hCam, pcImageMemory, MemID, width, height, nBitsPerPixel, pitch)
if nRet != ueye.IS_SUCCESS:
    print("is_InquireImageMem ERROR")
else:
    print("Press q to leave the program")




a=ueye.DOUBLE(30)
c=ueye.DOUBLE()
b=ueye.UINT(8)
thing=ueye.is_Exposure(hCam,ueye.IS_EXPOSURE_CMD_SET_EXPOSURE, a,b )
thing=ueye.is_Exposure(hCam,ueye.IS_EXPOSURE_CMD_GET_EXPOSURE, c,b )
print(c)
#---------------------------------------------------------------------------------------------------------------------------------------
nRet = ueye.is_FreezeVideo(hCam, ueye.IS_WAIT)
Iinfo=ueye.UEYEIMAGEINFO()
count=0


while(nRet == ueye.IS_SUCCESS):

    
    # In order to display the image in an OpenCV window we need to...
    # ...extract the data of our image memory
  

	nRet = ueye.is_FreezeVideo(hCam, ueye.IS_WAIT)
	array = ueye.get_data(pcImageMemory, width, height, nBitsPerPixel, pitch, copy=False)
	nRet1= ueye.is_GetImageInfo(hCam,MemID, Iinfo, ueye.sizeof(Iinfo) )
	frame = np.reshape(array,(height.value, width.value, bytes_per_pixel))
	#print(Iinfo.TimestampSystem)
	TitleString="/home/vantage/Documents/githere/VANTAGE/Automation/OpticalCam_Automation/TestImages_Automated/VANTAGEOp"+str(Iinfo.TimestampSystem.wMonth.value)+"_"+str(Iinfo.TimestampSystem.wDay.value)+"_"+str(Iinfo.TimestampSystem.wHour.value)+"_"+str(Iinfo.TimestampSystem.wMinute.value)+"_"+str(Iinfo.TimestampSystem.wSecond.value)+"_"+str(Iinfo.TimestampSystem.wMilliseconds.value)+".jpg"
	cv2.imwrite(TitleString, frame)
	#print("Wrote Image")
	count+=1
	
	if count>0:
		break



    # ...resize the image by a half
    #frame2 = cv2.resize(frame,(0,0),fx=0.3, fy=0.3)
    
#---------------------------------------------------------------------------------------------------------------------------------------
    #Include image data processing here

#---------------------------------------------------------------------------------------------------------------------------------------

    #...and finally display it
    #cv2.imshow("SimpleLive_Python_uEye_OpenCV", frame2)

    # Press q if you want to end the loop
   


ueye.is_FreeImageMem(hCam, pcImageMemory, MemID)

# Disables the hCam camera handle and releases the data structures and memory areas taken up by the uEye camera
ueye.is_ExitCamera(hCam)

# Destroys the OpenCv windows
cv2.destroyAllWindows()

print()
print("END")