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
        function obj = Validate()
            
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
            %testType = 'Simulation';
            %testType = 'Modular';
            testType = '100m';
            simtube = 6;
            t100mTestNum = 6;

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
                if 0
                    configDirecName = 'Config/Testing/TOF/Modular_3-22-19_Cropped_TOF';
                    manifestFilename = 'Config/Testing/TOF/Modular_3-22-19_Cropped_TOF/Manifest.json';
                    SensorData = jsondecode(fileread('config/Testing/TOF/Modular_3-22-19_Cropped_TOF/Sensors.json'));
                else
                    configDirecName = 'Config/Testing/TOF/Modular_040819_Test3';
                    manifestFilename = 'Config/Testing/TOF/Modular_040819_Test3/Manifest.json';
                    SensorData = jsondecode(fileread('config/Testing/TOF/Modular_040819_Test3/Sensors.json'));
                end
            elseif strcmpi(testType,'100m')
                switch t100mTestNum
                    case 1
                        configDirecName = 'Config/Final_Tests/100m_032519/Test 1';
                        manifestFilename = 'Config/Final_Tests/100m_032519/Test 1/Manifest.json';
                        SensorData = jsondecode(fileread('Config/Final_Tests/100m_032519/Test 1/Sensors.json'));
                    case 6
                        configDirecName = 'Config/Final_Tests/100m_032519/Test 6';
                        manifestFilename = 'Config/Final_Tests/100m_032519/Test 6/Manifest.json';
                        SensorData = jsondecode(fileread('Config/Final_Tests/100m_032519/Test 6/Sensors.json'));
                    otherwise
                        error('Invalid t100mTestNum')
                end
            else
                error('Invalid testType')
            end

            %%% Initialize VANTAGE Model
            Model = VANTAGE.PostProcessing.Model(manifestFilename,configDirecName);

            %%% Present Errors
            obj.TOFpresentErrorsVsReqs(Model,SensorData);
        end
        
        %% Validate Optical
        %
        % 
        % 
        % @param    
        %
        % @return 
        %
        % @author  
        % @date     
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
        % @author   
        % @date     
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
        function TOFpresentErrorsVsReqs(~,Model,SensorData)
            % TOF Processing
            fileLims = [1 inf];
            %%%%%%%%%%%%%%%%%%%%%%%%%
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
                Model.Deployer,'presentResults',1,'fileLims',fileLims,'showDebugPlots',0);
            % Get Truth Data
            Truth = Model.Truth_VCF;
            %%%%%%%%%%%%%%%%%%%%%%%%%
            % Loop over cubesats
            %%% Absolute Error Plots
            titlefontsize = 20;
            labelfontsize = 16;
            figure
            hold on
            grid on
            grid minor
            xlabel('Range, m','fontsize',labelfontsize)
            ylabel('Error, cm','fontsize',labelfontsize)
            title(['TOF Absolute Centroid Error Vs Range'],'fontsize',titlefontsize)
            linewidth = 2;
            for i = 1:Truth.numCubeSats
                % Truth Data interpolation
                Truth.Cubesat(i).timeInterp = Model.Deployer.CubesatArray(i).time;
                Truth.Cubesat(i).posInterp  = interp1(Truth.t,...
                    Truth.Cubesat(i).pos,Truth.Cubesat(i).timeInterp);
                % Calculate errors
                Error.Cubesat(i).time = Truth.Cubesat(i).timeInterp;
                Error.Cubesat(i).err  = vecnorm(Truth.Cubesat(i).posInterp-Model.Deployer.CubesatArray(i).centroids_VCF',2,2);
                % Plot
                plot(vecnorm(Truth.Cubesat(i).posInterp,2,2),Error.Cubesat(i).err.*100,'linewidth',linewidth);
                legendstrings{i} = ['Cubesat ',num2str(i)];
            end
            legend(legendstrings,'location','eastoutside')
            hold off
            %%% Errors vs requirements plot
            err = [];
            for i = 1:Truth.numCubeSats
                err = [err Error.Cubesat(i).err'];
            end
            onesigmastd = quantile(err,normcdf(1)-normcdf(-1));
            fprintf(['One Sigma Std over all TOF points: ',num2str(onesigmastd),' m\n\n']);
        end
    end
    
    
    
    
    
    
end