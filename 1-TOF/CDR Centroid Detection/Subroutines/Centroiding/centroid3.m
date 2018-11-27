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
function pos = centroid3(ptCloud,planes,u,SET)
%% Find plane intersection

for i = 1:3
  A(i,1:3) = planes(i).n;
  b(i,1)   = planes(i).n(1)*planes(i).o(1) + planes(i).n(2)*planes(i).o(2) + planes(i).n(3)*planes(i).o(3);
end

if rank(A) ~= 3
  error('Planes do not intersect in the expected way')
end

ptIntersect = (A\b);

%% Find Best-Guess Face
% ensure that unitvectors are inward facing
for i = 1:3
  n(:,i) = sign(dot(ptIntersect,planes(i).n))*planes(i).n;
end
% Make best guess for which face is which
As = SET.CSPARAMS.A;
for i = 1:3
  inPlane = double( (planes(i).planeCloud.Location-planes(i).o)*planes(i).V );
  I_bound = boundary(inPlane(:,1),inPlane(:,2));
  face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
  A = area(face);
  [dAmin(i),J(i)] = min(abs(As-A));
end
[~,I] = min(dAmin);
bestI = I; % this index is the plane corresonding to bestSize
bestSize = J(bestI); % this value says what U is bestI

%% Determine other face sizes based on intersect length with best-guess face
c = 1;
for i = [1:I-1 I+1:3]
  % unit vector parallel to intersection
  V = cross(planes(bestI).n,planes(i).n);
  % project point cloud onto this unit vector direction
  pc_alongIntersect = [planes(bestI).planeCloud.Location;planes(i).planeCloud.Location]*V;
  % Determine length of intersection
  lengthIntersect   = max(pc_alongIntersect)-min(pc_alongIntersect);
  
  % Make best guess at which face this is
  switch bestSize
    case {1,7}
      offI(c)    = i;
      offSize(c) = u;
    case u
      offI(c)        = i;
      [~,offSize(c)] = min(abs(SET.CSPARAMS.L-lengthIntersect));
    otherwise
      error('Best-guess for face size does not match an allowable size (1 or U)')
  end
  c = c + 1;
end

%% Find volumetric centroid
% volumetric diagonal
switch bestSize
  case {1,7}
    innerDiag = unitvec(n(:,bestI)*SET.CSPARAMS.u_long*u + n(:,offI(1))*SET.CSPARAMS.L(bestSize) + n(:,offI(2))*SET.CSPARAMS.L(bestSize));
  case u
    innerDiag = unitvec(n(:,bestI)*SET.CSPARAMS.L(7) + n(:,offI(1))*SET.CSPARAMS.L(offSize(2)) + n(:,offI(2))*SET.CSPARAMS.L(offSize(1)));
  otherwise
    error('Best-guess for face size does not match an allowable size (1 or U)')
end
pos = ptIntersect + 0.5*sqrt((u*SET.CSPARAMS.u_long)^2+2*(SET.CSPARAMS.u_short)^2)*innerDiag;
    
end