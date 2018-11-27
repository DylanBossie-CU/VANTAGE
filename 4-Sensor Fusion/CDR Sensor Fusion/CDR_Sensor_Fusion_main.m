%% This script is being used to test the fusionCentroid function

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
for i = 1:length(camOrigin)
    [pos(i)] = fusionCentroid(camOrigin, camVec{i}, pos_TOF{i}, sig_cam, sig_TOF);
    if i > 1
        [vel] = calculateVelocity(pos,time);
    end
end