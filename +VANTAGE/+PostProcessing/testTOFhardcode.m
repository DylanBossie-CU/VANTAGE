clearvars;close all;clc

tube = 1;

switch tube
    case 1
        truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube1/config_simulation_template_2_25_Josh_ToF_Calibration_tube1_truth_data.json';
        manifestFilename = 'Config/Testing/TOF/Manifest_TOFdev.json';
        SensorData = jsondecode(fileread('config/Testing/TOF/Sensors_TOFdev.json'));
    otherwise
        error('unimplemented tube requested')
end


Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename);
Deployer = Model.Deployer;
TOF = VANTAGE.PostProcessing.TOF('config/TOF.json',Model);
Deployer = TOF.TOFProcessing(SensorData,Deployer,'fileLims',[50,60],'presentResults',1);
