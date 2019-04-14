classdef Validate
    %% Properties
    properties(Access = public)
        % Boolean for whether or not to auto update truth data
        CorrelateTruthData
        
        % Model handle class
        ModelRef
    end
    
    
    %% Public methods
    methods
        %% Class Constructor
        %
        % Constructs validate class 
        %
        % @param    
        %
        % @return   Class instance
        % 
        % @author   Dylan Bossie
        % @date     14-Apr-2019
        function obj = Validate(configFilename,ModelRef)
            configFilename = [configFilename '/Validate.json'];
            parameters = jsondecode(fileread(configFilename));
            obj.CorrelateTruthData = parameters.CorrelateTruthData;
            obj.ModelRef = ModelRef;
            
            % If user chooses to update truth data with corrections
            if obj.CorrelateTruthData
                updatedTruth = obj.PerformTruthDataCorrelation();
                obj.ModelRef.Truth_VCF = updatedTruth;
                obj.WriteUpdatedTruthData(updatedTruth,ModelRef.Deployer.TruthFileName);
            end
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
        function CubeSatFitted = fitCubeSatTraj(~,CubeSat,t,Type,t_fit,Model)
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

            XFit_CubeSat = Model.ransacLine(t,CS_X,0.15);
            YFit_CubeSat = Model.ransacLine(t,CS_Y,0.15);
            ZFit_CubeSat = Model.ransacLine(t,CS_Z,0.15);
            
            CSFitted_X = polyval(XFit_CubeSat,t_fit);
            CSFitted_Y = polyval(YFit_CubeSat,t_fit);
            CSFitted_Z = polyval(ZFit_CubeSat,t_fit);
            
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
        function [Error,XError,YError,ZError] = ProcessError(~, CubeSat, Truth)
            XError = abs(CubeSat(:,1) - Truth(:,1));
            YError = abs(CubeSat(:,2) - Truth(:,2));
            ZError = abs(CubeSat(:,3) - Truth(:,3));
            
            ErrorVec = [XError YError ZError];
            
            Error = zeros(length(ErrorVec),1);
            for i = 1:length(ErrorVec)
                Error(i) = norm(ErrorVec(i,:))*100;
            end
            
        end
        
        %% Compute mean error
        %
        % Compute mean error for all tests and assimilate cubesat results
        % 
        % @param        Model       Model class containing initial data
        %
        % @return
        %
        % @author Dylan Bossie 
        % @date   14-Apr-2019
        function [Error,XError,YError,ZError] = ComputeMeanError(~,Model)
            if strcmpi(Model.Deployer.testScenario,'Modular')
                resultsFolder = 'Data/ModularTest_4_9/Results/';
                
                AbsoluteErrorFiles = dir([resultsFolder 'AbsError*']);
                CubeSatDataFiles = dir([resultsFolder 'CSData*']);
                TruthDataFiles = dir([resultsFolder 'TruthData*']);
                XErrorFiles = dir([resultsFolder 'XError*']);
                YErrorFiles = dir([resultsFolder 'YError*']);
                ZErrorFiles = dir([resultsFolder 'ZError*']);
            end
            
            for i = 1:numel(AbsoluteErrorFiles)
                absError = load([AbsoluteErrorFiles(i).folder '/' AbsoluteErrorFiles(i).name]);
                
            end
        end
        
        function generateErrorPlot( r, mu, sigma )
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            %%% Author: Marshall Herr
            %%%
            %%% Purpose: To make nice plots of the error vs range of VANTAGE
            %%%
            %%% Inputs:
            %%%     - r: A vector of the range of each datapoint
            %%%     - mu: The mean error at each range given in r
            %%%     - sigma: The standard deviation of the error at each range given in
            %%%         r
            %%%
            %%% Outputs:
            %%%     - A cool plot
            %%%
            %%% Date Created: 22 March 2019
            %%%
            %%% Last Editted: 22 March 2019
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

            % % Testing
            % r = linspace(0,100,100);
            % mu = r.^2 ./ 100;
            % sigma = r.^(1.5) ./ 50 + 1;

            n = 1000;
            s = 10;
            [ x, y1, y2, c1 ] = deal( zeros( n, length(r) ) );

            for i = 1 : length(r)

                idx = ( (i-1)*n + 1 ) : ( i*n );
                x(idx) = r(i) .* ones(1,n)';
                y1(idx) = mu(i)+linspace(0,s*sigma(i),n)';
                y2(idx) = mu(i)-linspace(0,s*sigma(i),n)';
                c1(idx) = exp( -linspace(0,s*sigma(i),n) ./ sigma(i) );

            end

            hold on
            p1 = surf( x, y1, 0.*x, 'FaceAlpha', 'flat', ...
                'AlphaDataMapping', 'scaled', 'AlphaData', c1, 'LineStyle', 'None' );
            surf( x, y2, 0.*x, 'FaceAlpha', 'flat', 'AlphaDataMapping', 'scaled', ...
                'AlphaData', c1, 'LineStyle', 'None' )
            p2 = plot( r, mu, '-k', 'LineWidth', 2 );
            p3 = plot( r, mu+sigma, '-r', 'LineWidth', 2 );
            ylim( [ min(mu-3.*sigma), max(mu+3.*sigma) ] )
            legend( [ p1, p2, p3 ], ...
                { 'Likelyhood of Error', 'Mean Error', '1\sigma Upper Error Bound' }...
                , 'location', 'best' )

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
        function PlotResults(~,t_fit,CubeSatFitted,TruthFitted,AbsoluteError)
            warning('off','MATLAB:MKDIR:DirectoryExists');
            mkdir('./Data/ErrorOut')
            
            figure
            hold on
            plot(t_fit,CubeSatFitted{1}(:,3))
            plot(t_fit,TruthFitted{1}(:,3))
            legend('Measured Range (m)','True Range (m)','Location','SouthEast')
            title('Downrange Distance of CubeSat Measured and True Values - Fusion')
            ylabel('Range (m)')
            outFile = [pwd '/Data/ErrorOut/CubeSat1_DownrangeErrorFusion'];
            export_fig(sprintf('%s',outFile),'-png');
            
            figure
            hold on
            plot(t_fit,CubeSatFitted{1}(:,1))
            plot(t_fit,TruthFitted{1}(:,1))
            legend('Measured Horizontal (m)','True Horizontal (m)','Location','SouthEast')
            title('Horizontal Distance of CubeSat Measured and True Values - Fusion')
            ylabel('Distance (m)')
            outFile = [pwd '/Data/ErrorOut/CubeSat1_HorizontalErrorFusion'];
            export_fig(sprintf('%s',outFile),'-png');
            
            figure
            hold on
            plot(t_fit,CubeSatFitted{1}(:,2))
            plot(t_fit,TruthFitted{1}(:,2))
            legend('Measured Vertical (m)','True Vertical (m)','Location','SouthEast')
            title('Vertical Distance of CubeSat Measured and True Values - Fusion')
            ylabel('Distance (m)')
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
            outFile = [pwd '/Data/ErrorOut/CubeSat1_FullErrorFusion'];
            export_fig(sprintf('%s',outFile),'-png');
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
        
        %% Correlate truth data timestamps etc.
        %
        % func description
        % 
        % @param        name       desc
        %
        % @return       bruhmoment#2        yeetus that feetus
        %
        % @author Marshall Herr
        % @date   14-Apr-2019
        function [updatedTruth] = PerformTruthDataCorrelation(~)
            updatedTruth = [420 69];
        end
        
        %% Write out new truth data with corrections
        %
        % func description
        % 
        % @param        name       desc
        %
        % @return       bruhmoment#2        yeetus that feetus
        %
        % @author Marshall Herr
        % @date   14-Apr-2019
        function [] = WriteUpdatedTruthData(~)
        end
    end
    
    
    
    
    
    
end