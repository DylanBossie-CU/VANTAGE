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
function [SET] = CDR_TOF_initialize()
%% Obtain Cubesat Parameters
SET.CSPARAMS = CubesatParams;

%% Define files to be analyzed
direc = 'Simulation Data/Tube 6/';
ls = dir(direc);
c = 1;
for i = 1:30
  if contains(ls(i).name,'FPS_Tube')
    SET.filestrs{c} = [direc ls(i).name];
    c = c + 1;
  end
end

SET.filestrs = {[direc,'30FPS_Tube_2_3U_2U_1U_VX_0_VY_0_VZ_001_WX_0_WY_0_WZ_0_SEP_0P03_F00030.pcd']};
SET.filestrs = {[direc,'30FPS_Tube_6_3U_2U_1U_VX_0_VY_0_VZ_001_WX1U_0p1_WY1U_0p1_WZ1U_0p1_SEP_0P03_F00060.pcd']};
SET.dataType  = 'sim'; % 'test' for test data
                       % 'sim'  for sim  data
                        
%% Hardcodd initial position of cubesats
warning('Initial position currently hard coded')
tube = 6;
switch tube
  case 2
    R0 = [13.1 0 -3.625;13.1 0 -24.5;13.1 0 -56.625]'./100; % m
    R0(3,:) = R0(3,:) - 3.625/100;
  case 6
    R0 = [39.2 -14.4 -56.625; 39.2 -14.4 -24.5;39.2 -14.4 -3.625]'./100; % m
    R0(3,:) = R0(3,:) - 3.625/100;
  otherwise
    error('Unhandled tube was defined')
end


%% Obtain Truth data
SET.TRUTH = generateSimTruth(SET.filestrs,R0);













end