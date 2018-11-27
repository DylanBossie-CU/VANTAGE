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
CSPARAMS.u_short = 0.1; % m
CSPARAMS.u_long  = 0.1125; % m

%% U
CSPARAMS.U = [1 2 3 4 5 6];

%% Side Lengths
CSPARAMS.L_short = CSPARAMS.u_short;
CSPARAMS.L_long  = CSPARAMS.u_long .*CSPARAMS.U;
CSPARAMS.L = [CSPARAMS.L_long CSPARAMS.L_short];

%% Face Areas
CSPARAMS.A_short = CSPARAMS.u_short^2;
% Note this is a hack, A_long should use u_long but this works better for
% now, should use better heuristics so this hack is not necessary
CSPARAMS.A_long  = CSPARAMS.U.*CSPARAMS.u_short^2;
CSPARAMS.A = [CSPARAMS.A_long CSPARAMS.A_short];

%% Face Diagonals
CSPARAMS.D_short = sqrt(CSPARAMS.u_short^2 + CSPARAMS.L_short^2);
CSPARAMS.D_long  = sqrt(CSPARAMS.u_short^2 + CSPARAMS.L.^2);
CSPARAMS.D = [CSPARAMS.D_long CSPARAMS.D_short];






end