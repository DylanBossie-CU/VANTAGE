%% VANTAGE Wrapper
% This script performs as the high-level commanding script which will
% sequentially instruct each subsystem of the VANTAGE software to execute
% at the appropriate times.
% @author   Dylan Bosie
% @date     17-Feb-2019
clc
close all
clear

%% Process Deployment Manifest
fileName = 'config/Manifest.json';
Deployer = VANTAGE.PostProcessing.Deployer(fileName);

%% Send Sensor Ready Signal to RBP
% pleaseAvionicsTeamSaveMe(wat);

%% Receive Launch Signal from RBP
% iDontKnowHowToDoThis(yay);

%% Initialize Automated Sensor Data Collection
% SeanAutomationCodeHere(yeet);

%% Data Post-Processing
%Initialize VANTAGE Classes
CubeSats = Deployer.CubesatArray;
TOF = VANTAGE.PostProcessing.TOF;
Optical = VANTAGE.PostProcessing.Optical;

SensorData = jsondecode(fileread('config/Sensors.json'));

% TOF Post-Processing
TOFData = dir(strcat(SensorData.TOFData,'*.pcd'));
try
    TOF.naiveFindCentroids(TOFData,Deployer,SensorData);
    %CubeSats_TOF = TOF.Processing(TOFData,Deployer,SensorData);
catch
    disp('TOF not fully implemented yet. Continuing...')
end

% At this point, TOF data has been processed up to its predefined desired
% range 
%% Compare Results against Deployment Predictions


%% Generate Output Report


%% Send Output Report to RBP


%% Cleanup & Shutdown

