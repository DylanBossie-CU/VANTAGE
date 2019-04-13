classdef Test_Fusion < matlab.unittest.TestCase
    properties
        configDirecName = 'Config/Testing/Dylan';
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        function test100mData(obj)
            %%% Housekeeping and Allocation
            close all;
            rng(99);
            %testType = 'Simulation';
            %testType = 'Modular';
            testType = 'Modular';
            simtube = 6;
            
            % Create validation class for validation methods
            import Validate
            Validation = Validate();

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
            
            % pos - cell array (n x 3) containing 3-D VCF positions of
            % CubeSats over the full testing range:
            % pos(:,i) - CubeSat_i from 0-z
            [pos,t] = Model.ComputeStateOutput();
            
            CubeSatFitted = cell(Model.Deployer.numCubesats,1);
            TruthFitted = cell(Model.Deployer.numCubesats,1);
            AbsoluteError = cell(Model.Deployer.numCubesats,1);
            for i=1:Model.Deployer.numCubesats
               CubeSat = Model.Deployer.CubesatArray(i);
               data_t = CubeSat.time(end);
               CubeSatFitted{i} = Validation.fitCubeSatTraj(CubeSat.centroids_VCF,CubeSat.time,'CS',data_t);
               TruthFitted{i} = Validation.fitCubeSatTraj(Truth.Cubesat(i).pos,Model.Truth_VCF.t,'Truth',data_t);
               
               AbsoluteError{i} = Validation.ProcessError(CubeSatFitted{i},TruthFitted{i});
            end
            
            if false
                tmp = horzcat(pos{:,1})';
                figure
                plot3(tmp(:,1),tmp(:,2),tmp(:,3))
                hold on
                plot3(Truth.Cubesat(1).pos(:,1),Truth.Cubesat(1).pos(:,2),Truth.Cubesat(1).pos(:,3))
                grid on
                xlabel('X (m)')
                ylabel('Y (m)')
                zlabel('Z (m)')
                legend('VANTAGE Estimated Trajectory','Truth Measured Trajectory')
                title(sprintf('%s: Trajectory 3D',Model.Deployer.TruthFileName(1:end-11)),'Interpreter','none')
            end
            
            Validation.PlotResults(CubeSatFitted,TruthFitted,AbsoluteError);
        end
        
    end
    
    
    methods (Access = private)
    end

end
