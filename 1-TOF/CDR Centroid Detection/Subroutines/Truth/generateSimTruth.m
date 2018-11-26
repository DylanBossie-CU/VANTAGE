%%  Author: Joshua Kirby
%  Created: 11/23/2018
% Modified: 11/23/2018
%
% generateSimTruth
%
% 
% Purpose:
%
% Inputs:
%   filestrs  - cell array of n TOF files to process, cell
%
% Outputs:
%   truth    - length n struct containing a 3xm set of vectors defining the 
%               location of m identified cubesats in each TOF file
%
function [TRUTH] = generateSimTruth(filestrs,R0)
%% For all filestrs
for i = 1:length(filestrs)
  % repackage
  str = filestrs{i};
  % replace all P's with .'s
  j = strfind(str,'P');
  str(j) = '.';
  % crop to beginning
  if contains(str,'\')
    error('Slashes in filestrs must be ''/'' not ''\''')
  end
  slashes = strfind(str,'/');
  str = str(slashes(end)+1:end);
  % find fps
  j = strfind(str,'F.S');
  fps = str2num(str(1:j-1));
  % find tube
  j = strfind(str,'Tube_');
  tube = str2num(str(j+5));
  str = str(j+6:end);
  % find num cubesats and u
  k = strfind(str,'V');
  j = strfind(str(1:k),'U');
  numSats = length(j);
  for k = 1:length(j)
    u(k) = str2num(str(j(k)-1));
  end
  TRUTH(i).u = flip(u);
  str = str(j(end)+1:end);
  % find cubesat velocity
  j = strfind(str,'VX_');
  k = strfind(str,'_VY');
  vx = str2num(str(j+3:k-1)); % m/s
  j = strfind(str,'VY_');
  k = strfind(str,'_VZ');
  vy = str2num(str(j+3:k-1)); % m/s
  j = strfind(str,'VZ_');
  k = strfind(str,'_WX');
  vz = str2num(str(j+3:k-1)); % m/s
  V = [vx vy vz]'; % m/s
  % Find cubesat angular velocity
%   j = strfind(str,'WX_');
%   k = strfind(str,'_WY');
%   wx = str2num(str(j+3:k-1)); % rad/s
%   j = strfind(str,'WY_');
%   k = strfind(str,'_WZ');
%   wy = str2num(str(j+3:k-1)); % rad/s
%   j = strfind(str,'WZ_');
%   k = strfind(str,'_SE.');
%   wz = str2num(str(j+3:k-1)); % rad/s
%   W = [wx wy wz]'; % rad/s
  % Find separation
  j = strfind(str,'SE._');
  k = strfind(str,'_F');
  sep = str2num(str(j+4:k-1));
  % Find frame
  j = strfind(str,'_F');
  k = strfind(str,'.pcd');
  frame = str2num(str(j+2:k-1));
  % Find time
  t = frame/fps; % s
  % Find centroid
  for j = 1:numSats
    TRUTH(i).pos(:,j) = R0(:,j) + V*t; % m
  end
  
  
  
end











end