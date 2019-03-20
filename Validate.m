classdef Validate
    %% Properties
    properties(Access = public)
        %
        
    end
    
    
    %% Public methods
    methods
        %% Class Constructor
        %
        % Constructs class 
        %
        % @param    
        %
        % @return   Class instance
        % 
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function obj = Validate(obj)
            
        end
        
        %% Validate TOF
        %
        % 
        % 
        % @param    
        %
        % @return 
        %
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function validateTOF(obj)
            %%% Housekeeping and Allocation
            close all;
            rng(99);
            tube = 6;
            fileLims = [19 40];

            %%% Filenames and Configurables
            switch tube
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

            %%% Initialize VANTAGE Model
            Model = VANTAGE.PostProcessing.Model(manifestFilename,configDirecName);

            %%% Present Errors
            obj.TOFpresentErrorsVsReqs(Model,SensorData,Model.Deployer.TruthFileName);
        end
        
        %% Validate Optical
        %
        % 
        % 
        % @param    
        %
        % @return 
        %
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function validateOptical(obj)
            
        end
        
        %% Validate VANTAGE
        %
        % 
        % 
        % @param    
        %
        % @return 
        %
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function validateVantage(obj)
            
        end
    end
    %% Private methods
    methods (Access = private)
        %
        % Present TOF Errors vs Reqs
        %
        % @param    
        %
        % @return   
        %
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function TOFpresentErrorsVsReqs(obj,Model,SensorData,truthFileName)
            % TOF Processing
            fileLims = [1 inf];
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
                Model.Deployer,'presentResults',1,'fileLims',fileLims,'showDebugPlots',0);
            % Truth Data processing
            Truth = obj.processTruthData(truthFileName);
            % Loop over cubesats
            for i = 1:Truth.numCubeSats
                % Truth Data interpolation
                Truth.Cubesat(i).timeInterp = Model.Deployer.CubesatArray(i).time;
                Truth.Cubesat(i).posInterp  = interp1(Truth.t,...
                    Truth.Cubesat(i).pos,Truth.Cubesat(i).timeInterp);
                % Calculate errors
                Error.Cubesat(i).time = Truth.Cubesat(i).timeInterp;
                Error.Cubesat(i).err  = vecnorm(Truth.Cubesat(i).posInterp-Model.Deployer.CubesatArray(i).centroids_VCF',2,2);
                % Plot
                figure
                hold on
                grid on
                grid minor
                plot(vecnorm(Truth.Cubesat(i).posInterp,2,2),Error.Cubesat(i).err.*100);
                xlabel('Range, m')
                ylabel('Error, cm')
                hold off
            end
        end
        
        %
        % Processes truth data
        %
        % @param    truthFilename   full filename of truth data JSON file
        %
        % @return   struct containing truth data for the current execution
        %           of VANTAGE post processing
        %
        % @author   Joshua Kirby
        % @date     03-Mar-2019
        function Truth = processTruthData(~,truthFilename)
            % Read json truth file
            tmp = jsondecode(fileread(truthFilename));
            % extract timesteps
            Truth.t = [tmp.t];
            % Order Cubesats [first-out to last-out]
            cubesatNamesUnordered = fieldnames(tmp(1).pos);
            for i = 1:length(cubesatNamesUnordered)
                z(i) = tmp(1).pos.(cubesatNamesUnordered{i})(3);
            end
            [~,I] = sort(z,'descend');
            cubesatNames = cubesatNamesUnordered(I);
            % Extract cubesat position data in meters
            for i = 1:length(cubesatNames)
                for j = 1:length(tmp)
                    Truth.Cubesat(i).pos(j,:) = tmp(j).pos.(cubesatNames{i})./100;
                end
            end
            % Extract number cubesats
            Truth.numCubeSats = length(cubesatNames);
        end
    end
    
    
    
    
    
    
end