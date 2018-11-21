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
function [planes,numPlanes] = Copy_of_fitPlanes(ptCloud)
%% Allocation
% Maximum allowed distance from a plane to be considered part of the plane
maxDistance = 0.001;

% Initial value for minimum number of points considered to make up a plane
minPoints   = 10;

% Initialize planes structure
numPlanes = 0;
planes(3) = struct('o',[],'n',[],'V',[],'planeCloud',[]);

% Tracking of points not associated with any plane
remainPtCloud = ptCloud;

%% Find planes until no more planes exist
while remainPtCloud.Count > minPoints
  %% Fit a plane to the ptCloud
  [~,inlierIndices,outlierIndices] = pcfitplane(remainPtCloud,maxDistance);
  
  %% Quit now if no significant planes found
  if isempty(inlierIndices) || numel(inlierIndices) < minPoints
    break
  else
    numPlanes = numPlanes + 1; % add a plane if one was found
  end
  
  %% Extract plane from ptCloud
  % extract
  plane = select(remainPtCloud,inlierIndices);
  % denoise
  plane = pcdenoise(plane);
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
      % delete this plane
      planes(numPlanes).n = [];
      planes(numPlanes).V = [];
      planes(numPlanes).o = [];
      planes(numPlanes).planeCloud = [];
      numPlanes = numPlanes -1;
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