classdef Test_Fusion < matlab.unittest.TestCase
    properties
        %configDirecName = 'Config/Final_Tests/Simulation_4_15/140/Sample3';
        configDirecName = {
            'Config/Final_Tests/Simulation_4_15/140/Sample1',...
            'Config/Final_Tests/Simulation_4_15/140/Sample2',...
            'Config/Final_Tests/Simulation_4_15/140/Sample3',...
            'Config/Final_Tests/Simulation_4_15/140/Sample4',...
            'Config/Final_Tests/Simulation_4_15/140/Sample5',...
            'Config/Final_Tests/Simulation_4_15/140/Sample6',...
            'Config/Final_Tests/Simulation_4_15/140/Sample7',...
            'Config/Final_Tests/Simulation_4_15/140/Sample8',...
            'Config/Final_Tests/Simulation_4_15/140/Sample9',...
            'Config/Final_Tests/Simulation_4_15/140/Sample10'};
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        function testFullSystem(obj)
            return
            import VANTAGE.PostProcessing.Validate
            for iter = 1:numel(obj.configDirecName)
            %%% Housekeeping and Allocation
            close all;
            rng(99);

            %%% Filenames and Configurables
            manifestFilename = strcat(obj.configDirecName{iter},'/Manifest.json');
            SensorData = jsondecode(fileread(strcat(obj.configDirecName{iter},'/Sensors.json')));
            
            Model = VANTAGE.PostProcessing.Model(manifestFilename,obj.configDirecName{iter});
            
            fileLims = [1 inf];
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
                Model.Deployer,'presentResults',0,'fileLims',fileLims,'showDebugPlots',0);
            
            % Process truth data
            Truth = Model.Truth_VCF;
            
            % Compute results
            Model.ComputeStateOutput();
            
            Validator = Validate(obj.configDirecName{iter},Model,true);
            
            CubeSatFitted = cell(Model.Deployer.numCubesats,1);
            TruthFitted = cell(Model.Deployer.numCubesats,1);
            AbsoluteError = cell(Model.Deployer.numCubesats,1);
            XError = cell(Model.Deployer.numCubesats,1);
            YError = cell(Model.Deployer.numCubesats,1);
            ZError = cell(Model.Deployer.numCubesats,1);
            
            data_t = Model.Deployer.CubesatArray(1).time(end);
            t_fit = linspace(0,data_t);
            for i=1:Model.Deployer.numCubesats
               CubeSat = Model.Deployer.CubesatArray(i);
               CubeSatFitted{i} = Validator.fitCubeSatTraj(CubeSat.centroids_VCF,CubeSat.time,'CS',t_fit,Model);
               TruthFitted{i} = interp1(Model.Truth_VCF.t,Model.Truth_VCF.Cubesat(i).pos,t_fit,'linear');
               [AbsoluteError{i},XError{i},YError{i},ZError{i}] = ...
                   Validator.ProcessError(CubeSatFitted{i},TruthFitted{i});
            end
            
            % Save fitted results for error analysis later
            if strcmpi(Model.Deployer.testScenario,'Modular')
                dataFolder = 'Data/Results/matFiles/ModularTest_4_9';
                folderString = Model.Deployer.TruthFileName;
                tmp = split(folderString,'/');
                testNumber = tmp{3};
            elseif strcmpi(Model.Deployer.testScenario,'100m')
                dataFolder = 'Data/Results/matFiles/100m';
                folderString = Model.Deployer.TruthFileName;
                tmp = split(folderString,'/');
                testNumber = tmp{3};
            elseif strcmpi(Model.Deployer.testScenario,'Simulation')
                dataFolder = 'Data/Results/matFiles/Simulation_4_15_140';
                tmp = split(SensorData.TOFData,'/');
                testNumber = tmp{4};
            else
                error('invalid test type in Deployer.TruthFileName')
            end
            
            mkdir(dataFolder)
            save([pwd '/' dataFolder '/CSData' testNumber '.mat'],'CubeSatFitted');
            save([pwd '/' dataFolder '/TruthData' testNumber '.mat'],'TruthFitted');
            save([pwd '/' dataFolder '/AbsErrorData' testNumber '.mat'],'AbsoluteError');
            save([pwd '/' dataFolder '/XErrorData' testNumber '.mat'],'XError');
            save([pwd '/' dataFolder '/YErrorData' testNumber '.mat'],'YError');
            save([pwd '/' dataFolder '/ZErrorData' testNumber '.mat'],'ZError');
            save([pwd '/' dataFolder '/CSTime' testNumber '.mat'],'t_fit');
            end
        end
        
        function testError(obj)
            import VANTAGE.PostProcessing.Validate
            for iter = 1:numel(obj.configDirecName)
            %%% Housekeeping and Allocation
            rng(99);

            %%% Filenames and Configurables
            manifestFilename = strcat(obj.configDirecName{iter},'/Manifest.json');
            SensorData = jsondecode(fileread(strcat(obj.configDirecName{iter},'/Sensors.json')));
            
            Model = VANTAGE.PostProcessing.Model(manifestFilename,obj.configDirecName{iter});

            Validator = Validate(obj.configDirecName{iter},Model,false);
            
            %Validator.PlotResults(Model,SensorData);
            
            Validator.ErrorAnalysis(Model,SensorData);
            end
        end
    end
    
    
    methods (Access = private)
    end

end
