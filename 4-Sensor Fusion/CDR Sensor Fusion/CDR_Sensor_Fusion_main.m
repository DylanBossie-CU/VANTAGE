%% This script is being used to test the fusionCentroid function
%{
% Define outputs from camera
camOrigin = [0 0 0];
camVec = [1 .5 0];

% Define output from TOF
pos_TOF = [2 0.75 0];

% Define uncertainties
sig_cam = 0.01;
sig_TOF = 0.05;

% Run fusionCentroid
[pos] = fusionCentroid(camOrigin, camVec, pos_TOF, sig_cam, sig_TOF);
%}

%% Error testing

% Define truth data
FPS = 5;
tMax = 100;
t = linspace(0, tMax, tMax.*FPS)';

pos_init = [15 0 -5]./100;
V_truth = [0 0 1];
pos_truth = pos_init + V_truth.*t;


