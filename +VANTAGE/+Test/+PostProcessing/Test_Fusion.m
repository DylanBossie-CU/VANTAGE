classdef Test_Fusion < matlab.unittest.TestCase
    properties
        %%%%%% Modular config direcs
        configDirecNameModular = 'Config/Final_Tests/ModularTest_4_9/Test*'
        %%%%%% 100m config direcs
        configDirecName100m = 'Config/Final_Tests/3_25_100m/Test*';
        %%%%%% Simulation config direcs
        configDirecNameSim085 = 'Config/Final_Tests/Simulation/_085/Sample*';
        configDirecNameSim030 = 'Config/Final_Tests/Simulation/_030/Sample*';
        configDirecNameSim140 = 'Config/Final_Tests/Simulation/_140/Sample*';
        configDirecNameSim195 = 'Config/Final_Tests/Simulation/_195/Sample*';
        configDirecNameSim250 = 'Config/Final_Tests/Simulation/_250/Sample*';
         
        testType = 'Simulation_140';
        
        configDirecName
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        function testFullSystem(obj)
            import VANTAGE.PostProcessing.Validate
            switch obj.testType
                case 'Modular'
                    obj.configDirecName = obj.configDirecNameModular;
                    testType = 'ModularTest_4_9';
                case '100m'
                    obj.configDirecName = obj.configDirecName100m;
                    testType = '3_25_100m';
                case 'Simulation_085'
                    obj.configDirecName = obj.configDirecNameSim085;
                    testType = 'Simulation/_085';
                case 'Simulation_030'
                    obj.configDirecName = obj.configDirecNameSim030;
                    testType = 'Simulation/_030';
                case 'Simulation_140'
                    obj.configDirecName = obj.configDirecNameSim140;
                    testType = 'Simulation/_140';
                case 'Simulation_195'
                    obj.configDirecName = obj.configDirecNameSim195;
                    testType = 'Simulation/_195';
            end

            configfiles = dir(obj.configDirecName);
            
            for iter = 1:numel(configfiles)
            %%% Housekeeping and Allocation
            close all;
            rng(99);

            configfile = [pwd '/config/Final_Tests/' testType '/' configfiles(iter).name];
            
            %%% Filenames and Configurables
            manifestFilename = strcat(configfile,'/Manifest.json');
            SensorData = jsondecode(fileread(strcat(configfile,'/Sensors.json')));
            
            Model = VANTAGE.PostProcessing.Model(manifestFilename,configfile,true);
            
            fileLims = [1 inf];
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
                Model.Deployer,'presentResults',0,'fileLims',fileLims,'showDebugPlots',0);
            
            % Process truth data
            Truth = Model.Truth_VCF;
            
            % Compute results
            Model.ComputeStateOutput();
            
            Validator = Validate(configfile,Model,true);
            
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
            if strcmpi(obj.testType,'Modular')
                dataFolder = 'Data/Results/matFiles/ModularTest_4_9/';
                folderString = Model.Deployer.TruthFileName;
                tmp = split(folderString,'/');
                testNumber = tmp{3};
            elseif strcmpi(obj.testType,'100m')
                dataFolder = 'Data/Results/matFiles/100m/';
                folderString = Model.Deployer.TruthFileName;
                tmp = split(folderString,'/');
                testNumber = tmp{3};
            elseif strcmpi(obj.testType,'Simulation_030')
                dataFolder = 'Data/Results/matFiles/Simulation_4_15_030/';
                tmp = split(SensorData.TOFData,'/');
                testNumber = tmp{5};
            elseif strcmpi(obj.testType,'Simulation_085')
                dataFolder = 'Data/Results/matFiles/Simulation_4_15_085/';
                tmp = split(SensorData.TOFData,'/');
                testNumber = tmp{5};
            elseif strcmpi(obj.testType,'Simulation_140')
                dataFolder = 'Data/Results/matFiles/Simulation_4_15_140/';
                tmp = split(SensorData.TOFData,'/');
                testNumber = tmp{5};
            elseif strcmpi(obj.testType,'Simulation_195')
                dataFolder = 'Data/Results/matFiles/Simulation_4_15_195/';
                tmp = split(SensorData.TOFData,'/');
                testNumber = tmp{5};
            else
                error('invalid test type in Deployer.TruthFileName')
            end
            
            mkdir(dataFolder)
            mkdir([dataFolder 'data/']);
            save([pwd '/' dataFolder 'data/CSData' testNumber '.mat'],'CubeSatFitted');
            save([pwd '/' dataFolder 'data/TruthData' testNumber '.mat'],'TruthFitted');
            save([pwd '/' dataFolder 'data/AbsErrorData' testNumber '.mat'],'AbsoluteError');
            save([pwd '/' dataFolder 'data/XErrorData' testNumber '.mat'],'XError');
            save([pwd '/' dataFolder 'data/YErrorData' testNumber '.mat'],'YError');
            save([pwd '/' dataFolder 'data/ZErrorData' testNumber '.mat'],'ZError');
            save([pwd '/' dataFolder 'data/CSTime' testNumber '.mat'],'t_fit');
            end
        end
        
        function testError(obj)
            import VANTAGE.PostProcessing.Validate
            
            switch obj.testType
                case 'Modular'
                    obj.configDirecName = obj.configDirecNameModular;
                    testType = 'ModularTest_4_9';
                    testDef = 'Modular';
                case '100m'
                    obj.configDirecName = obj.configDirecName100m;
                    testType = '3_25_100m';
                    testDef = '100m';
                case 'Simulation_085'
                    obj.configDirecName = obj.configDirecNameSim085;
                    testType = 'Simulation/_085';
                    testDef = 'Sim085';
                case 'Simulation_030'
                    obj.configDirecName = obj.configDirecNameSim030;
                    testType = 'Simulation/_030';
                    testDef = 'Sim030';
                case 'Simulation_140'
                    obj.configDirecName = obj.configDirecNameSim140;
                    testType = 'Simulation/_140';
                    testDef = 'Sim140';
                case 'Simulation_195'
                    obj.configDirecName = obj.configDirecNameSim195;
                    testType = 'Simulation/_195';
                    testDef = 'Sim195';
            end
            
            configfiles = dir(obj.configDirecName);
            
            for iter = 1:numel(configfiles)
            %%% Housekeeping and Allocation
            rng(99);

            %%% Filenames and Configurables
            configfolder = [configfiles(iter).folder '/' configfiles(iter).name];
            manifestFilename = [configfolder '/Manifest.json'];
            SensorData = jsondecode(fileread(strcat(configfolder,'/Sensors.json')));
            
            Model = VANTAGE.PostProcessing.Model(manifestFilename,configfolder,false);

            Validator = Validate(configfolder,Model,false);
            
            %Validator.PlotResults(Model,SensorData);
            
            Validator.ErrorAnalysis(Model,SensorData,testDef);
            end
%            ______.........--=T=--.........______
%               .             |:|
%          :-. //           /""""""-.
%          ': '-._____..--""(""""""()`---.__
%           /:   _..__   ''  ":""""'[] |""`\\
%           ': :'     `-.     _:._     '"""" :
%            ::          '--=:____:.___....-"
%                              O"       O" 
%             MARSHALL LANDING PAD ALERT
%             
%             
%             MAKE THE CALL TO YOUR FUNCTION HERE AFTER FOLDER DEF
            
            matFileDirectory = [pwd '/Data/Results/matFiles'];
            Validator.masterPlotter(matFileDirectory);
            
        end
    end
    
    
    methods (Access = private)
    end

end
