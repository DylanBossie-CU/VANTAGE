%%  Author: Joshua Kirby
%  Created: 11/20/2018
% Modified: 11/24/2018
%
% Purpose:
%
%% Housekeeping
close all;clc

%% Setup path
addpath(genpath('Subroutines'));

%% Define files to be processed
direc = 'Justin Test/Simulation Data/ToF/';
ls = dir(direc);
c = 1;
for i = 1:30
  if contains(ls(i).name,'FPS_Tube')
    filestrs{c} = [direc ls(i).name];
    c = c + 1;
  end
end

filestrs = {[direc,'30FPS_Tube_2_3U_2U_1U_VX_0_VY_0_VZ_001_WX_0_WY_0_WZ_0_SEP_0P03_F00050.pcd']};


%% Get truth data
[truth] = generateSimTruth(filestrs);

%% Initialize
SET = CDR_TOF_initialize();

%% Find centroids
[CENTROID] = TOF_centroid(SET.filestrs,SET,SET.dataType);
 
%% Show results
% For single file testing
if length(filestrs)==1
  figure
  hold on
  ptCloud = loadSimToF(filestrs{1});
  h = pcshow(ptCloud);
  for i = 1:length(filestrs)
    plot3(truth(i).pos(1,:),truth(i).pos(2,:),truth(i).pos(3,:),'b.','markersize',8)
    plot3(CENTROID(i).pos(1,:),CENTROID(i).pos(2,:),CENTROID(i).pos(3,:),'r.','markersize',8)
  end
  xlabel('x')
  ylabel('y')
  zlabel('z')
  xlim([-0.2 0.3])
  ylim([-0.2 0.3])
  grid on
  grid minor
  view(90,0)
  hold off
end
  
% For multifile testing
figure
hold on
grid on
grid minor
for i = 1:length(filestrs)
  for j = 1:length(CENTROID(i).u)
    %res(i,j) = norm(truth(i).pos(:,j)-centroids(i).pos(:,4-j));
  end
end
plot(res(:,1))
plot(res(:,2))
plot(res(:,3))
hold off








