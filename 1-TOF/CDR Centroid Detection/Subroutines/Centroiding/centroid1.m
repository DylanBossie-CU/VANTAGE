%%  Author: Justin Fay and Joshua Kirby
%  Created: 
% Modified: 11/24/2018
%
% Purpose:
%
% Inputs:
%
% Outputs:
%   
function pos = centroid1(ptCloud,planes,u,SET)
%% Project points onto 2d plane
plane = planes(1).planeCloud;
inPlane = double( (plane.Location-planes(1).o)*planes(1).V );

%% Find plane boundary points
I_bound = boundary(inPlane(:,1),inPlane(:,2));

%% Find preliminary centroid of boundary
face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
[x,y] = centroid(face);

%% Find corners of face
for i = 1:size(face.Vertices,1)
  r(i)     = norm(face.Vertices(i,:)-[x y]);
  theta(i) = atan2d(face.Vertices(i,2)-y,face.Vertices(i,1)-x);
end
[theta,I] = sort(theta);
r = r(I);
face.Vertices(:,1) = face.Vertices(I,1);
face.Vertices(:,2) = face.Vertices(I,2);
% locs identify the corner indices in face.Vertices
tmp = smooth(abs(gradient(r,theta)));
[~,locs] = findpeaks(padarray(r',10),'NPeaks',4,'sortstr','descend');
locs = locs - 10;

%% Identify centroid, algorithm varies based on number of corners found
% NOTE: THIS NEEDS TO BE CHANGED TO USE THE IDENTIFIED U OF THE CUBESAT IN
% ORDER TO ACCOUNT FOR ERRORS FROM TOF DATA NOT COVERING FULL SIDE OF
% CUBESAT
switch length(locs)
  % Four Corners
  case 4
    % Centroid is simple average
    tmp = mean(face.Vertices(locs,:),1);
    x = tmp(1);
    y = tmp(2);
  % Three Corners
  case 3
    foundOffDiagonal = 0;
    % Loop through points
    for i = 1:3
      vc = 1;
      % produce vectors from current point to other two points
      for j = [1:i-1 i+1:3]
        vec{vc} = face.Vertices(locs(j),:)-face.Vertices(locs(i),:);
        vc = vc + 1;
      end
      % if angle between those vectors is within 10 deg of 90 deg, the
      % current index (i) corresponds to the off-diagonal point
      if abs(acosd(dot(vec{1},vec{2})/(norm(vec{1})*norm(vec{2})))-90) < 10
        foundOffDiagonal = 1;
        break
      end
    end
    % Error if not off diagonal was found
    if ~foundOffDiagonal
      error('Off diagonal corner could not be found when findpeaks returns three corners, catching point could be method or 10 degree orthogonality threshold')
    end
    % centroid is mean of on-diagonal points
    tmp = mean(face.Vertices(locs([1:i-1 i+1:3]),:),1);
    x = tmp(1);
    y = tmp(2);
    
  % Two Corners
  case 2
    error('findpeaks only identifed two corners in the cubesat face, add the ability to handle this')
  
  % One corner
  case 1
    error('findpeaks only identified one corner in the cubesat face, create the ability to handle this?')
  
  % No corners
  case 0
    error('findpeaks could not identify any corners in the cubesat face')
  
  % Some other number
  otherwise
    error('findpeaks identified an unhandled number of corners in the cubesat face, time to investigate...')
end

%% Convert plane centroid to 3d point
outPlane = planes(1).o + (x*planes(1).V(:,1))' + (y*planes(1).V(:,2))';

%% Project inward to volumetric centroid
d = distInFromFace(face,u,SET);
pos = outPlane + d.*(sign(dot(outPlane,planes(1).n))*planes(1).n)';

end