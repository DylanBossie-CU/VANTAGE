%%  Author: Joshua Kirby
%  Created: 11/24/2018
% Modified: 11/24/2018
% 
% Purpose:
%
% Inputs:
%
% Outputs:
%
function pos = centroid2(ptCloud,planes,u,SET)
%% Not currently used, but does check if planes intersect as expected
for i = 1:2
  M(i,1:3) = planes(i).n;
  c(i,1)   = planes(i).n(1)*planes(i).o(1) + planes(i).n(2)*planes(i).o(2) + planes(i).n(3)*planes(i).o(3);
end

if rank(M) ~= 2
  error('The two planes identified do not intersect as a line')
end

%% Determine parameters corresponding to the plane intersect
% unit vector parallel to intersection
V = cross(planes(1).n,planes(2).n);

% project point cloud onto this unit vector direction
pc_alongIntersect = [planes(1).planeCloud.Location;planes(2).planeCloud.Location]*V;

% Determine length of intersection
lengthIntersect   = max(pc_alongIntersect)-min(pc_alongIntersect);

% Determine mean value along the intersection direction
mean_alongIntersect = 0.5*(max(pc_alongIntersect)+min(pc_alongIntersect));

%% Locate mean point along intersect
[min_alongIntersect,I]  = min(pc_alongIntersect);
tmp    = [planes(1).planeCloud.Location;planes(2).planeCloud.Location];
if I <= planes(1).planeCloud.Count % if point is in plane 1
  t = (dot(planes(2).n,planes(2).o)-dot(planes(2).n,tmp(I,:)))/norm(planes(2).n)^2;
  minpt = tmp(I,:)' + t*planes(2).n;
else % if point is in plane 2
  t = (dot(planes(1).n,planes(1).o)-dot(planes(1).n,tmp(I,:)))/norm(planes(1).n)^2;
  minpt = tmp(I,:)' + t*planes(1).n;
end
meanpt = minpt + (mean_alongIntersect-min_alongIntersect)*V;

%% Determine best-guess face size and calculate centroid
% ensure that unitvectors are inward facing
n(:,1) = sign(dot(meanpt,planes(1).n))*planes(1).n;
n(:,2) = sign(dot(meanpt,planes(2).n))*planes(2).n;
switch u
  % 1U Cubesat
  case 0
    % mid-plane diagonal unit vector
    innerDiag = (n(:,1)+n(:,2))./norm(n(:,1)+n(:,2));
    % centroid (project along innerDiag by half a diagonal face length)
    pos       = meanpt + 0.5*SET.CSPARAMS.D_short*innerDiag;
  case {1,2,3,4,5,6}
    % Make best guess at which face is which
    As = SET.CSPARAMS.A;
    for i = 1:2
      inPlane = double( (planes(i).planeCloud.Location-planes(i).o)*planes(i).V );
      I_bound = boundary(inPlane(:,1),inPlane(:,2));
      face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
      A = area(face);
      [dAmin(i),J(i)] = min(abs(As-A));
    end
    [~,I] = min(dAmin);
    bestI = I; % this index is the plane corresonding to bestSize
    offI = [1 2];
    offI = offI(offI~=bestI); % this index is the other plane
    bestSize = J(bestI); % this value says whether bestI is a 1U, 2U, or 3U length side
    if bestSize == 1 && u ~= 1
      bestSize = 7;
    end
    switch bestSize
      case {1,2,3,4,5,6}
        [~,offSize]  = min(abs(SET.CSPARAMS.L-lengthIntersect)); % this value says the U of the off-size
      case 7
        offSize = u;
    end
      % mid-plane diagonal unit vector
    innerDiag = unitvec(SET.CSPARAMS.L(offSize)*n(:,bestI) + SET.CSPARAMS.L(bestSize)*n(:,offI));
    % centroid (project along innerDiag by half a diagonal face length)
    pos = meanpt + 0.5*sqrt((SET.CSPARAMS.L(offSize))^2+(SET.CSPARAMS.L(bestSize))^2)*innerDiag;
  otherwise
    error('Invalid u has been given as an input')
end
end