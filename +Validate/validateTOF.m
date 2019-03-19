%%% Script which executes validation for TOF
%% Housekeeping and Allocation
close all;clearvars
rng(99);
tube = 6;
fileLims = [1 inf];

%% Filenames and Configurables
switch tube
    case 1
        configDirecName = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1';
        truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube1/config_simulation_template_2_25_Josh_ToF_Calibration_tube1_truth_data.json';
        manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Manifest_TOFdev.json';
        SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Sensors.json'));
    case 6
        configDirecName = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6';
        truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube6/config_simulation_template_2_25_Josh_ToF_Calibration_tube6_truth_data.json';
        manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Manifest_TOFdev.json';
        SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Sensors.json'));
    otherwise
        error('unimplemented tube requested')
end

%% Initialize VANTAGE Model
Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename,configDirecName);

%% Present Errors














