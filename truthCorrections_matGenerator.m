%% Truth Data Corrections Processing
%{
This script will generate .mat files for all truth data correction files to
be processed in error analysis
@author         Dylan Bossie
@date           18-Apr-2019
%}
clear

%% Load mat files
directorySimulation = {'Data/Simulation/Simulation_*'};
directoryModular = {'Data/ModularTest_4_9/Test*'};
directory100m = {'Data/3_25_100m/Test*'};
directories = [directorySimulation directoryModular directory100m];
        
outputFolder = 'Data/truthmats/';

% Iterate through the test types
for i = 1:numel(directories)
    % For testtype(i), locate all relevant directories based on configured
    % input above
    testFolders = directories{i};
    folders = dir(testFolders);
    dataStruct = struct();
    
    % Extract test type string
    tmp = strsplit(testFolders,'/');
    testType = tmp{2};
    
    % Extract all truth_corrections files and convert to .mats
    for j = 1:numel(folders)
        folderdata = folders(j);
        truthFile = [pwd '/Data/' testType '/' folderdata.name '/truth_corrections.json'];
        truthData = jsondecode(fileread(truthFile));
        dt = truthData{1};
        n_vec = truthData{2};
        theta = truthData{3};
        offset_vec = truthData{4};
        % Convert to .mat
        dataStruct.dt{j} = dt;
        dataStruct.n_vec{j} = n_vec;
        dataStruct.theta{j} = theta;
        dataStruct.offset_vec{j} = offset_vec;
    end
    
    % Output .mat
    mkdir([pwd '/' outputFolder]);
    save([pwd '/' outputFolder testType '.mat'],'dataStruct');
end