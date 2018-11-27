%% This script is being used to test the fusionCentroid function
clc
close all
clear
% Define outputs from camera
camOrigin = [0 0 0];
camVec = {[1 .5 0],[1 1 0]};

% Define output from TOF
pos_TOF = {[1 0.75 0],[1 1.9 0]};

% Define timestamps
time = [1 2];

% Define uncertainties
sig_cam = 0.01;
sig_TOF = 0.05;

% Run fusionCentroid
pos = zeros(length(time),3);
for i = 1:length(time)
    [pos(i,:)] = fusionCentroid(camOrigin, camVec{i}, pos_TOF{i}, sig_cam, sig_TOF);
    if i > 1
        [vel] = calculateVelocity(pos,time);
    end
end