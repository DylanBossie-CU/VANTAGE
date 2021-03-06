# VANTAGE
## General Comments
This codebase is based around the test types used for the 2018-2019 development cycle. These included three types:
1. Simulation Testing
The simulation test was created using a combination of Cinema4D and Blensor, for the purpose of simulation the optical and TOF cameras, respectively. This suite ([VANTAGE_Simulation](https://github.com/vantagecu/VANTAGE_Simulation)) can be found in the [VANTAGE organization](https://github.com/vantagecu) repo. The truth data for this test is generated by the Cinema4D program itself.

2. Modular Testing
The modular test was developed for the purpose of testing the VANTAGE system from 0-10m. This test was performed in the CU RECUV lab, with truth data acquired using a system of VICON cameras.

3. 100m Testing
The 100m test was developed for the purpose of testing the system across the full 0-100m required range. This test was performed at the Boulder airport, using a test rig based on attaching CubeSats to a car, and acquiring truth data by affixing a GPS Trimble system to the car.

### Running the Software Suite
VANTAGE has a main script for running the entire software suite, as well as test cases for utilizing only the post-processing portion of the code.
#### VANTAGE_main.m
Running VANTAGE_main executes everything involved in the full system, from transmitting a Manifest.json file to VANTAGE, executing data capture, and performing post-processing. See below for a description of the avionics, communications, and how to configure the first portion of the code for a different avionics setup.

#### Post-processing
To perform only post-processing on an existing data set, utilize +VANTAGE/+Test/+PostProcessing/Test_Fusion.m . This file utilizes a testing framework to attempt post-processing on a given data set. To configure this test case, modify the configDirecNames in the test case constructor to point to the config directories of the corresponding test types. Additionally modify *testType* to define the type of the three tests that you wish to process. This can be set to any of the cases in the switch statement below.

When run, the program will detect the relevant files based on the configuration file, and sequentially process the TOF data, then the optical data. See the UML diagrams in the VANTAGE PFR data package for specifics on the workings of the software algorithms.

### Data Directory
The organization of the data directory (*VANTAGE/Data/*) is crucial for the successful operation of the VANTAGE software suite. The majority of errors that will be encountered when running the suite will be due to incorrectly configured data folders. The data directory is currently configured to handle the three types of tests used in the 2018-2019 development cycle. To ensure the correct configuration of the data:

Create a folder *Data/* within the main VANTAGE folder. This is excluded by .git in the .gitignore, so all data to be processed will downloaded and stored individually on your local machine. When the software suite has been fully set up and run, the Data directory will have the following folders:
Simulation/
truthMats/
Results/
ErrorOut/
ModularTest_4_9/
3_25_100m/

These should contain the following:
#### Simulation/
The simulation folder will have subfolders describing the type, date, and speed of the test. An example of this is: *Simulation_4_15_030*, which represents a test taken on April 15, at 0.30m/s. Within each of these subdirectories should be two folders, *Optical/* and *TOF/*. It will also have a *truth.json* file, and a *truth_corrected.json*, which is the truth file VANTAGE will process. In the case of the simulation, optical data should have naming conventions of a prefix *VTube*, and a timestamp suffix. If using the VANTAGE simulation software suite, this convention will automaticall be generated in the output. The TOF data should have multiple samples (taken with randomized noise), so it will have subdirectories of convention *Sample*. These files will then be prefixed with *Sample*, and suffixed with a timestamp and frame stamp.

#### ModularTest_4_9/ & 3_25_100m/
This folder should have subfolders corresponding to a test number taken on that day (e.g. *Test5/*). This will then follow the general structure of a folder *Optical/*, *TOF/*, *truth.json*, and *truth_corrected.json*.

Optical images within the *Optical/* folder should be defined by the format *VANTAGEOPYYYYM_DD_HH_MM_SS_mmm.png*
TOF pointclouds within the *TOF/* folder should be defined by the format *pointcloud_YYYY_MM_DD_HH_MM_SS.mmmmmm.pcd*


## Configs
There are folders corresponding to each of the three tests used by VANTAGE, configured as *3_25_100m*, *ModularTest_4_9*, and *Simulation*. Within each of these folders will be subdirectories corresponding to individual tests run on those dates.
1. Modular
Sample folder: *Test5/*
2. 100m
Sample folder: *Test5/*
3. Simulation
Sample folder: *_030/Sample1* (for 0.30m/s)
Each of these folders will then contain configuration files, as described below:
### Deployer.json

VANTAGE_ROT: [Yaw, Pitch, Roll] euler angles of the deployer from its initial configuration
[rad]

PHI, XI, NABLA, RHO, P_OPTICAL, D_TOF: Sensor location parameterization variables. After moving the deployer origin to the correct position in the global coordinate frame, then move the deployer origin from the centerline to the VCF origin according to these parameters
defined in 13-5
[cm]

MU, BETA, SIGMA, PSI, GAMMA, L: Deployer parameterization variables
defined in 13-5
[cm]


TUBE_ORIGIN_COEFFS: Deployer tube origins are relative to deployer origin. Each tube contains the coefficients of the deployer parameterization variables needed to locate the tube origin. The order of the variables in each coefficient array is given as:

x ~ [PSI, GAMMA]

y ~ [MU]

z ~ [L]

CAMERA_LOC: camera location in X,Y,Z relative to the VANTAGE frame's origin
[cm]

CAMERA_ROT: camera rotation H,P,B
[rad]

FOCAL_LENGTH: Focal length in [mm]

APERTURE: Aperture [mm] - this is the physical sensor diameter (for a rectangular sensor)

H_FOV: horizontal field of view [deg]

V_FOV: vertical field of view [deg]

X_Res: horizontal resolution [Pixel]

Y_Res: vertical resolution [Pixel]

Focus: Focus position (where you actually focus) [mm]

### Manifest.json
This is a sample of the file that would be delivered to the VANTAGE system by an operator of the NanoRacks deployer. It contains descriptions of the cubesats to be deployed as well as deployment geometry. It also contains the expected release time of the cubesats. Additionally, it should contain a filepath to the truth data json and the test scenario.

DeploymentTube: tube number from which cubesats are expected to deploy.

VantageTube: tube that is occupied by VANTAGE system.

expectedRelease: datetime array containing expected release time.

CubesatArray: array containing data for each cubesat to be deployed.

  name: cubesat name.
  
  rangeOrder: order in which this cubesat will exit the tube.
  
  expectedU: expected cubesat size in U.
  
  actualDims: the actual dimensions of the cubesat in meters.
  
TruthDataFile: filepath to truth data file (.json).

testScenario: test scenario being run ('modular', '100m', or 'simulation')

### Optical.json
OpticalData: Directory pointing to optical images for current test

OpticalFileExtension: Prefix for optical image files

DesiredFPS: Deprecated variable for processing at a specific FPS other than sensor parameter

PlotBinarizedImages: Boolean for plotting images after adaptive binarization

PlotCentroids: Boolean for plotting centroids of objects on binarized images

PlotHist: Boolean for plotting visual magnitude histogram of current image

PlotAny: Boolean for setting on/off for plotting anything in optical processing

PlotCubeSats: Boolean for plotting grayscale image

PlotIBoundaries: Boolean for plotting boundaries on grayscale image

RunOcclusion: Boolean for attempting optical occlusion processing

PerformDataCleaning: Boolean for setting data to 0 manually as defined in 'Clean100m' and 'CleanModular'

Clean100m: Pixel regions for manual data processing (100m test)

CleanModular: Pixel regions for manual data processing (Modular test)
### Sensors.json
TOFData: Directory pointing to pointclouds for current test

OpticalFocalLength: Focal length of optical camera (m)

OpticalPixelSize: Pixel width of optical camera (m)

OpticalResolution: (width,height) (pixels)

### TOF.json
maxAllowableRange: Range at which to stop using data from the TOF camera (m)

ptMaxDistanceFromPlane: Maximum distance of any given point from a plane to be considered part of the plane (m)

fps: Nominal fps of the TOF camera

HVfov_deg: [horizontal-TOF-field-of-view vertical-TOF-field-of-view] (deg)

### Transform.json
V_TCF2VCF: 3D vector from origin of TOF Cartesian Frame (TCF) to VANTAGE Cartesian Frame (VCF)

VT_DCM: 3x3 orthonormal transform matrix from TCF to VCF such that v_VCF = VT_DCM * v_TCF

V_CCF2VCF: 3D vector from origin of Camera Cartesian Frame (CCF) to VANTAGE Cartesian Frame (VCF)

VC_DCM: 3x3 orthonormal transform matrix from CCF to VCF such that v_VCF = VC_DCM * v_CCF

### Validate.json
CorrelateTruthData: Whether to perform truth data corrections/timestamp alignment
ComputingResults: Deprecated - whether to perform results processing

## Tools
Miscellaneous tools can be found in the [MiscTools](https://github.com/DylanBossie-CU/VANTAGE/tree/master/MiscTools) directory.
### tofCropping

### PlotCubeSats
This tool is used to generate 3D scatter plots of the first CubeSat launched for a given test type. Output figures will then be saved to *FigureOut/*.
The script is currently configured to plot for three specific tests:
1. Simulation Test (4-15 0.85m/s)
2. Modular Test (4-9)
3. 100m Test (3-25)

To configure which test is run, modify *testType (line 4)* to match one of the switch cases.

### truthCorrections_matGenerator
This tool is used to update the output *dataStruct* truth variable created in *Validate.m (line 467)* to .mat files to cooperate with the *VANTAGE.masterPlotter()* function. It will search for each of the three types of test cases in the *VANTAGE/Data/* directory, and create a *dataStruct* .mat variable which contains truth data for later processing.
To utilize this script, define the correct data directories in the beginning of the file which points to the location of the corrected truth data files.

### PlotTOFPointClouds
This uses the built in *pcshow()* function in MATLAB to display raw pointcloud files.
To utilize this script, define a directory which contains point clouds, and define the correct suffix in the *PointClouds* variable in line 13.

### RewriteDataStructVelocity
This script reads dataStruct output files, and transposes the velocity vector as needed to be processed in *masterPlotter()*.

### TimeManager
This uses date vectors and date strings to extract the time from a given TOF or Optical image filename and convert it to a global internal VANTAGE Time (number of seconds past first TOF file time), using the method *VantageTime()*.  The method *VantageTime2DateStr()* can be used to convert VANTAGE Times to consistent human-readable date strings.

### TOFProcessing
The TOF class's method, *TOFProcessing()* is a one-stop-shop for running the entire TOF suite.  In its default configuration it receives *SensorData* which contains information about where TOF point cloud files are housed as well as a *Deployer* instance.  It will proceed to identify CubeSats and their centroids in each point cloud, associate the centroids from frame to frame, and finally save all centroids to their respective *CubeSat*s.  It also produces *TOFfit_VCF*, which is used by the *CubeSat* method *evalTofFit()* to evaluate the TOF's prediction of a *CubeSat*'s position at a given time.

## Automation
Contains tools for getting data from sensors and communicating with Raspberry Pi for remote boot.
### OpticalCam_Automation
Contains three tools for the optical camera. Important settings to mess with here: exposure time(associated with the ueye.is_Exposure command) and the sensor gain (for brighter images with more noise, is_SetHardwareGain command)
#### Optical_Data_Capture.py
This file is used for data collection during testing. Set up to take many images at a specified interval.

#### Optical_Single_Frame.py
This file is used for capturing single frames, primarily for use in performing sensor calibrations when data needs to be taken on command. This script is called from the calibration Matlab script.

#### SimpleLive_Pyueye_OpenCV.py
Used to see a live view of the optical camera output. Useful for performing alignments, can also be useful for camera calibrations and for showing off at Symposium/Expo



### TOF_Automation
This directory contains all of the drivers and stuff for ToF. All of the scripts you need to run are in the \examples subdirectory
Listed below are the important ones.

#### TOF_Data_Capture.py
Main testing script, takes a certain number of frames.

#### image_viewer_orig.py
Live views of the ToF camera.

#### TOF_Single_Frame_Capture.py
Captures Single frame of the ToF camera's "intensity" image. This is like a normal optical image that can be used for calibration.

#### create_application.py
The ToF camera runs on an application where certain parameters are set. See the ToF manual to understand what all the parameters mean.


### PiComms

### main.m
Main wrapper without the automation or post processing code. The MAC Address that the sockets are opened in is the address of the external computer that VANTAGE is connected to, or the Raspberry Pi if the Raspberry Pi is still desired.

### socket_vantage.py
The code running on the Raspberry Pi. If the code is ran on the external computer that simulates NanoRacks, the Pi can be bypassed completely. Synchronizes with the main script, sends a manifest and receives the results from Vantage. MAC address the sockets are bound to is the address of the Ethernet port of the external computer.
