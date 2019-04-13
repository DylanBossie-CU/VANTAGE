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
            testType = 'Modular';
            %testType = '100m';
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
                configDirecName = 'Config/Testing/TOF/Modular_3-22-19_Cropped_TOF';
                manifestFilename = 'Config/Testing/TOF/Modular_3-22-19_Cropped_TOF/Manifest.json';
                SensorData = jsondecode(fileread('config/Testing/TOF/Modular_3-22-19_Cropped_TOF/Sensors.json'));
            elseif strcmpi(testType,'100m')
                configDirecName = 'Config/Testing/TOF/100m 3-25-19 Cropped TOF';
                manifestFilename = 'Config/Testing/TOF/100m 3-25-19 Cropped TOF/Manifest.json';
                SensorData = jsondecode(fileread('config/Testing/TOF/100m 3-25-19 Cropped TOF/Sensors.json'));
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
        
        %% Fit polynomial to CubeSat data
        %
        % Apply a linear fit to a CubeSat VCF XYZ input
        % 
        % @param        CubeSat         Cell array or array of X Y Z values
        % @param        t               Timestamps
        %
        % @return       CubeSatFitted   Array of XYZ values fitted
        %
        % @author Dylan Bossie 
        % @date   11-Apr-2019
        function CubeSatFitted = fitCubeSatTraj(~,CubeSat,t,Type,data_t)
            CS_X = zeros(length(CubeSat),1);
            CS_Y = zeros(length(CubeSat),1);
            CS_Z = zeros(length(CubeSat),1);
            for i = 1:length(CubeSat)
                if strcmpi(Type,'CS')
                    tmp = CubeSat{i};
                elseif strcmpi(Type,'Truth')
                    tmp = CubeSat(i,:);
                else
                    error('No')
                end
                
                CS_X(i) = tmp(1);
                CS_Y(i) = tmp(2);
                CS_Z(i) = tmp(3);
            end
            
            if length(t(:,1)) == 1
                t = t';
            end

            XFit_CubeSat = polyfit(t,CS_X,1);
            YFit_CubeSat = polyfit(t,CS_Y,1);
            ZFit_CubeSat = polyfit(t,CS_Z,1);
            
            fitPoints = linspace(0,data_t,1000);
            
            CSFitted_X = polyval(XFit_CubeSat,fitPoints);
            CSFitted_Y = polyval(YFit_CubeSat,fitPoints);
            CSFitted_Z = polyval(ZFit_CubeSat,fitPoints);
            
            CubeSatFitted = [CSFitted_X; CSFitted_Y; CSFitted_Z]';
        end
        
        %% Plot error
        %
        % Compute error between CubeSat and truth, plot results
        % 
        % @param    
        %
        % @return 
        %
        % @author Dylan Bossie 
        % @date   11-Apr-2019
        function Error = ProcessError(~, CubeSat, Truth)
            XError = abs(CubeSat(:,1) - Truth(:,1));
            YError = abs(CubeSat(:,2) - Truth(:,2));
            ZError = abs(CubeSat(:,3) - Truth(:,3));
            
            ErrorVec = [XError YError ZError];
            
            Error = zeros(length(ErrorVec),1);
            for i = 1:length(ErrorVec)
                Error(i) = norm(ErrorVec(i,:))*100;
            end
            
        end
        
        %% Plot results
        %
        % Plot final fitted results
        % 
        % @param        CubeSatFitted       Cell array of CubeSats with
        %                               corresponding fitted measurements
        % @param        TruthFitted         Cell array of truth data for
        %                               corrresponding CubeSats
        % @param        AbsoluteError       Distances between meas. and
        %                                   truth values
        %
        % @return 
        %
        % @author Dylan Bossie
        % @date   11-Apr-2019
        function PlotResults(obj,CubeSatFitted,TruthFitted,AbsoluteError)
            
            figure
            hold on
            plot(CubeSatFitted{1}(:,3))
            plot(TruthFitted{1}(:,3))
            legend('Measured Range (m)','True Range (m)','Location','SouthEast')
            title('Downrange Distance of CubeSat Measured and True Values - Fusion')
            ylabel('Range (m)')
            outFile = [pwd '/Data/ErrorOut/CubeSat1_DownrangeErrorFusion'];
            export_fig(sprintf('%s',outFile),'-png');
            
            figure
            hold on
            plot(CubeSatFitted{1}(:,1))
            plot(TruthFitted{1}(:,1))
            legend('Measured Horizontal (m)','True Horizontal (m)','Location','SouthEast')
            title('Horizontal Distance of CubeSat Measured and True Values - Fusion')
            ylabel('Range (m)')
            outFile = [pwd '/Data/ErrorOut/CubeSat1_HorizontalErrorFusion'];
            export_fig(sprintf('%s',outFile),'-png');
            
            figure
            hold on
            plot(CubeSatFitted{1}(:,2))
            plot(TruthFitted{1}(:,2))
            legend('Measured Vertical (m)','True Vertical (m)','Location','SouthEast')
            title('Vertical Distance of CubeSat Measured and True Values - Fusion')
            ylabel('Range (m)')
            outFile = [pwd '/Data/ErrorOut/CubeSat1_VerticalErrorFusion'];
            export_fig(sprintf('%s',outFile),'-png');
            
            
            figure
            hold on
            title('Absolute Error between CubeSat Measurements and True Values')
            ylabel('Error (cm)')
            xlabel('Range (m)')
            for i = 1:length(CubeSatFitted)
                CubeSat = CubeSatFitted{i};
                Error = AbsoluteError{i};
                plot(CubeSat(:,3),Error)
            end
            
            legendEntries = cell(length(CubeSatFitted)+1,1);
            for i = 1:length(CubeSatFitted)
                legendEntries{i} = strcat('CubeSat ',num2str(i),' Error');
            end
            legendEntries{end} = 'Error Requirement';
            errorReq = zeros(50,1) + 10;
            errorReq_X = linspace(1,10,50);
            plot(errorReq_X,errorReq,'LineWidth',2,'Color',[0.7 0 0])
            legend(legendEntries,'Location','NorthWest');
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
        end
    end
    
    
    
    
    
    
end