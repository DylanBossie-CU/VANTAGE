%%  Author: Justin Fay and Joshua Kirby
%  Created: 
% Modified: 11/21/2018
%
% Purpose:
%
% Inputs:
%   ptCloud - point cloud containing ONLY one single cubesat
%
% Outputs:
%   
function [planes,numPlanes] = fitPlanes(ptCloud)
%% Allocation
% Maximum allowed distance from a plane to be considered part of the plane
maxDistance = 0.001;

% Initial value for minimum number of points considered to make up a plane
minPoints   = 10;

% Initialize planes structure
numPlanes = 0;
planes = struct;

% Tracking of points not associated with any plane
remainPtCloud = ptCloud;

%% Find planes until no more planes exist
while remainPtCloud.Count > minPoints
  %% Fit a plane to the ptCloud
  warning('off','vision:pointcloud:notEnoughInliers')
  [~,inlierIndices,outlierIndices] = pcfitplane(remainPtCloud,maxDistance);
  warning('on','vision:pointcloud:notEnoughInliers')
  
  %% Quit now if no significant planes found
  if isempty(inlierIndices) || numel(inlierIndices) < minPoints
    break
  else
    numPlanes = numPlanes + 1; % add a plane if one was found
  end
  
  %% Extract plane from ptCloud
  % extract
  plane = select(remainPtCloud,inlierIndices);
  % store in output
  planes(numPlanes).planeCloud = plane;
  
  %% Extract remaining points from ptCloud
  remainPtCloud = select(remainPtCloud,outlierIndices);
  
  %% Obtain planar basis
  [planes(numPlanes).n,planes(numPlanes).V,planes(numPlanes).o] = affine_fit(planes(numPlanes).planeCloud.Location);
  
  %% Remove if repeat plane (e.g. if this is the cubesat 'feet')
  % For all previous planes
  for i = 1:numPlanes-1
    % if normal is within 45 deg of prior plane's normal or antinormal
    if acosd(dot(planes(numPlanes).n,planes(i).n)) < 45 || acosd(dot(planes(numPlanes).n,planes(i).n)) > 135
      % if this plane is smaller than previous plane
      if planes(numPlanes).planeCloud.Count <= planes(i).planeCloud.Count
        % delete this plane
        planes(numPlanes) = [];
        numPlanes = numPlanes -1;
      % if the previous plane is smaller than this plane
      else
        % replace previous plane with this plane and delete this plane
        planes(i) = planes(numPlanes);
        planes(numPlanes) = [];
      end
    end
  end
  
  %% Update minimum number points required to make a plane
  % Take minPoints to be one-tenth the number of points found in the first
  % plane
  if numPlanes == 1
    minPoints = round(planes(numPlanes).planeCloud.Count/10);
  end
end

%% Check that no more than three planes were identified
if numPlanes > 3
  error('fitPlanes identified more than three planes for a single cubesat')
end

%% Check that planes are mutually orthogonal
for i = 1:numPlanes
  for j = 1:numPlanes
    if asind(dot(planes(i).n,planes(j).n)) > 1 && i~=j
      error('fitPlanes identified planes which are not mutually orthogonal by greater than 1 deg')
    end
  end
end

end