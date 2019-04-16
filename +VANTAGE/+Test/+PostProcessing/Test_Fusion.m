classdef Test_Fusion < matlab.unittest.TestCase
    properties
        configDirecName = 'Config/Testing/Dylan';
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        function testFullSystem(obj)
            import VANTAGE.PostProcessing.Validate
            %%% Housekeeping and Allocation
            close all;
            rng(99);
            testType = '100m';
            simtube = 6;

            %%% Filenames and Configurables
            if strcmpi(testType,'Simulation')
                switch simtube
                    case 1
                        configDirecName = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1';
                        manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Manifest_TOFdev.json';
                        SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Sensors.json'));
                    case 6
                        configDirecName = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6';
                        manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Manifest_TOFdev.json';
                        SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Sensors.json'));
                    otherwise
                        error('unimplemented tube requested')
                end
            elseif strcmpi(testType,'Modular')
                manifestFilename = strcat(obj.configDirecName,'/Manifest.json');
                SensorData = jsondecode(fileread(strcat(obj.configDirecName,'/Sensors.json')));
            elseif strcmpi(testType,'100m')
                manifestFilename = strcat(obj.configDirecName,'/Manifest.json');
                SensorData = jsondecode(fileread(strcat(obj.configDirecName,'/Sensors.json')));
            else
                error('Invalid testType')
            end
            
            Model = VANTAGE.PostProcessing.Model(manifestFilename,obj.configDirecName);
            
            fileLims = [1 inf];
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
                Model.Deployer,'presentResults',0,'fileLims',fileLims,'showDebugPlots',0);
            
            % Process truth data
            Truth = Model.Truth_VCF;
            
            % Compute results
            Model.ComputeStateOutput();
            
            Validator = Validate(obj.configDirecName,Model);
            
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
                dataFolder = 'Data/ModularTest_4_9/Results';
                folderString = Model.Deployer.TruthFileName;
                tmp = split(folderString,'/');
                testNumber = tmp{3};
            elseif strcmpi(Model.Deployer.testScenario,'100m')
                dataFolder = 'Data/3_25_100m/Results';
                testNumber = 'Test1';
            else
                testNumber = 'notimplemented';
            end
            
            mkdir(dataFolder)
            save([pwd '/' dataFolder '/CSData' testNumber '.mat'],'CubeSatFitted');
            save([pwd '/' dataFolder '/TruthData' testNumber '.mat'],'TruthFitted');
            save([pwd '/' dataFolder '/AbsErrorData' testNumber '.mat'],'AbsoluteError');
            save([pwd '/' dataFolder '/XErrorData' testNumber '.mat'],'XError');
            save([pwd '/' dataFolder '/YErrorData' testNumber '.mat'],'YError');
            save([pwd '/' dataFolder '/ZErrorData' testNumber '.mat'],'ZError');
            
            Validator.PlotResults(t_fit,CubeSatFitted,TruthFitted,AbsoluteError,Model);
        end
        
        function testError(obj)
            return
            import VANTAGE.PostProcessing.Validate
            %%% Housekeeping and Allocation
            close all;
            rng(99);
            testType = '100m';
            simtube = 6;

            %%% Filenames and Configurables
            if strcmpi(testType,'Simulation')
                switch simtube
                    case 1
                        manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Manifest_TOFdev.json';
                    case 6
                        manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Manifest_TOFdev.json';
                    otherwise
                        error('unimplemented tube requested')
                end
            elseif strcmpi(testType,'Modular')
                manifestFilename = strcat(obj.configDirecName,'/Manifest.json');
            elseif strcmpi(testType,'100m')
                manifestFilename = strcat(obj.configDirecName,'/Manifest.json');
            else
                error('Invalid testType')
            end
            
            Model = VANTAGE.PostProcessing.Model(manifestFilename,obj.configDirecName);

            Validator = Validate(obj.configDirecName,Model);
            
            Validator.ComputeMeanError(Model);
        end
    end
    
    
    methods (Access = private)
    end

end
