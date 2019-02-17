%% VANTAGE Wrapper
% This script performs as the high-level commanding script which will
% sequentially instruct each subsystem of the VANTAGE software to execute
% at the appropriate times.
% @author   Dylan Bosie
% @date     17-Feb-2019

%% Process Deployment Manifest
fileName = 'config/Manifest.json';
jsondecode(fileread(fileName));

%% Send Sensor Ready Signal to RBP
% pleaseAvionicsTeamSaveMe(wat);

%% Receive Launch Signal from RBP
% iDontKnowHowToDoThis(yay);

%% Initialize Automated Sensor Data Collection
% SeanCodeHere(yeet);

%% Compare Results against Deployment Predictions


%% Generate Output Report


%% Send Output Report to RBP


%% Cleanup & Shutdown

