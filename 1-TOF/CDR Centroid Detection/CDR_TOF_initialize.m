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
direc = 'Justin Test/Simulation Data/ToF/';
ls = dir(direc);
c = 1;
for i = 1:30
  if contains(ls(i).name,'FPS_Tube')
    SET.filestrs{c} = [direc ls(i).name];
    c = c + 1;
  end
end

SET.filestrs = {[direc,'30FPS_Tube_2_3U_2U_1U_VX_0_VY_0_VZ_001_WX_0_WY_0_WZ_0_SEP_0P03_F00040.pcd']};
SET.dataType  = 'sim'; % 'test' for test data
                        % 'sim'  for sim  data
%% Obtain Truth data
SET.TRUTH = generateSimTruth(SET.filestrs);










end