% velocity'
clear
close all
modularDataDir = 'Data/Results/matFiles/ModularTest_4_9/Modular/';
dataDir100m = 'Data/Results/matFiles/100m/100m/';


% Rewrite modular data
modTestFolders = dir([modularDataDir 'Test*']);
testFolders100m = dir([dataDir100m 'Test*']);
% 
% for i = 1:numel(modTestFolders)
%     file = modTestFolders(i).name;
%     data = load([modularDataDir file]);
%     dataStruct = data.dataStruct;
%     dataStruct.velocity = dataStruct.velocity';
%     save([pwd '/' modularDataDir file],'dataStruct');
% end

for i = 1:numel(testFolders100m)
    file = testFolders100m(i).name;
    data = load([dataDir100m file]);
    dataStruct = data.dataStruct;
    %dataStruct.velocity = dataStruct.velocity';
    save([pwd '/' dataDir100m file],'dataStruct');
end