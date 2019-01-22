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

%% Initialize
SET = CDR_TOF_initialize();

%% Find centroids
[CENTROID] = TOF_centroid(SET.filestrs,SET,SET.dataType);
 
%% Show results
% For single file testing
if length(SET.filestrs)==1
  figure
  hold on
  ptCloud = loadSimToF(SET.filestrs{1});
  h = pcshow(ptCloud,'markersize',25);
  for i = 1:1%length(SET.filestrs)
    if strcmpi(SET.dataType,'sim')
      plot3(SET.TRUTH(i).pos(1,:),SET.TRUTH(i).pos(2,:),SET.TRUTH(i).pos(3,:),'b.','markersize',8)
    end
    plot3(CENTROID(i).pos(1,:),CENTROID(i).pos(2,:),CENTROID(i).pos(3,:),'r.','markersize',8)
  end
  %plot3(0,0,0,'k.','markersize',10)
  xlabel('x')
  ylabel('y')
  zlabel('z')
  axis equal
  title('TOF CubeSat Centroiding')
  legend('TOF Point Cloud','True Centroids','Calculated Centroids','location','east')
  grid on
  grid minor
  hold off
end
  
% % For multifile testing
if strcmpi(SET.dataType,'sim')
  figure
  hold on
  grid on
  grid minor
  for i = 1:length(SET.filestrs)
    for j = 1:1%length(CENTROID(i).u)
      res(i,j) = norm(SET.TRUTH(i).pos(:,j)-CENTROID(i).pos(:,j));
    end
  end
  % plot(res(:,1))
  % plot(res(:,2))
  % plot(res(:,3))
  hold off
end








