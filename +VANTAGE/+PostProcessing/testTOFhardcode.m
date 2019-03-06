clearvars;close all;clc

truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube1/config_simulation_template_2_25_Josh_ToF_Calibration_tube1_truth_data.json';
manifestFilename = 'Config/Testing/TOF/Manifest_TOFdev.json';
Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename);
Deployer = Model.Deployer;
SensorData = jsondecode(fileread('config/Testing/TOF/Sensors_TOFdev.json'));
TOF = VANTAGE.PostProcessing.TOF('config/TOF.json',Model);
Deployer = TOF.TOFProcessing(SensorData,Deployer,'fileLims',[71,80],'presentResults',1);
