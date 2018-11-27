%% This script is being used to test the fusionCentroid function
clc
close all
clear
%{
% Define outputs from camera
camOrigin = [0 0 0];
camVec = [1 2 0];

% Define output from TOF
pos_TOF = {[1 0.75 0],[1 1.9 0],[1 2.4 0]};

% Define timestamps
time = [1 1.2 1.3];

% Define uncertainties
sig_cam = 0.01;
sig_TOF = 0.02;

% Run fusionCentroid
[pos] = fusionCentroid(camOrigin, camVec, pos_TOF, sig_cam, sig_TOF, [2 0.25 0]);
%}

%% Error testing
%
% Define geometry
camOrigin = [0 0 0];
pos_init = [15 0 -5]./100;

% Define truth data
FPS = 5;
tMax = 100;
numFrame = tMax*FPS;
t = linspace(0, tMax, numFrame)';

V_truth = [0 0 1];
pos_truth = pos_init + V_truth.*t;

% Define error parameters
errMag_TOF = sqrt(sum(pos_truth,2))*0.01;
errX_TOF = 10;
errY_TOF = 10;
errZ_TOF = 2;
errDir_TOF = [errX_TOF, errY_TOF, errZ_TOF];

errMag_cam = sqrt(sum(pos_truth,2))*0.01;
errX_cam = 2;
errY_cam = 2;
errZ_cam = 10;
errDir_cam = [errX_cam, errY_cam, errZ_cam];

% Create TOF data with error
errMagVec = (rand(numFrame,1)-0.5).*2.*errMag_TOF;

errDirVec = rand(numFrame,3).*errDir_TOF;
errDirVec = errDirVec./sqrt(errDirVec(:,1).^2 + errDirVec(:,2).^2 + errDirVec(:,3).^2);
errVec = errDirVec.*errMagVec;
pos_TOF = pos_truth + errVec;

% Create camera data with error
errMagVec = (rand(numFrame,1)-0.5).*2.*errMag_cam;

errDirVec = rand(numFrame,3).*errDir_cam;
errDirVec = errDirVec./sqrt(errDirVec(:,1).^2 + errDirVec(:,2).^2 + errDirVec(:,3).^2);
errVec = errDirVec.*errMagVec;
pos_cam = pos_truth + errVec;

% Perform sensor fusion
pos_fusion = zeros(numFrame,3);
benchTime = zeros(numFrame,1);
for i = 1:numFrame
    tic
    pos_fusion(i,:) = fusionCentroid(camOrigin, pos_cam(i,:), pos_TOF(i,:), errMag_cam(i), errMag_TOF(i), pos_truth(i,:));
    benchTime(i) = toc;
end

% Calculate velocity for different estimates
vel_TOF = sqrt(sum(diff(pos_TOF).^2,2))./diff(t);
vel_fusion = sqrt(sum(diff(pos_fusion).^2,2))./diff(t);

% Calculate average velocities
vBar_TOF = mean(vel_TOF);
vBar_fusion = mean(vel_fusion);

% Position error results
err_TOF = pos_TOF - pos_truth; err_TOF = sqrt(sum(err_TOF.^2,2));
err_fusion = pos_fusion - pos_truth; err_fusion = sqrt(sum(err_fusion.^2,2));

% Velocity error results
vErr_TOF = abs(vel_TOF - norm(V_truth));
vErr_fusion = abs(vel_fusion - norm(V_truth));

% Plotting
figure
plot(err_TOF)
hold on
plot(err_fusion)
xlabel('Frame')
ylabel('Magnitude error (m)')
title('Position Error of Sensor Fusion')
legend('TOF','Fusion','Location','Best')

figure
plot(vErr_TOF)
hold on
plot(vErr_fusion)
xlabel('Frame')
ylabel('Magnitude error (m/s)')
title('Velocity Error of Sensor Fusion')
legend('TOF','Fusion','Location','Best')
%{
figure
loglog(benchTime)
xlabel('Frame')
ylabel('Runtime (s)')
title('Sensor Fusion Runtime')
%
figure
plot3(pos_truth(:,1),pos_truth(:,2),pos_truth(:,3),'r.')
hold on
plot3(pos_TOF(:,1),pos_TOF(:,2),pos_TOF(:,3),'b.')
plot3(pos_cam(:,1),pos_cam(:,2),pos_cam(:,3),'g.')
plot3(pos_fusion(:,1),pos_fusion(:,2),pos_fusion(:,3),'k.')


%}
