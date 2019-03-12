clear
close all
clc

%% File Definitions
% High-level directory of all TOF information. This should contain the
% executable data capture script and the desired location to save point
% cloud outputs.
tofDirName = '/home/vantage/Documents/githere/VANTAGE/Automation/TOF_Automation/examples/';
tofDirData = '/home/vantage/Documents/githere/VANTAGE/Data/AutomatedTOFCapture/';
tofFlush = strcat(tofDirData,'pointcloud*');
tofScript = strcat(tofDirName,'TOF_Data_Capture.py');

opticalDirName = '/home/vantage/Documents/githere/VANTAGE/Automation/OpticalCam_Automation/';
opticalDirImages = '/home/vantage/Documents/githere/VANTAGE/Data/AutomatedImageCapture/';
opticalDirData = '/home/vantage/Documents/githere/VANTAGE/Data/';
opticalFlush = strcat(opticalDirData,'VANTAGEOp*');
opticalScript = strcat(opticalDirName,'Optical_Data_Capture.py');

% Flush all TOF point clouds detected in output directory
unix(sprintf('rm %s',tofFlush));

% Flush all optical images detected in output directory
unix(sprintf('rm %s',opticalFlush));


%% TOF Data Collection
% This command executes a bash command, which will begin data capture using
% the TOF sensor, and save point clouds to an output directory.
unix(sprintf('python %s &',tofScript));

%% Optical Camera Data Collection
% This command executes a bash command, which will begin data capture using
% the optical camera, and save images to an output directory.
unix(sprintf('python %s &',opticalScript));