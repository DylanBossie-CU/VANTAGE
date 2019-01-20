%%  Author: Joshua Kirby
%  Created: 11/20/2018
% Modified: 11/24/2018
%
% TOF_centroid
%
% Assumptions: 
%  -This code currently assumes that once CubeSats enter the FOV they
%  can always be identified (valid for early frames, which are all that this
%  code can handle right now due to degrading point cloud density as time
%  goes on)
%  -This code currently assumes that CubeSat U's are known in order.
% 
% Purpose:
%
% Inputs:
%   filestrs  - cell array of n TOF files to process, cell
%   SET       - 
%   dataType  - 'sim' : indicates that the source data came from the
%                       VANTAGE simulation
%               'test': indicates that the source data came from test data
%                       obtained using Prof McMahon's TOF camera
%
% Outputs:
%   centroids - length n struct containing a 3xm set of vectors defining the 
%               location of m identified cubesats in each TOF file
%
function [CENTROID] = TOF_centroid(filestrs,SET,dataType)
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
  
  %% Initialize CENTROID
  CENTOID(i).u    = nan(1,6);
  CENTROID(i).pos = nan(3,6);
  
  %% If good frame
    if ptCloud.Count>200 && min(ptCloud.Location(:,3))<=3
        %% Split into seperate cubesat point clouds
        % NOTE: ordered first-out to last-out
        [cubesats] = ptSplit(ptCloud);
        
        %% Loop through cubesats
        for j = 1:numel(cubesats)
            %% Fit planes to point cloud
            [planes,numPlanes] = fitPlanes(cubesats(j).ptCloud);
            
            %% Find size of CubeSat
            CENTROID(i).u(j) = findCubesatSize(j,SET);
            
            %% Do not proceed if no planes are identified
            if numPlanes==0
                break;
            end
            
            %% Find centroid based on number of identified planes
            if numPlanes==1
                CENTROID(i).pos(:,j) = centroid1(cubesats(j).ptCloud,planes,CENTROID(i).u(j),SET);
            elseif numPlanes==2
                CENTROID(i).pos(:,j) = centroid2(cubesats(j).ptCloud,planes,CENTROID(i).u(j),SET);
            elseif numPlanes==3
                CENTROID(i).pos(:,j) = centroid3(cubesats(j).ptCloud,planes,CENTROID(i).u(j),SET);
            end
        end   % for all cubesats
    end % if good frame
end

end