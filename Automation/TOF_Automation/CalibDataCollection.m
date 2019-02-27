% Simultaneous Image Capture for Calibration Purposes

clear
close all
clc

%% File Definitions
% High-level directory of all TOF information. This should contain the
% executable data capture script and the desired location to save point
% cloud outputs.
tofDirName = '/home/vantage/Documents/githere/VANTAGE/Automation/TOF_Automation/examples/';
tofDirPointClouds = strcat(tofDirName,'TOF_PointClouds/');
tofFlush = strcat(tofDirPointClouds,'pointcloud*');
%tofScript = strcat(tofDirName,'TOF_Data_Capture.py');
tofScript = strcat(tofDirName,'TOF_Single_Frame_Capture.py');

opticalDirName = '/home/vantage/Documents/githere/VANTAGE/Automation/OpticalCam_Automation/';
opticalDirImages = strcat(opticalDirName,'TestImages_Automated/');
opticalFlush = strcat(opticalDirImages,'VANTAGEOp*');
opticalScript = strcat(opticalDirName,'Optical_Single_Frame.py');

% Flush all TOF point clouds detected in output directory
unix(sprintf('rm %s',tofFlush));

% Flush all optical images detected in output directory
unix(sprintf('rm %s',opticalFlush));

%% TOF Data Collection
% This command executes a bash command, which will begin data capture using
% the TOF sensor, and save point clouds to an output directory.

fprintf('beginning data capture in 3 seconds')
pause(3)

for i=1:20
    fprintf('Taking Image in 1 Second: %d\n',i)
    pause(1)
    unix(sprintf('python %s &',tofScript));

    %% Optical Camera Data Collection
    % This command executes a bash command, which will begin data capture using
    % the optical camera, and save images to an output directory.
    unix(sprintf('python %s &',opticalScript));
    
    fprintf('Commands Sent\n')
    pause(3)
    
end
