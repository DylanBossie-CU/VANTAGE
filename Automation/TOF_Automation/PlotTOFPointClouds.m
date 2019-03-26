clc
close all
clear

%% File Definitions
% High-level directory of all TOF information. This should contain the
% executable data capture script and the desired location to save point
% cloud outputs.
tofDirName = '/home/vantage/Documents/githere/VANTAGE/Data/';
tofDirPointClouds = strcat(tofDirName,'TOF_Automated/3_25_TOF/Test1/');

% Grab all point cloud files in the defined output directory.
PointClouds = dir(tofDirPointClouds);

%% TOF Plotting Parameters
% Azimuth, Elevation - These parameters automatically adjust the view of
% the generated plots to be looking outwards, as if you are the TOF camera.
% For this to be accurate, have the TOF take images upright (The LED lights
% on the back, 2 green, 2 orange, should on top).
az = 0;
el = 270-180;

%% Plot Point Clouds
% This for loop iterates each detected point cloud in the defined directory
% above, and sequentially generates each in a single figure.
for ii = 1:length(PointClouds)
    
    fname = PointClouds(ii).name;
    isDefFName = strcmp(fname, '.') || strcmp(fname, '..');
    
    if ~isDefFName
        fpath = fullfile(tofDirPointClouds, fname);
        testFile = pcread(fpath);
        
        X = testFile.Location(:,1);
        Y = testFile.Location(:,2);
        Z = testFile.Location(:,3);
        
        scatter3(X,Y,Z,1)
        view(az,el);
        drawnow
        %pcshow(testFile)
        %view(az,el);
        %drawnow
    end
end