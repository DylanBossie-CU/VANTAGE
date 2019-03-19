%% VANTAGE Wrapper
% This script performs as the high-level commanding script which will
% sequentially instruct each subsystem of the VANTAGE software to execute
% at the appropriate times.
% @author   Dylan Bosie
% @date     17-Feb-2019
close all
clear

%% Process Deployment Manifest and Truth data
truthFilename = 'config/Testing/TruthDataTest.json';
manifestFilename = 'Config/Manifest.json';

%% Send Sensor Ready Signal to RBP
% TBD when rest of software is complete


%% Receive Launch Signal from RBP
% TBD when rest of software is complete


%% Initialize Automated Sensor Data Collection
% To be added once post-processing is complete


%% Perform Test Data Cleaning
% Must perform background subtraction before data can be processed


%% Data Post-Processing
% Import the VANTAGE post-processing software suite
import VANTAGE.PostProcessing.*
% Initialize the post-processing objects within Model class
Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename,'./Config');
OpticalCam = Model.Optical;

% Unit vector to VCF origin from CCF
UnitOriginVCF = Model.Deployer.GetCamOriginVCF;

dataFiles = dir(strcat(OpticalCam.DataDirec,OpticalCam.FileExtension));

% Compute position and velocity states for the input data
Model.ComputeStateOutput()
disp('Completed Post-processing :^)')
% At this point, TOF data has been processed up to its predefined desired
% range 

%% Compare Results against Deployment Predictions
import VANTAGE.Validate.*
Validate.validateOptical();
Validate.validateTOF();
Validate.validateVantage();
disp('Completed deployment comparison (^:')
%% Generate Output Report

disp('Generated output report :")')
%% Send Output Report to RBP

disp('Output report transmitted to deployer')
%% Cleanup & Shutdown

disp('Cleanup complete. Shutting down...')
