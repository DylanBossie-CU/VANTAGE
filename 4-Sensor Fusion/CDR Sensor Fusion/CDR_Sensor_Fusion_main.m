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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Error Testing %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Problem definition
% Define geometry
camOrigin = [0 0 0];
pos_init = [15 0 -5]./100;
V_truth = [0 0 1];

% Define sensor parameters
FPS_TOF = 25;
dMin_TOF = 0.25;
dMax_TOF = 3;

FPS_cam = 5;
dMin_cam = dMax_TOF;
dMax_cam = 100;

% Define error parameters
load('CleanError.mat')
errFunc_TOF = @(d) interp1(ztbestestimate, totalErrorT, d);
errPercent_TOF = 0.01;
errX_TOF = 1;
errY_TOF = 1;
errZ_TOF = 1;
errDir_TOF = [errX_TOF, errY_TOF, errZ_TOF];

errPercent_cam = 0.01;
errX_cam = 1;
errY_cam = 1;
errZ_cam = 0;
errDir_cam = [errX_cam, errY_cam, errZ_cam];

%% Creation of truth data
% Define truth data for TOF
t_TOF = (0:1/FPS_TOF:dMax_TOF/norm(V_truth))';
pos_truth_TOF = pos_init + V_truth.*t_TOF;

I = sqrt(sum(pos_truth_TOF.^2,2)) < dMin_TOF;
t_TOF = t_TOF(~I);
pos_truth_TOF = pos_truth_TOF(~I,:);
numFrame_TOF = numel(t_TOF);

% Create truth data for camera
t_cam = (0:1/FPS_cam:dMax_cam/norm(V_truth))';
pos_truth_cam = pos_init + V_truth.*t_cam;

I = sqrt(sum(pos_truth_cam.^2,2)) < dMin_cam;
t_cam = t_cam(~I);
pos_truth_cam = pos_truth_cam(~I,:);
numFrame_cam = numel(t_cam);

%% Monte Carlo
N = 5000;
errMag_TOF = errFunc_TOF(sqrt(sum(pos_truth_TOF,2)));
errMag_cam = sqrt(sum(pos_truth_cam,2))*errPercent_cam;

vBarErr_prop = zeros(N,1);
vBarErr_fusion = zeros(N,1);

posErr_prop = zeros(N,numFrame_cam);
posErr_fusion = zeros(N,numFrame_cam);
posErr_TOF = zeros(N,numFrame_TOF);
%vFitErr_TOF = zeros(N,1);
%vFitErr_fusion = zeros(N,1);
for j = 1:N
    %% Creation of simulation data (with error) in Monte Carlo
    % Create TOF data with error
    errMagVec = (rand(numFrame_TOF,1)-0.5).*2.*errMag_TOF;
    errDirVec = (rand(numFrame_TOF,3)-0.5).*2.*errDir_TOF;
    errDirVec = errDirVec./sqrt(errDirVec(:,1).^2 + errDirVec(:,2).^2 + errDirVec(:,3).^2);
    errVec = errDirVec.*errMagVec;
    pos_TOF = pos_truth_TOF + errVec;

    % Create camera data with error
    errMagVec = (rand(numFrame_cam,1)-0.5).*2.*errMag_cam;
    errDirVec = (rand(numFrame_cam,3)-0.5).*2.*errDir_cam;
    errDirVec = errDirVec./sqrt(errDirVec(:,1).^2 + errDirVec(:,2).^2 + errDirVec(:,3).^2);
    errVec = errDirVec.*errMagVec;
    pos_cam = pos_truth_cam + errVec;

    %% Create propogated TOF data
    V_TOF = mean(diff(pos_TOF)./diff(t_TOF),1);
    pos_preProp = pos_init + t_TOF.*V_TOF;
    posErr_TOF(j,:) = sqrt(sum((pos_preProp - pos_truth_TOF).^2,2))';
    pos_prop = pos_init + t_cam(1).*V_TOF + (t_cam-t_cam(1)).*V_TOF;
    errMag_prop = errFunc_TOF(sqrt(sum(pos_prop,2)));

    %% Perform sensor fusion
    pos_fusion = zeros(numFrame_cam,3);
    benchTime = zeros(numFrame_cam,1);
    for i = 1:numFrame_cam
        tic
        pos_fusion(i,:) = fusionCentroid(camOrigin, pos_cam(i,:), pos_prop(i,:), errMag_cam(i), errMag_prop(i), pos_truth_cam(i,:));
        benchTime(i) = toc;
    end

    % Calculate raw velocity for different estimates
    vel_TOF = sqrt(sum(diff(pos_prop).^2,2))./diff(t_cam);
    vel_fusion = sqrt(sum(diff(pos_fusion).^2,2))./diff(t_cam);

    % Calculate average of raw velocities for each cartesian direction
    vBar_TOF = mean(diff(pos_prop,1)./diff(t_cam),1);
    vBar_fusion = mean(diff(pos_fusion,1)./diff(t_cam),1);

    % Velocity fitting
    %vFit_TOF = ((pos_prop - pos_TOF(end))\t_cam)';
    %vFit_fusion = ((pos_fusion - pos_TOF(end))\t_cam)';

    % Raw Position error results
    err_TOF = pos_prop - pos_truth_cam; err_TOF = sqrt(sum(err_TOF.^2,2));
    err_fusion = pos_fusion - pos_truth_cam; err_fusion = sqrt(sum(err_fusion.^2,2));
    
    posErr_prop(j,:) = err_TOF';
    posErr_fusion(j,:) = err_fusion';

    % Raw Velocity error results
    vErr_TOF = abs(vel_TOF - norm(V_truth));
    vErr_fusion = abs(vel_fusion - norm(V_truth));

    % Magnitude velocity error
    vBarErr_prop(j) = norm(vBar_TOF-V_truth).*100;
    vBarErr_fusion(j) = norm(vBar_fusion-V_truth).*100;

    %vFitErr_TOF(j) = norm(vFit_TOF-V_truth).*100;
    %vFitErr_fusion(j) = norm(vFit_fusion-V_truth).*100;
end
% Printing
%{
fprintf('Actual Velocity:      [%9.6f %9.6f %9.6f] m/s\n',V_truth(1),V_truth(2),V_truth(3))
fprintf('TOF Mean Velocity:    [%9.6f %9.6f %9.6f] m/s\n',vBar_TOF(1),vBar_TOF(2),vBar_TOF(3))
fprintf('Fusion Mean Velocity: [%9.6f %9.6f %9.6f] m/s\n',vBar_fusion(1),vBar_fusion(2),vBar_fusion(3))
fprintf('TOF Fit Velocity:     [%9.6f %9.6f %9.6f] m/s\n',vFit_TOF(1),vFit_TOF(2),vFit_TOF(3))
fprintf('Fusion Fit Velocity:  [%9.6f %9.6f %9.6f] m/s\n',vFit_fusion(1),vFit_fusion(2),vFit_fusion(3))
fprintf('TOF Mean Error:        %9.6f cm/s\n',vBarErr_TOF)
fprintf('Fusion Mean Error:     %9.6f cm/s\n',vBarErr_fusion)
fprintf('TOF Fit Error:         %9.6f cm/s\n',vFitErr_TOF)
fprintf('Fusion Fit Error:      %9.6f cm/s\n',vFitErr_fusion)
%}

% Plotting
lw = 1.5;
figure
nbins = 100;
reqV = 1; % (cm/s)
histogram(vBarErr_prop,nbins,'Normalization','cdf','DisplayStyle','bar','EdgeColor','none')
hold on
histogram(vBarErr_fusion,nbins,'Normalization','cdf','DisplayStyle','bar','EdgeColor','none')
plot([reqV, reqV], [0, 1],'LineWidth',5)
title('Effect of Sensor Fusion on Velocity Error')
xlabel('Magnitude velocity error (cm/s)')
ylabel('Cumulative probability')
ylim([0 1])
xlim([0 reqV.*1.1])
legend('TOF alone','Sensor fusion','Requirement','Location','best')

figure
dTmp1 = sqrt(sum(pos_truth_cam.^2,2));
dTmp2 = sqrt(sum(pos_truth_TOF.^2,2));
loglog([dTmp1;dTmp2],[max(posErr_prop), max(posErr_TOF)],'LineWidth',lw)
hold on
loglog(dTmp1,max(posErr_fusion),'LineWidth',lw)

nTmp = 500;
dTmp = linspace(0, 100, nTmp);
reqPos = zeros(1,nTmp);
reqPos(dTmp<10) = 10;
reqPos(dTmp>10) = dTmp(dTmp>10).*10;
loglog(dTmp,reqPos,'LineWidth',lw)
xlabel('Range (m)')
ylabel('Relative position error (cm)')
axis tight
grid on
title('Maximum effect of Sensor Fusion on Position Error')
legend('TOF alone','Sensor fusion','Requirement','Location','best')

figure
plot([dTmp1;dTmp2],[mean(posErr_prop,1), mean(posErr_TOF,1)],'LineWidth',lw)
hold on
plot(dTmp1,mean(posErr_fusion,1),'LineWidth',lw)
xlabel('Range (m)')
ylabel('Mean relative position error (cm)')
axis tight
grid on
title('Average effect of Sensor Fusion on Position Error')
legend('TOF alone','Sensor fusion','Location','best')

figure
plot([dTmp1;dTmp2],[max(posErr_prop), max(posErr_TOF)],'LineWidth',lw)
hold on
plot(dTmp1,max(posErr_fusion),'LineWidth',lw)
xlabel('Range (m)')
ylabel('Max relative position error (cm)')
axis tight
grid on
title('Maximum effect of Sensor Fusion on Position Error')
legend('TOF alone','Sensor fusion','Location','best')
%{
figure
plot(sqrt(sum(pos_truth.^2,2)),err_TOF.*100)
hold on
plot(sqrt(sum(pos_truth.^2,2)),err_fusion.*100)
xlabel('Downrange Distance (m)')
ylabel('Magnitude error (cm)')
title('Position Error of Sensor Fusion')
legend('TOF','Fusion','Location','Best')
%
figure
plot(sqrt(sum(pos_truth(2:end,:).^2,2)),vErr_TOF.*100)
hold on
plot(sqrt(sum(pos_truth(2:end,:).^2,2)),vErr_fusion.*100)
xlabel('Downrange Distance (m)')
ylabel('Magnitude error (cm/s)')
title('Velocity Error of Sensor Fusion')
legend('TOF','Fusion','Location','Best')
%
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
plot3(camOrigin(1), camOrigin(2), camOrigin(3),'k*')
plot3(pos_init(1), pos_init(2), pos_init(3),'r*')
xlabel('x (m)'); ylabel('y (m)'); zlabel('z (m)')
legend('True position','TOF position','Camera position','Fusion position','Camera Origin','Launch position','Location','best')
grid on
%}
