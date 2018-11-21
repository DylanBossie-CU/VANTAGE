%%  Author: Joshua Kirby
%  Created: 11/20/2018
% Modified: 11/20/2018
%
% TOF_centroid
%
% 
% Purpose:
%
% Inputs:
%   filestrs  - cell array of n TOF files to process, cell
%   dataType  - 'sim' : indicates that the source data came from the
%                       VANTAGE simulation
%               'test': indicates that the source data came from test data
%                       obtained using Prof McMahon's TOF camera
%
% Outputs:
%   centroids - length n cell array containing a 3xm set of vectors defining the 
%               location of m identified cubesats in each TOF file
%
function [centroids] = TOF_centroid(filestrs,dataType)
%% Loop through files
for i = 1:length(filestrs)
  %% Load data
  if strcmpi(dataType,'sim')
    ptCloud = loadSimToF(filestrs{i});
  elseif strcmpi(dataType,'test')
    ptCloud = loadTestData(filestrs{i});
  else
    error('Invalid dataType')
  end
  
  %% If good frame
    if ptCloud.Count>200 && min(ptCloud.Location(:,3))<=3
        %% Split into seperate cubesat point clouds
        [cubesats] = ptSplit(ptCloud);
        
        %% Loop through cubesats
        for j = 1:numel(cubesats)
            %% Find size of cubesat
            centroids(j).u(i) = findCubesatSize(cubesats(j).ptCloud);
            
            %% Fit planes to point cloud
            [planes,numPlanes] = fitPlanes(cubesats(j).ptCloud);
            
            %% Do not proceed if no planes are identified
            if numPlanes==0
                break;
            end
            
            %% Find centroid based on number of identified planes
            u = mode( centroids(j).u(centroids(j).u~=0) );
            if numPlanes==1
                centroids(j).pos(i,:) = centroid1(cubesats(j).ptCloud,planes,u);
            elseif numPlanes==2
                
            elseif numPlanes==3
                centroids(j).pos(i,:) = centroid3(cubesats(j).ptCloud,planes,u);
            end
        end
    end
end

















end