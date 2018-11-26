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
function [CSPARAMS] = CubesatParams()
%% (U)nit length
CSPARAMS.u = 0.10; % m

%% U
CSPARAMS.U = [1 2 3 4 5 6];

%% Side Lengths
CSPARAMS.L = CSPARAMS.u.*CSPARAMS.U;

%% Face Areas
CSPARAMS.A = CSPARAMS.U.*CSPARAMS.u^2;

%% Face Diagonals
CSPARAMS.D = sqrt(CSPARAMS.u^2 + CSPARAMS.L.^2);






end