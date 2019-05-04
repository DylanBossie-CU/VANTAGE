clc
close all
clear

%% File Definitions
% High-level directory of all TOF information. This should contain the
% executable data capture script and the desired location to save point
% cloud outputs.
tofDirName = 'Data/Simulation/Simulation_4_15_085/TOF/';
tofDirPointClouds = strcat(tofDirName,'Sample2/');

% Grab all point cloud files in the defined output directory.
PointClouds = dir([tofDirPointClouds 'VTube*']);

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

        pcshow(testFile)
        colormap('parula')
        view(az,el);
        drawnow
    end
end