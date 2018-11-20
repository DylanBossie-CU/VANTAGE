%%  Author: Joshua Kirby
%  Created: 11/20/2018
% Modified: 11/20/2018
%
% Purpose:
%
%% Housekeeping
close all;clc

%% Setup path
addpath(genpath('Subroutines'));

%% Define files to be processed
filestrs = {'Justin Test/Test Data/Separation/DRSep_0_in.ply'};

%%
ptCloud = loadTestData(filestrs{1});
pcshow(ptCloud)
