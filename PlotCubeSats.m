clear
close all

testType = 'Simulation_4_15_085';

matFileDir = 'Data/Results/matFiles/';

matFileDirModular = [matFileDir testType '/data/'];
matFileDirSimulation = [matFileDir testType '/data/'];
matFileDir100m = [matFileDir testType '/data/'];

switch testType
    case 'ModularTest_4_9'
        matFiles = dir([matFileDirModular 'CubeSatArrayTest*']);
        matFileDir = matFileDirModular;
    case '100m'
        matFiles = dir([matFileDir100m 'CubeSatArrayTest*']);
        matFileDir = matFileDir100m;
    case 'Simulation_4_15_085'
        matFiles = dir([matFileDirSimulation 'CubeSatArraySample*']);
        matFileDir = matFileDirSimulation;
end

mkdir('FigureOut');
for i = 1:numel(matFiles)
    load([matFileDir matFiles(i).name]);
    CubeSat = CubeSats(1);
    centroids = CubeSat.centroids_VCF;
    for j = 1:numel(centroids)
        X(j) = centroids{j}(1);
        Y(j) = centroids{j}(2);
        Z(j) = centroids{j}(3);
    end
    tmp = split(matFiles(i).name,'.');
    filename = tmp{1};
    tmp = split(filename,'CubeSatArray');
    testNum = tmp{2};
    figure
    hold on
    scatter(X,Y,Z)
    savefig(['FigureOut/' testNum testType])
end


