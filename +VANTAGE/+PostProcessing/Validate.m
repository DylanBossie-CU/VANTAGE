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
                % I'm a hardcoded value for which CubeSat is most trusted,
                % you should change me when you know how to determine this
                % in general!
                mostTrustedCubesat = 1;
                % Extracting measured data
                MeasuredDataArray = obj.ModelRef.Deployer.CubesatArray;
                % Extracting truth data
                TruthData = obj.ModelRef.Truth_VCF;
                % Updating truth data
%                 [updatedTruth,dt,n_vec,theta,offset_vec] = obj.PerformTruthDataCorrelation(MeasuredDataArray,TruthData,mostTrustedCubesat);
                [updatedTruth,dt,n_vec,theta,offset_vec] = obj.PerformTruthDataCorrelationMultiple(MeasuredDataArray,TruthData);
                % Replacing old truth data with new truth data
                obj.ModelRef.Truth_VCF = updatedTruth;
                % Writting new truth data to json file
                obj.WriteUpdatedTruthData(updatedTruth,ModelRef.Deployer.TruthFileName,dt,n_vec,theta,offset_vec);
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
        
        %% Correlate the truth data of a single, most trusted CubeSat
        %
        % Preforms timestamp corrections, data rotation, and data
        % translation to the truth data input such that it matches the TOF
        % data as closely as possible for the most trusted CubeSat
        % 
        % @param   MeasuredDataArray   a crazy struct which has position
        %                              and time measurements burried
        %                              somewhere in it
        %
        % @param   TruthData   this is a crazy struct with more in it than
        %                      I comprehend
        %
        % @return   updatedTruth   another crazy struct formatted
        %                          identically to TruthData with the
        %                          timestamp corrected, rotated, translated
        %                          truth data within it
        %
        % @return   dt   The time shifting scalar applied to the truth data
        %                timestamps
        %
        % @return   n_vec   The vector about which the truth data is
        %                   rotated
        %
        % @return   theta   The angle [rad] of the rotation around n_vec
        %
        % @return   offset_vec   The translation applied to the truth data
        %
        % @author Marshall Herr
        % @date   14-Apr-2019
        function [updatedTruth,dt,n_vec,theta,offset_vec] = PerformTruthDataCorrelation(obj,MeasuredDataArray,TruthData,mostTrustedCubesat,should_debug)
            %{
            First thing is first, I am going to unpackage TruthData into
            the variables I used when I originally wrote this code. They
            are:
            
            - VANTAGE_Data: a struct with the following fields as they
                            pertain to the VANTAGE measurements
                > t: Time column vector
                > x: X position column vector
                > y: Y position column vector
                > z: Z position column vector
                > d: A length 2 vector, first element: minimum distance
                     on which ToF is trusted, second element: maximum
                     distance on which ToF is trusted
            - Truth_Data: a struct with the following fields as they pertain
                          to the Truth measurements
                > t: Time column vector
                > x: X position column vector
                > y: Y position column vector
                > z: Z position column vector
            %}
            CubeSatofInterest = MeasuredDataArray(1,mostTrustedCubesat);
            N = length( CubeSatofInterest.time );
            [ VANTAGE_Data.t, VANTAGE_Data.x, VANTAGE_Data.y, ...
                VANTAGE_Data.z ] = deal( NaN .* ones( N, 1 ) );
            for i = 1 : N
                VANTAGE_Data.t(i) = CubeSatofInterest.time(i);
                VANTAGE_Data.x(i) = CubeSatofInterest.centroids_VCF{i}(1);
                VANTAGE_Data.y(i) = CubeSatofInterest.centroids_VCF{i}(2);
                VANTAGE_Data.z(i) = CubeSatofInterest.centroids_VCF{i}(3);
            end
            % thanks josh!
            VANTAGE_Data.d = CubeSatofInterest.TOFrange;
            
            CubeSatofInterest = TruthData.Cubesat(mostTrustedCubesat);
            N = length( TruthData.t );
            [ Truth_Data.t, Truth_Data.x, Truth_Data.y, Truth_Data.z ] =...
                deal( NaN .* ones( N, 1 ) );
            for i = 1 : N
                Truth_Data.t(i) = TruthData.t(i);
                Truth_Data.x(i) = CubeSatofInterest.pos(i,1);
                Truth_Data.y(i) = CubeSatofInterest.pos(i,2);
                Truth_Data.z(i) = CubeSatofInterest.pos(i,3);
            end
            
            %{
            Now that that's done, we can blindly paste my algorith below!
            %}
            [dt,n_vec,theta,offset_vec] = obj.CorrelationMachine(VANTAGE_Data,Truth_Data);
            
            %{
            Alright! Algorithm over, time to repackage TruthData and return
            it as updatedTruth
            I apply a time shift of +dt to TruthData.Cubesat(:).t
            TruthData.t is a row vector
            I apply a rotation of theta around n_vec to TruthData.Cubesat(:).pos
            I apply a translation of offset_vec to TruthData.Cubesat(:).pos
            TruthData.Cubesat(:).pos is [x,y,z] where x, y, & z are column vectors
            %}
            updatedTruth = TruthData; % best to start here I guess...
            updatedTruth.t = updatedTruth.t + dt;
            
            % theft of a rotation matrix of theta radians about n_vec
            R = [ cos(theta) + n_vec(1)^2 * ( 1 - cos(theta) ), ...
                      n_vec(1) * n_vec(2) * ( 1 - cos(theta) ) - n_vec(3) * sin(theta), ...
                      n_vec(1) * n_vec(3) * ( 1 - cos(theta) ) + n_vec(2) * sin(theta); ...
                      n_vec(2) * n_vec(1) * ( 1 - cos(theta) ) + n_vec(3) * sin(theta), ...
                      cos(theta) + n_vec(2)^2 * ( 1 - cos(theta) ), ...
                      n_vec(2) * n_vec(3) * ( 1 - cos(theta) ) - n_vec(1) * sin(theta); ...
                      n_vec(3) * n_vec(1) * ( 1 - cos(theta) ) - n_vec(2) * sin(theta), ...
                      n_vec(3) * n_vec(2) * ( 1 - cos(theta) ) + n_vec(1) * sin(theta), ...
                      cos(theta) + n_vec(3)^2 * ( 1 - cos(theta) ) ];
            
            N = length( updatedTruth.t );
            M = length( updatedTruth.Cubesat );
            % for each time
            for i = 1 : N
                % for each cubesat
                for j = 1 : M
                    % extract the ith position vector (transpose to turn it
                    % into a column vector)
                    pos_vec = updatedTruth.Cubesat(j).pos(i,:)';
                    % rotate it theta [rad] about n_vec
                    pos_vec = R * pos_vec;
                    % translate it by +offset_vec (transpose to turn it
                    % back into a row vector)
                    pos_vec = pos_vec' + offset_vec;
                    % update updatedTruth.Cubesat(j).pos(i,:);
                    updatedTruth.Cubesat(j).pos(i,:) = pos_vec;
                end
            end
            % done!
        end
        
        %% This does the grunt work for my Correlation fcns
        %
        % Determines the timeshift, rotation vector, rotation angle, and
        % trnslation for truthdata wrt measurements
        % 
        % @param   VANTAGE_Data   This is a struct that holds the measured
        %                         times and positions for a single cubesat
        %
        % @param   Truth_Data   This is a struct that holds the truth
        %                       times and positions for the same cubesat
        %
        % @param   should_debug   Boolian for if you want plot spam or not
        %
        % @return   dt   The time shifting scalar applied to the truth data
        %                timestamps
        %
        % @return   n_vec   The vector about which the truth data is
        %                   rotated
        %
        % @return   theta   The angle [rad] of the rotation around n_vec
        %
        % @return   offset_vec   The translation applied to the truth data
        %
        % @author Marshall Herr
        % @date   15-Apr-2019
        function [dt,n_vec,theta,offset_vec] = CorrelationMachine(~,VANTAGE_Data,Truth_Data,should_debug)
            
            if nargin < 4
                should_debug = 0;
            end
            
            %{
            First we have to extract the VANTAGE measurement values we trust
            %}
            VANTAGE_Data_Distance = sqrt( VANTAGE_Data.x.^2 + VANTAGE_Data.y.^2 + ...
                VANTAGE_Data.z.^2 );
            
            VANTAGE_Index_Start = find( VANTAGE_Data_Distance >= VANTAGE_Data.d(1), ...
                1, 'first' );
            VANTAGE_Index_End = find( VANTAGE_Data_Distance <= VANTAGE_Data.d(2), ...
                1, 'last' );
            VANTAGE_Index = VANTAGE_Index_Start : VANTAGE_Index_End;
            
            %{
            Now we will generate a 3D linear least squares fit (f) of the trusted
            VANTAGE 3D data ([x,y,z]):
            f_x = m_x * t + b_x
            [ m_x; b_x ] = inv( A' * A ) * ( A' * x )
            A = [ t, 1 ];
            same process for y and z
            f = [ f_x; f_y; f_z ]
            %}
            VANTAGE_Fit.t = VANTAGE_Data.t( VANTAGE_Index );
            A = [ VANTAGE_Fit.t, ones( size( VANTAGE_Fit.t ) ) ];
            intermediary = ( A' * A ) \ ( A' * VANTAGE_Data.x( VANTAGE_Index ) );
            m = intermediary(1);
            b = intermediary(2);
            VANTAGE_Fit.f_x = @(t) m .* t + b;
            
            intermediary = ( A' * A ) \ ( A' * VANTAGE_Data.y( VANTAGE_Index ) );
            m = intermediary(1);
            b = intermediary(2);
            VANTAGE_Fit.f_y = @(t) m .* t + b;
            
            intermediary = ( A' * A ) \ ( A' * VANTAGE_Data.z( VANTAGE_Index ) );
            m = intermediary(1);
            b = intermediary(2);
            VANTAGE_Fit.f_z = @(t) m .* t + b;
            
            if should_debug
                
                figure;
                scatter3( VANTAGE_Data.x( VANTAGE_Index ), ...
                    VANTAGE_Data.y( VANTAGE_Index ), ...
                    VANTAGE_Data.z( VANTAGE_Index ), 2, ...
                    VANTAGE_Data.t( VANTAGE_Index ) )
                hold on
                plot3( VANTAGE_Fit.f_x( VANTAGE_Data.t( VANTAGE_Index ) ), ...
                    VANTAGE_Fit.f_y( VANTAGE_Data.t( VANTAGE_Index ) ), ...
                    VANTAGE_Fit.f_z( VANTAGE_Data.t( VANTAGE_Index ) ), ...
                    '-r', 'LineWidth', 2 )
                title( 'VANTAGE Data & Fit' )
                
            end
            
            %{
            Now we need to do the same for the truth data, but only over the range
            which corresponds to the positions where the truth data is accurate
            %}
            Truth_Data_Distance = sqrt( Truth_Data.x.^2 + Truth_Data.y.^2 + ...
                Truth_Data.z.^2 );
            
            Truth_Index_Start = find( Truth_Data_Distance >= VANTAGE_Data.d(1), 1, 'first' );
            Truth_Index_End = find( Truth_Data_Distance <= VANTAGE_Data.d(2), 1, 'last' );
            Truth_Index = Truth_Index_Start:Truth_Index_End;
            
            %{
            Now that we know what values of the Truth data line up with the trusred
            VANTAGE data, we can fit them
            %}
            Truth_Fit.t = Truth_Data.t( Truth_Index );
            A = [ Truth_Fit.t, ones( size( Truth_Fit.t ) ) ];
            intermediary = ( A' * A ) \ ( A' * Truth_Data.x( Truth_Index ) );
            m = intermediary(1);
            b = intermediary(2);
            Truth_Fit.f_x = @(t) m .* t + b;
            
            intermediary = ( A' * A ) \ ( A' * Truth_Data.y( Truth_Index ) );
            m = intermediary(1);
            b = intermediary(2);
            Truth_Fit.f_y = @(t) m .* t + b;
            
            intermediary = ( A' * A ) \ ( A' * Truth_Data.z( Truth_Index ) );
            m = intermediary(1);
            b = intermediary(2);
            Truth_Fit.f_z = @(t) m .* t + b;
            
            if should_debug
                
                figure;
                scatter3( Truth_Data.x( Truth_Index ), ...
                    Truth_Data.y( Truth_Index ), ...
                    Truth_Data.z( Truth_Index ), 2, ...
                    Truth_Data.t( Truth_Index ) )
                hold on
                plot3( Truth_Fit.f_x( Truth_Data.t( Truth_Index ) ), ...
                    Truth_Fit.f_y( Truth_Data.t( Truth_Index ) ), ...
                    Truth_Fit.f_z( Truth_Data.t( Truth_Index ) ), ...
                    '-r', 'LineWidth', 2 )
                title( 'Truth Data & Fit' )
                
            end
            
            %{
            Now we can determine the best fit lines to the distance line generated by
            X_3D_Fit.vec, but this time we will be fitting function for time
            %}
            Truth_Fit.f_dist_3D = @(t) sqrt( Truth_Fit.f_x(t).^2 + ...
                Truth_Fit.f_y(t).^2 + Truth_Fit.f_z(t).^2 );
            
            A = [ Truth_Fit.f_dist_3D( Truth_Fit.t ), ones( size( Truth_Fit.t ) ) ];
            intermediary = ( A' * A ) \ ( A' * Truth_Fit.t );
            m = intermediary(1);
            b = intermediary(2);
            Truth_Fit.f_t = @(d) m .* d + b;
            
            VANTAGE_Fit.f_dist_3D = @(t) sqrt( VANTAGE_Fit.f_x(t).^2 + ...
                VANTAGE_Fit.f_y(t).^2 + VANTAGE_Fit.f_z(t).^2 );
            
            A = [ VANTAGE_Fit.f_dist_3D( VANTAGE_Fit.t ), ones( size( VANTAGE_Fit.t ) ) ];
            intermediary = ( A' * A ) \ ( A' * VANTAGE_Fit.t );
            m = intermediary(1);
            b = intermediary(2);
            VANTAGE_Fit.f_t = @(d) m .* d + b;
            
            if should_debug
                
                figure;
                plot( VANTAGE_Data.t( VANTAGE_Index ), ...
                    VANTAGE_Data_Distance( VANTAGE_Index ), 'ok' )
                hold on
                plot( VANTAGE_Data.t( VANTAGE_Index ), ...
                    VANTAGE_Fit.f_dist_3D( VANTAGE_Data.t( VANTAGE_Index ) ), ...
                    '-r', 'LineWidth', 2 )
                plot( VANTAGE_Fit.f_t( VANTAGE_Fit.f_dist_3D( VANTAGE_Data.t( VANTAGE_Index ) ) ), ...
                    VANTAGE_Fit.f_dist_3D( VANTAGE_Data.t( VANTAGE_Index ) ), ...
                    '-b', 'LineWidth', 2 )
                title( 'VANTAGE Distance & Dist Fit & Time Fit' )
                
                figure;
                plot( Truth_Data.t( Truth_Index ), ...
                    Truth_Data_Distance( Truth_Index ), 'ok' )
                hold on
                plot( Truth_Data.t( Truth_Index ), ...
                    Truth_Fit.f_dist_3D( Truth_Data.t( Truth_Index ) ), ...
                    '-r', 'LineWidth', 2 )
                plot( Truth_Fit.f_t( Truth_Fit.f_dist_3D( Truth_Data.t( Truth_Index ) ) ), ...
                    Truth_Fit.f_dist_3D( Truth_Data.t( Truth_Index ) ), ...
                    '-b', 'LineWidth', 2 )
                title( 'Truth Distance & Dist Fit & Time Fit' )
                
            end
            
            %{
            Now we will numerically minimize the function:
            ( VANTAGE_Fit.f_t( VANTAGE_Fit.t ) - Truth_Fit.f_t( Truth_Fit.t + dt ) ).^2
            by varying dt to adequate precision
            %}
            VANTAGE_t = linspace( min( VANTAGE_Fit.t ), max( VANTAGE_Fit.t ), 1000 )';
            
            % note we need to make the distance traveled the same for both time vectors
            Truth_Start_Distance = min( VANTAGE_Fit.f_dist_3D( VANTAGE_t ) );
            Truth_Start_Time = Truth_Fit.f_t( Truth_Start_Distance );
            Truth_Stop_Distance = max( VANTAGE_Fit.f_dist_3D( VANTAGE_t ) );
            Truth_Stop_Time = Truth_Fit.f_t( Truth_Stop_Distance );
            Truth_t = linspace( Truth_Start_Time, Truth_Stop_Time, 1000 )';
            
            % initializing things for the while loop
            dt_error_vec = Truth_Fit.f_t( Truth_Fit.f_dist_3D( Truth_t ) ) - ...
                VANTAGE_Fit.f_t( VANTAGE_Fit.f_dist_3D( VANTAGE_t ) );
            dt_error_mean = mean( dt_error_vec );
            dt_error_squares = sum( dt_error_vec.^2 );
            dt_sign = sign( dt_error_mean );
            dt_change = dt_sign * 10^( round( log10( abs( dt_error_mean ) ) + 1 ) );
            
            precision_current = abs( dt_change );
            precision_required = 1e-12; % something sufficiently small chosen
            dt = 0;
            
            % debug stuff
            if should_debug
                
                figure;
                plot( VANTAGE_Fit.f_t( VANTAGE_Fit.f_dist_3D( VANTAGE_t ) ), ...
                    VANTAGE_Fit.f_dist_3D( VANTAGE_t ), ...
                    '-k', 'LineWidth', 2 )
                hold on
                plot( Truth_Fit.f_t( Truth_Fit.f_dist_3D( Truth_t ) ), ...
                    Truth_Fit.f_dist_3D( Truth_t ), ...
                    '-r', 'LineWidth', 2 )
                title( 'VANTAGE Time Fit & Truth Time Fit' )
                
                prev_squares = dt_error_squares;
                iteration = 1;
                T_t{iteration} = Truth_Fit.f_t( Truth_Fit.f_dist_3D( Truth_t ) );
                T_d{iteration} = Truth_Fit.f_dist_3D( Truth_t );
                
            end
            
            while precision_current > precision_required
                
                % test out the next point in the same direction as last time
                dt_error_vec_new = ...
                    Truth_Fit.f_t( Truth_Fit.f_dist_3D( Truth_t ) ) - ...
                    VANTAGE_Fit.f_t( VANTAGE_Fit.f_dist_3D( VANTAGE_t ) ) + ...
                    dt + dt_change;
                dt_error_squares_new = sum( dt_error_vec_new.^2 );
                
                % if it is an improvement, use it
                if dt_error_squares_new < dt_error_squares
                    
                    dt = dt + dt_change;
                    dt_error_squares = dt_error_squares_new;
                    
                else % mignt be time to switch directions
                    
                    dt_error_vec_new = ...
                        Truth_Fit.f_t( Truth_Fit.f_dist_3D( Truth_t ) ) - ...
                        VANTAGE_Fit.f_t( VANTAGE_Fit.f_dist_3D( VANTAGE_t ) ) + ...
                        dt - dt_change;
                    dt_error_squares_new = sum( dt_error_vec_new.^2 );
                    
                    % if it is an improvement, use it
                    if dt_error_squares_new < dt_error_squares
                        
                        dt_change = -dt_change;
                        dt = dt + dt_change;
                        dt_error_squares = dt_error_squares_new;
                        
                    else % it is at a local minimum for this stepsize, decrease stepsize
                        
                        dt_change = dt_change / 10;
                        
                    end
                    
                end
                
                % update the precision
                precision_current = abs( dt_change );
                
                % degub (lol) stuff
                if should_debug
                    
                    if dt_error_squares < prev_squares
                        
                        prev_squares = dt_error_squares;
                        iteration = iteration + 1;
                        T_t{iteration} = Truth_t + dt;
                        T_d{iteration} = Truth_Fit.f_dist_3D( Truth_t );
                        
                    end
                    
                end
                
            end
            
            if should_debug
                
                colors = [ 0, 0, 0; jet( length( T_t ) - 1 ) ];
                figure;
                plot( VANTAGE_t, VANTAGE_Fit.f_dist_3D( VANTAGE_t ), '-ok', ...
                    'LineWidth', 2 )
                hold on
                plot( T_t{1}, T_d{1}, '-o', 'color', colors(1,:), 'LineWidth', 2 )
                
                for i = 2 : length( T_t )
                    
                    plot( T_t{i}, T_d{i}, '-', 'color', colors(i,:), 'LineWidth', 2 )
                    
                end
                
            end
            
            % Apply dt correction to Truth timestamp
            Truth_Data.t = Truth_Data.t + dt;
            
            % Apply dt correction to Truth fits
            Truth_Fit.t = Truth_Fit.t + dt;
            Truth_Fit.x = @(t) Truth_Fit.x( t - dt );
            Truth_Fit.y = @(t) Truth_Fit.y( t - dt );
            Truth_Fit.z = @(t) Truth_Fit.z( t - dt );
            
            %{
            Now it is time to rotate the truth data to be aligned with the VANTAGE data
            %}
            VANTAGE_vector = [ VANTAGE_Fit.f_x( max( VANTAGE_t ) + 1 ), ...
                               VANTAGE_Fit.f_y( max( VANTAGE_t ) + 1 ), ...
                               VANTAGE_Fit.f_z( max( VANTAGE_t ) + 1 ) ] - ...
                             [ VANTAGE_Fit.f_x( min( VANTAGE_t ) ), ...
                               VANTAGE_Fit.f_y( min( VANTAGE_t ) ), ...
                               VANTAGE_Fit.f_z( min( VANTAGE_t ) ) ];
            Truth_vector = [ Truth_Fit.f_x( min( VANTAGE_t ) + 1 ), ...
                             Truth_Fit.f_y( min( VANTAGE_t ) + 1 ), ...
                             Truth_Fit.f_z( min( VANTAGE_t ) + 1 ) ] - ...
                           [ Truth_Fit.f_x( min( VANTAGE_t ) ), ...
                             Truth_Fit.f_y( min( VANTAGE_t ) ), ...
                             Truth_Fit.f_z( min( VANTAGE_t ) ) ];
            
            % angle between VANTAGE_vector and Truth_vector
            theta = acos( dot( VANTAGE_vector, Truth_vector ) / ...
                ( norm( VANTAGE_vector ) * norm( Truth_vector ) ) );
            
            % initialization of the rotation vector in case theta = 0
            n_vec = [ 0, 0, 0 ];
            
            if should_debug
                
                figure;
                scatter3( VANTAGE_Data.x, VANTAGE_Data.y, VANTAGE_Data.z, 2, ...
                    VANTAGE_Data.t )
                hold on
                plot3( VANTAGE_Fit.f_x( VANTAGE_Data.t ), ...
                    VANTAGE_Fit.f_y( VANTAGE_Data.t ), ...
                    VANTAGE_Fit.f_z( VANTAGE_Data.t ), ...
                    '-r', 'LineWidth', 2 )
                scatter3( Truth_Data.x, Truth_Data.y, Truth_Data.z, 2, ...
                    Truth_Data.t )
                plot3( Truth_Fit.f_x( Truth_Data.t-dt ), ...
                    Truth_Fit.f_y( Truth_Data.t-dt ), ...
                    Truth_Fit.f_z( Truth_Data.t-dt ), ...
                    '-r', 'LineWidth', 2 )
                
            end
            
            % check to make sure theta is nonzero
            if theta
                
                % normal unit vector to VANTAGE_vector and Truth_vector
                n_vec = cross( Truth_vector, VANTAGE_vector );
                n_vec = n_vec ./ norm( n_vec );
                
                %{
                according to Wikipedia
                (https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle)
                the rotation matrix for a rotation of theta about n_vec is:
                %}
                R = [ cos(theta) + n_vec(1)^2 * ( 1 - cos(theta) ), ...
                      n_vec(1) * n_vec(2) * ( 1 - cos(theta) ) - n_vec(3) * sin(theta), ...
                      n_vec(1) * n_vec(3) * ( 1 - cos(theta) ) + n_vec(2) * sin(theta); ...
                      n_vec(2) * n_vec(1) * ( 1 - cos(theta) ) + n_vec(3) * sin(theta), ...
                      cos(theta) + n_vec(2)^2 * ( 1 - cos(theta) ), ...
                      n_vec(2) * n_vec(3) * ( 1 - cos(theta) ) - n_vec(1) * sin(theta); ...
                      n_vec(3) * n_vec(1) * ( 1 - cos(theta) ) - n_vec(2) * sin(theta), ...
                      n_vec(3) * n_vec(2) * ( 1 - cos(theta) ) + n_vec(1) * sin(theta), ...
                      cos(theta) + n_vec(3)^2 * ( 1 - cos(theta) ) ];
                
                % rotate Truth_Data.[x,y,z]
                for i = 1 : length( Truth_Data.t )
                    
                    vec = [ Truth_Data.x(i); Truth_Data.y(i); Truth_Data.z(i) ];
                    
                    intermediate = R * vec;
                    
                    Truth_Data.x(i) = intermediate(1);
                    Truth_Data.y(i) = intermediate(2);
                    Truth_Data.z(i) = intermediate(3);
                    
                end
                
                % rotate the best fit line
                b_vec = [ Truth_Fit.f_x( 0 ), Truth_Fit.f_y( 0 ), Truth_Fit.f_z( 0 ) ];
                Truth_vector = R * Truth_vector';
                b_vec = R * b_vec';
                Truth_Fit.f_x = @(t) Truth_vector(1) .* ( t - dt ) + b_vec(1);
                Truth_Fit.f_y = @(t) Truth_vector(2) .* ( t - dt ) + b_vec(2);
                Truth_Fit.f_z = @(t) Truth_vector(3) .* ( t - dt ) + b_vec(3);
                
            end
            
            if should_debug
                
                scatter3( Truth_Data.x, Truth_Data.y, Truth_Data.z, 2, ...
                    Truth_Data.t )
                plot3( Truth_Fit.f_x( Truth_Data.t ), ...
                    Truth_Fit.f_y( Truth_Data.t ), ...
                    Truth_Fit.f_z( Truth_Data.t ), ...
                    '-b', 'LineWidth', 2 )
                colormap jet
                title( 'Timestamp Corrected Truth Data Rotation: Before & After' )
                
            end
            
            %{
            Finally, we are going to change the offset in the truth data such that the
            areas of data we trust overlap in 3D space by making the mean point during
            that span be identical for VANTAGE data and truth data
            Note: VANTAGE_t from earlier is a time vector corresponding to this domain
            %}
            Truth_Offset_x = mean( VANTAGE_Fit.f_x( VANTAGE_t ) ) - ...
                mean( Truth_Fit.f_x( VANTAGE_t ) );
            Truth_Offset_y = mean( VANTAGE_Fit.f_y( VANTAGE_t ) ) - ...
                mean( Truth_Fit.f_y( VANTAGE_t ) );
            Truth_Offset_z = mean( VANTAGE_Fit.f_z( VANTAGE_t ) ) - ...
                mean( Truth_Fit.f_z( VANTAGE_t ) );
            offset_vec = [ Truth_Offset_x, Truth_Offset_y, Truth_Offset_z ];
            
            if should_debug
                
                figure;
                scatter3( VANTAGE_Data.x, VANTAGE_Data.y, VANTAGE_Data.z, 2, ...
                    VANTAGE_Data.t )
                hold on
                plot3( VANTAGE_Fit.f_x( VANTAGE_Data.t ), ...
                    VANTAGE_Fit.f_y( VANTAGE_Data.t ), ...
                    VANTAGE_Fit.f_z( VANTAGE_Data.t ), ...
                    '-r', 'LineWidth', 2 )
                scatter3( Truth_Data.x, Truth_Data.y, Truth_Data.z, 2, ...
                    Truth_Data.t )
                plot3( Truth_Fit.f_x( Truth_Data.t ), ...
                    Truth_Fit.f_y( Truth_Data.t ), ...
                    Truth_Fit.f_z( Truth_Data.t ), ...
                    '-r', 'LineWidth', 2 )
                
            end
            
            % Applying offset
            Truth_Data.x = Truth_Data.x + Truth_Offset_x;
            Truth_Data.y = Truth_Data.y + Truth_Offset_y;
            Truth_Data.z = Truth_Data.z + Truth_Offset_z;
            Truth_Fit.f_x = @(t) Truth_Fit.f_x(t) + Truth_Offset_x;
            Truth_Fit.f_y = @(t) Truth_Fit.f_y(t) + Truth_Offset_y;
            Truth_Fit.f_z = @(t) Truth_Fit.f_z(t) + Truth_Offset_z;
            
            if should_debug
                
                scatter3( Truth_Data.x, Truth_Data.y, Truth_Data.z, 2, ...
                    Truth_Data.t )
                plot3( Truth_Fit.f_x( Truth_Data.t ), ...
                    Truth_Fit.f_y( Truth_Data.t ), ...
                    Truth_Fit.f_z( Truth_Data.t ), ...
                    '-b', 'LineWidth', 2 )
                colormap jet
                title( 'Timestamp Corrected, Rotated Truth Data Translation: Before & After' )
                
                figure;
                plot( VANTAGE_t, VANTAGE_Fit.f_x( VANTAGE_t ), '-k', 'LineWidth', 2 )
                hold on
                plot( VANTAGE_t, Truth_Fit.f_x( VANTAGE_t ), '-r', 'LineWidth', 2 )
                title( 'X VANTAGE Measurement Best Fit VS Truth Best Fit' )
                
                figure;
                plot( VANTAGE_t, VANTAGE_Fit.f_y( VANTAGE_t ), '-k', 'LineWidth', 2 )
                hold on
                plot( VANTAGE_t, Truth_Fit.f_y( VANTAGE_t ), '-r', 'LineWidth', 2 )
                title( 'Y VANTAGE Measurement Best Fit VS Truth Best Fit' )
                
                figure;
                plot( VANTAGE_t, VANTAGE_Fit.f_z( VANTAGE_t ), '-k', 'LineWidth', 2 )
                hold on
                plot( VANTAGE_t, Truth_Fit.f_z( VANTAGE_t ), '-r', 'LineWidth', 2 )
                title( 'Z VANTAGE Measurement Best Fit VS Truth Best Fit' )
                
            end
            
        end
        
        %% Correlate the truth data using a weighted sum of all CubeSats
        %
        % Preforms timestamp corrections, data rotation, and data
        % translation to the truth data input such that it matches the TOF
        % data as closely as possible for the weighted sum of all cubesat's
        % corrections
        % 
        % @param   MeasuredDataArray   a crazy struct which has position
        %                              and time measurements burried
        %                              somewhere in it
        %
        % @param   TruthData   this is a crazy struct with more in it than
        %                      I comprehend
        %
        % @return   updatedTruth   another crazy struct formatted
        %                          identically to TruthData with the
        %                          timestamp corrected, rotated, translated
        %                          truth data within it
        %
        % @return   dt   The time shifting scalar applied to the truth data
        %                timestamps
        %
        % @return   n_vec   The vector about which the truth data is
        %                   rotated
        %
        % @return   theta   The angle [rad] of the rotation around n_vec
        %
        % @return   offset_vec   The translation applied to the truth data
        %
        % @author Marshall Herr
        % @date   15-Apr-2019
        function [updatedTruth,dt,n_vec,theta,offset_vec] = PerformTruthDataCorrelationMultiple(obj,MeasuredDataArray,TruthData)
            %{
            This time we don't have a single most trusted cubesat and must
            average across however many there are
            %}
            n_sats = length( TruthData.Cubesat );
            
            [dt,theta] = deal(zeros(n_sats,1));
            % n_sats rows of 3 column row vectors
            [n_vec,offset_vec] = deal(zeros(n_sats,3));
            
            for k = 1 : n_sats
                
                %{
                First thing is first, I am going to unpackage TruthData into
                the variables I used when I originally wrote this code. They
                are:

                - VANTAGE_Data: a struct with the following fields as they
                                pertain to the VANTAGE measurements
                    > t: Time column vector
                    > x: X position column vector
                    > y: Y position column vector
                    > z: Z position column vector
                    > d: A length 2 vector, first element: minimum distance
                         on which ToF is trusted, second element: maximum
                         distance on which ToF is trusted
                - Truth_Data: a struct with the following fields as they pertain
                              to the Truth measurements
                    > t: Time column vector
                    > x: X position column vector
                    > y: Y position column vector
                    > z: Z position column vector
                %}
                CubeSatofInterest = MeasuredDataArray(1,k);
                N = length( CubeSatofInterest.time );
                [ VANTAGE_Data.t, VANTAGE_Data.x, VANTAGE_Data.y, ...
                    VANTAGE_Data.z ] = deal( NaN .* ones( N, 1 ) );
                for i = 1 : N
                    VANTAGE_Data.t(i) = CubeSatofInterest.time(i);
                    VANTAGE_Data.x(i) = CubeSatofInterest.centroids_VCF{i}(1);
                    VANTAGE_Data.y(i) = CubeSatofInterest.centroids_VCF{i}(2);
                    VANTAGE_Data.z(i) = CubeSatofInterest.centroids_VCF{i}(3);
                end
                % thanks josh!
                VANTAGE_Data.d = CubeSatofInterest.TOFrange;
                
                CubeSatofInterest = TruthData.Cubesat(k);
                N = length( TruthData.t );
                [ Truth_Data.t, Truth_Data.x, Truth_Data.y, Truth_Data.z ] =...
                    deal( NaN .* ones( N, 1 ) );
                for i = 1 : N
                    Truth_Data.t(i) = TruthData.t(i);
                    Truth_Data.x(i) = CubeSatofInterest.pos(i,1);
                    Truth_Data.y(i) = CubeSatofInterest.pos(i,2);
                    Truth_Data.z(i) = CubeSatofInterest.pos(i,3);
                end
                
                %{
                Now that that's done, we can blindly paste my algorith below!
                %}
                [dt(k),n_vec(k,:),theta(k),offset_vec(k,:)] = obj.CorrelationMachine(VANTAGE_Data,Truth_Data);
                
            end
            
            % time to average!
            dt = mean(dt);
            theta = mean(theta);
            n_vec = [ mean(n_vec(:,1)), mean(n_vec(:,2)), mean(n_vec(:,3)) ];
            offset_vec = [ mean(offset_vec(:,1)), mean(offset_vec(:,2)), mean(offset_vec(:,3)) ];
            
            %{
            Alright! Algorithm over, time to repackage TruthData and return
            it as updatedTruth
            I apply a time shift of +dt to TruthData.Cubesat(:).t
            TruthData.t is a row vector
            I apply a rotation of theta around n_vec to TruthData.Cubesat(:).pos
            I apply a translation of offset_vec to TruthData.Cubesat(:).pos
            TruthData.Cubesat(:).pos is [x,y,z] where x, y, & z are column vectors
            %}
            updatedTruth = TruthData; % best to start here I guess...
            updatedTruth.t = updatedTruth.t + dt;
            
            % theft of a rotation matrix of theta radians about n_vec
            R = [ cos(theta) + n_vec(1)^2 * ( 1 - cos(theta) ), ...
                  n_vec(1) * n_vec(2) * ( 1 - cos(theta) ) - n_vec(3) * sin(theta), ...
                  n_vec(1) * n_vec(3) * ( 1 - cos(theta) ) + n_vec(2) * sin(theta); ...
                  n_vec(2) * n_vec(1) * ( 1 - cos(theta) ) + n_vec(3) * sin(theta), ...
                  cos(theta) + n_vec(2)^2 * ( 1 - cos(theta) ), ...
                  n_vec(2) * n_vec(3) * ( 1 - cos(theta) ) - n_vec(1) * sin(theta); ...
                  n_vec(3) * n_vec(1) * ( 1 - cos(theta) ) - n_vec(2) * sin(theta), ...
                  n_vec(3) * n_vec(2) * ( 1 - cos(theta) ) + n_vec(1) * sin(theta), ...
                  cos(theta) + n_vec(3)^2 * ( 1 - cos(theta) ) ];
            
            N = length( updatedTruth.t );
            M = length( updatedTruth.Cubesat );
            % for each time
            for i = 1 : N
                % for each cubesat
                for j = 1 : M
                    % extract the ith position vector (transpose to turn it
                    % into a column vector)
                    pos_vec = updatedTruth.Cubesat(j).pos(i,:)';
                    % rotate it theta [rad] about n_vec
                    pos_vec = R * pos_vec;
                    % translate it by +offset_vec (transpose to turn it
                    % back into a row vector)
                    pos_vec = pos_vec' + offset_vec;
                    % update updatedTruth.Cubesat(j).pos(i,:);
                    updatedTruth.Cubesat(j).pos(i,:) = pos_vec;
                end
            end
            % done!
        end
        
        %% Write out new corrected truth data json text file and json corrections file
        %
        % Creates/overwrites files in the directory where the original
        % truth file was from in which will lie the corrected truth data in
        % one and the correction terms in the other
        % 
        % @param   updatedTruth   crazy truth structure to be friday the
        %                         13thed
        %
        % @param   TruthFileName   the truth data file relative path
        %
        % @param   dt   the timestamp correction
        %
        % @param   n_vec   the rotation correction vector
        %
        % @param   theta   the rotation correction angle
        %
        % @param   offset_vec   the translation correction vector
        %
        % @author Marshall Herr
        % @date   15-Apr-2019
        function [] = WriteUpdatedTruthData(~,updatedTruth,TruthFileName,dt,n_vec,theta,offset_vec)
            % gets the absolute path to the file
            fullPath = fullfile(pwd, TruthFileName);
            % cuts it up
            [filePath,fileName,fileExtension] = fileparts(fullPath);
            newTruthFileFullPath = [ filePath, fileName, '_corrected', fileExtension ];
            jason = jsonencode(updatedTruth);
            fID = fopen( newTruthFileFullPath, 'w' );
            fprintf( fID, jason );
            fclose(fID);
            newTruthCorrectionsFullPath = [ filePath, fileName, '_corrections', fileExtension ];
            jason = jsonencode({dt,n_vec,theta,offset_vec});
            fID = fopen( newTruthCorrectionsFullPath, 'w' );
            fprintf( fID, jason );
            fclose(fID);
        end
    end
end