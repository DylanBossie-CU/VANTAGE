% velocity'
clear
close all
modularDataDir = 'Data/Results/matFiles/ModularTest_4_9/Modular/';
dataDir100m = 'Data/Results/matFiles/100m/100m/';
simDir1 = 'Data/Results/matFiles/Simulation_4_15_030/';
simDir2 = 'Data/Results/matFiles/Simulation_4_15_085/';
simDir3 = 'Data/Results/matFiles/Simulation_4_15_140/';
simDir4 = 'Data/Results/matFiles/Simulation_4_15_195/';
simDir5 = 'Data/Results/matFiles/Simulation_4_15_250/';


% Rewrite modular data
modTestFolders = dir([modularDataDir 'Test*']);
testFolders100m = dir([dataDir100m 'Test*']);
sim1Folders = dir([simDir1 'Sample*']);
sim2Folders = dir([simDir2 'Sample*']);
sim3Folders = dir([simDir3 'Sample*']);
sim4Folders = dir([simDir4 'Sample*']);
sim5Folders = dir([simDir5 'Sample*']);

% 
% for i = 1:numel(modTestFolders)
%     file = modTestFolders(i).name;
%     data = load([modularDataDir file]);
%     dataStruct = data.dataStruct;
%     dataStruct.velocity = dataStruct.velocity';
%     save([pwd '/' modularDataDir file],'dataStruct');
% end
% 
% for i = 1:numel(testFolders100m)
%     file = testFolders100m(i).name;
%     data = load([dataDir100m file]);
%     dataStruct = data.dataStruct;
%     %dataStruct.velocity = dataStruct.velocity';
%     save([pwd '/' dataDir100m file],'dataStruct');
% end

for i = 1:numel(sim1Folders)
    file = sim1Folders(i).name;
    data = load([simDir1 file]);
    dataStruct = data.dataStruct;
    dataStruct.velocity = dataStruct.velocity';
    save([pwd '/' simDir1 file],'dataStruct');
end

for i = 1:numel(sim2Folders)
    file = sim2Folders(i).name;
    data = load([simDir2 file]);
    dataStruct = data.dataStruct;
    dataStruct.velocity = dataStruct.velocity';
    save([pwd '/' simDir2 file],'dataStruct');
end
for i = 1:numel(sim3Folders)
    file = sim3Folders(i).name;
    data = load([simDir3 file]);
    dataStruct = data.dataStruct;
    dataStruct.velocity = dataStruct.velocity';
    save([pwd '/' simDir3 file],'dataStruct');
end

for i = 1:numel(sim4Folders)
    file = sim4Folders(i).name;
    data = load([simDir4 file]);
    dataStruct = data.dataStruct;
    dataStruct.velocity = dataStruct.velocity';
    save([pwd '/' simDir4 file],'dataStruct');
end

for i = 1:numel(sim5Folders)
    file = sim5Folders(i).name;
    data = load([simDir5 file]);
    dataStruct = data.dataStruct;
    dataStruct.velocity = dataStruct.velocity';
    save([pwd '/' simDir5 file],'dataStruct');
end
