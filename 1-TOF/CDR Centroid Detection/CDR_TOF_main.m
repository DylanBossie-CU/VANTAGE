%%  Author: Joshua Kirby
%  Created: 11/20/2018
% Modified: 11/20/2018
%
% Purpose:
%
%% Housekeeping
close all;clc

%% Setup path
addpath(genpath('Subroutines'));

%% Define files to be processed
%filestrs = {'Justin Test/Test Data/Separation/DRSep_1_in.ply'};
filestrs = {'Justin Test/Simulation Data/ToF/30FPS_Tube_2_3U_2U_1U_VX_0_VY_0_VZ_0P1_WX_0_WY_0_WZ_0_SEP_0P03_F00012.pcd'};

%% Find centroids
[centroids] = TOF_centroid(filestrs,'sim');

%% Show results
figure
hold on
ptCloud = loadSimToF(filestrs{1});
h = pcshow(ptCloud);
plot3(centroids(1).pos(1),centroids(1).pos(2),centroids(1).pos(3),'k.','markersize',12)
xlabel('x')
ylabel('y')
zlabel('z')
xlim([0 0.2])
ylim([0 0.2])
zlim([0 1])
grid on
grid minor
hold off