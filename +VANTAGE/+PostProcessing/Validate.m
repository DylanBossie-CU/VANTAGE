classdef Validate
    %% Properties
    properties(Access = public)
        % Boolean for whether or not to auto update truth data
        CorrelateTruthData
        
        ComputingResults
        
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
        function obj = Validate(configFilename,ModelRef,CorrelateTruthData)
            configFilename = [configFilename '/Validate.json'];
            parameters = jsondecode(fileread(configFilename));
            %obj.CorrelateTruthData = parameters.CorrelateTruthData;
            obj.ComputingResults = parameters.ComputingResults;
            obj.ModelRef = ModelRef;
            
            % If user chooses to update truth data with corrections
            if CorrelateTruthData
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
            testType = 'Modular';
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
        
        %% Compute truth distance vector
        %
        % 
        % @param    
        %
        % @return 
        %
        % @author Dylan Bossie 
        % @date   11-Apr-2019
        function [distance] = ComputeTruthDistance(~, Model)
            positions = Model.Truth_VCF.Cubesat;
            distance = cell(numel(positions),1);
            for i = 1:numel(positions)
                cubesat = positions(i).pos;
                distance_cs = zeros(length(cubesat),1);
                for j = 1:length(cubesat)
                    distance_cs(j) = sqrt(cubesat(j,1).^2+cubesat(j,2).^2+cubesat(j,3).^2);
                end
                distance{i} = distance_cs;
            end
        end
        
        %% Compute time of cs launch
        %
        % 
        % @param    
        %
        % @return 
        %
        % @author Dylan Bossie 
        % @date   11-Apr-2019
        function [launch_time_diff] = ComputeLaunchTime(~, CubeSat,Time,Truth)
            CubeSat = CubeSat.CubeSatFitted;
            % Find first valid data point for CubeSat 1
            CS1 = CubeSat{1};
            for i = 1:length(CS1)
                if ~isnan(CS1(i,3))
                    break
                end
            end
            firstIndex = i;
            % Find measured launch time
            launch_time = interp1(Time.t_fit,CS1(:,3),0,'linear','extrap');
            
            % Find actual launch time
            CS1_Truth_Z = Truth.Cubesat(1).pos(:,3);
            true_launch_time = interp1(Truth.t,CS1_Truth_Z,0,'linear','extrap');
            
            launch_time_diff = abs(true_launch_time-launch_time);
        end
        
        %% Interpolate cubesats to truth dat apositions
        %
        % 
        % @param    
        %
        % @return 
        
        % @author Dylan Bossie 
        % @date   11-Apr-2019
        function [pos_err] = InterpolateDistance(~, CSArray,timefitted,time,distance)
            CubeSats = CSArray.CubeSatFitted;
            CubeSatTime = timefitted.t_fit;
            pos_err = cell(numel(CubeSats),1);
            for i = 1:numel(CubeSats)
                CubeSat = CubeSats{i};
                distancecs = distance{i};
                X = CubeSat(:,1);
                Y = CubeSat(:,2);
                Z = CubeSat(:,3);
                X_interp = interp1(CubeSatTime,X',time);
                Y_interp = interp1(CubeSatTime,Y',time);
                Z_interp = interp1(CubeSatTime,Z',time);
                
                absdistance = sqrt(X_interp.^2+Y_interp.^2+Z_interp.^2);
                
                pos_err{i} = abs(absdistance-distancecs');
                pos_err{i} = pos_err{i}';
            end
        end
        
        %% Compute mean velocity
        %
        % Compute mean velocity of all cubesats in current file
        % 
        % @param    
        %
        % @return 
        %
        % @author Dylan Bossie 
        % @date   11-Apr-2019
        function [meanvelocity] = ComputeMeanVelocity(obj,CubeSats,time,type)
            TestType = obj.ModelRef.Deployer.testScenario;
            if strcmpi(type,'cs')
                Time = time.t_fit;
                CubeSats = CubeSats.CubeSatFitted;
            elseif strcmpi(type,'truth')
                Time = time;
                tmp = cell(numel(CubeSats),1);
                for i = 1:numel(CubeSats)
                    tmp{i} = CubeSats(i).pos;
                end
                CubeSats = tmp;
            end
            finalIndex = length(Time);
            if strcmpi(TestType,'Simulation')
                CS1_Z = CubeSats{1}(:,3);
                for i = 1:length(CS1_Z)
                    if CS1_Z(i) >= 100
                        break
                    end
                end
                finalIndex = i;
            end
            
            V_X = zeros(finalIndex-1,1);
            V_Y = zeros(finalIndex-1,1);
            V_Z = zeros(finalIndex-1,1);
            
            
            
            V_Xmean = [];
            V_Ymean = [];
            V_Zmean = [];
            for i = 1:numel(CubeSats)
                CubeSat = CubeSats{i};
                X = CubeSat(1:finalIndex,1);
                Y = CubeSat(1:finalIndex,2);
                Z = CubeSat(1:finalIndex,3);
                for j = 1:finalIndex-1
                    V_X(j) = (X(j+1)-X(j))/(Time(j+1)-Time(j)) ;
                    V_Y(j) = (Y(j+1)-Y(j))/(Time(j+1)-Time(j)) ;
                    V_Z(j) = (Z(j+1)-Z(j))/(Time(j+1)-Time(j)) ;
                end
                V_Xmean = [V_Xmean mean(V_X)];
                V_Ymean = [V_Ymean mean(V_Y)];
                V_Zmean = [V_Zmean mean(V_Z)];
            end
            meanvelocity = [mean(V_Xmean) mean(V_Ymean) mean(V_Zmean)];
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
        function [] = ErrorAnalysis(obj,Model,SensorData,testDef)
                if strcmpi(testDef,'Modular')
                    resultsFolder = 'Data/Results/matFiles/ModularTest_4_9/';
                    matResults = [resultsFolder 'data/'];
                    AbsoluteErrorFiles = dir([matResults 'AbsError*']);
                    CubeSatDataFiles = dir([matResults 'CSData*']);
                    TruthDataFiles = dir([matResults 'TruthData*']);
                    XErrorFiles = dir([matResults 'XError*']);
                    YErrorFiles = dir([matResults 'YError*']);
                    ZErrorFiles = dir([matResults 'ZError*']);
                    TimeFiles = dir([matResults 'CSTime*']);

                    interpolationPoints = linspace(0,10,1000);

                elseif strcmpi(testDef,'100m')
                    resultsFolder = 'Data/Results/matFiles/100m/';
                    matResults = [resultsFolder 'data/'];
                    AbsoluteErrorFiles = dir([matResults 'AbsError*']);
                    CubeSatDataFiles = dir([matResults 'CSData*']);
                    TruthDataFiles = dir([matResults 'TruthData*']);
                    XErrorFiles = dir([matResults 'XError*']);
                    YErrorFiles = dir([matResults 'YError*']);
                    ZErrorFiles = dir([matResults 'ZError*']);
                    TimeFiles = dir([matResults 'CSTime*']);

                    interpolationPoints = linspace(0,100,1000);
                elseif strcmpi(testDef,'Sim085')
                    resultsFolder = 'Data/Results/matFiles/Simulation_4_15_085/';
                    matResults = [resultsFolder 'data/'];
                    AbsoluteErrorFiles = dir([matResults 'AbsError*']);
                    CubeSatDataFiles = dir([matResults 'CSData*']);
                    TruthDataFiles = dir([matResults 'TruthData*']);
                    XErrorFiles = dir([matResults 'XError*']);
                    YErrorFiles = dir([matResults 'YError*']);
                    ZErrorFiles = dir([matResults 'ZError*']);
                    TimeFiles = dir([matResults 'CSTime*']);

                    interpolationPoints = linspace(0,100,1000);
                    
                    elseif strcmpi(testDef,'Sim030')
                    resultsFolder = 'Data/Results/matFiles/Simulation_4_15_030/';
                    matResults = [resultsFolder 'data/'];
                    AbsoluteErrorFiles = dir([matResults 'AbsError*']);
                    CubeSatDataFiles = dir([matResults 'CSData*']);
                    TruthDataFiles = dir([matResults 'TruthData*']);
                    XErrorFiles = dir([matResults 'XError*']);
                    YErrorFiles = dir([matResults 'YError*']);
                    ZErrorFiles = dir([matResults 'ZError*']);
                    TimeFiles = dir([matResults 'CSTime*']);

                    interpolationPoints = linspace(0,100,1000);
                    
                    elseif strcmpi(testDef,'Sim140')
                    resultsFolder = 'Data/Results/matFiles/Simulation_4_15_140/';
                    matResults = [resultsFolder 'data/'];
                    AbsoluteErrorFiles = dir([matResults 'AbsError*']);
                    CubeSatDataFiles = dir([matResults 'CSData*']);
                    TruthDataFiles = dir([matResults 'TruthData*']);
                    XErrorFiles = dir([matResults 'XError*']);
                    YErrorFiles = dir([matResults 'YError*']);
                    ZErrorFiles = dir([matResults 'ZError*']);
                    TimeFiles = dir([matResults 'CSTime*']);

                    interpolationPoints = linspace(0,100,1000);
                    
                    elseif strcmpi(testDef,'Sim195')
                    resultsFolder = 'Data/Results/matFiles/Simulation_4_15_195/';
                    matResults = [resultsFolder 'data/'];
                    AbsoluteErrorFiles = dir([matResults 'AbsError*']);
                    CubeSatDataFiles = dir([matResults 'CSData*']);
                    TruthDataFiles = dir([matResults 'TruthData*']);
                    XErrorFiles = dir([matResults 'XError*']);
                    YErrorFiles = dir([matResults 'YError*']);
                    ZErrorFiles = dir([matResults 'ZError*']);
                    TimeFiles = dir([matResults 'CSTime*']);

                    interpolationPoints = linspace(0,100,1000);
                else
                    error('not a valid test case')
                end

                for i = 1:numel(AbsoluteErrorFiles)
                    absError = load([AbsoluteErrorFiles(i).folder '/' AbsoluteErrorFiles(i).name]);
                    CS = load([CubeSatDataFiles(i).folder '/' CubeSatDataFiles(i).name]);
                    Truth = load([TruthDataFiles(i).folder '/' TruthDataFiles(i).name]);
                    XError = load([XErrorFiles(i).folder '/' XErrorFiles(i).name]);
                    YError = load([YErrorFiles(i).folder '/' YErrorFiles(i).name]);
                    ZError = load([ZErrorFiles(i).folder '/' ZErrorFiles(i).name]);
                    Time = load([TimeFiles(i).folder '/' TimeFiles(i).name]);

                    % Defining properties to provide for output
                    meanvelocity = obj.ComputeMeanVelocity(CS,Time,'cs');
                    velocity = obj.ComputeMeanVelocity(Model.Truth_VCF.Cubesat,Model.Truth_VCF.t,'truth');

                    v_err = abs(meanvelocity-velocity');

                    distance = obj.ComputeTruthDistance(Model);

                    % Interpolate cubesats to the truth distances for
                    % pos_err
                    pos_err = obj.InterpolateDistance(CS,Time,Model.Truth_VCF.t,distance);

                    t_err = obj.ComputeLaunchTime(CS,Time,Model.Truth_VCF);

                    if t_err > 5
                        error('Time manager generated incorrect time offset');
                    end
                    
                    
                    % Output final results for this test
                    dataStruct = struct();
                    dataStruct.distance = distance;
                    dataStruct.velocity = velocity;
                    dataStruct.t_err = t_err;
                    dataStruct.v_err = v_err;
                    dataStruct.pos_err = pos_err;

                    % Save fitted results for error analysis later
                    dataFolder = resultsFolder;
                    if strcmpi(Model.Deployer.testScenario,'Modular')
                        dataStruct.velocity = dataStruct.velocity';
                        %dataFolder = [dataFolder 'Modular/'];
                        folderString = Model.Deployer.TruthFileName;
                        tmp = split(folderString,'/');
                        testNumber = tmp{3};
                    elseif strcmpi(Model.Deployer.testScenario,'100m')
                        %dataFolder = [dataFolder '100m/'];
                        dataStruct.velocity = dataStruct.velocity';
                        folderString = Model.Deployer.TruthFileName;
                        tmp = split(folderString,'/');
                        testNumber = tmp{3};
                    elseif strcmpi(Model.Deployer.testScenario,'Simulation')
                        %dataFolder = [dataFolder 'Simulation/'];
                        tmp = split(SensorData.TOFData,'/');
                        testNumber = tmp{5};
                    else
                        error('invalid test type in Deployer.TruthFileName')
                    end

                    mkdir(dataFolder)
                    save([pwd '/' dataFolder testNumber '_' testDef 'dataStruct.mat'],'dataStruct');

    %                 CubeSats = CS.CubeSatFitted;

                    % Interpolate error for each CubeSat across the desired
                    % range for the given test
    %                 interpError = zeros(numel(CubeSats),length(interpolationPoints));
    %                 for j = 1:numel(CubeSats)
    %                     CubeSat = CubeSats{j};
    %                     CSAbsError = pos_err{j};
    %                     Z_points = CubeSat(:,3);
    %                     interpError(j,:) = interp1(Z_points,CSAbsError,interpolationPoints);
    %                 end
    %                 
    %                 % Compute the mean error
    %                 MeanError = zeros(length(interpolationPoints),1);
    %                 for j = 1:length(interpolationPoints)
    %                     MeanError(j) = mean(interpError(:,j));
    %                 end
    %                 MeanErrorAllFiles(i,:) = MeanError;
                end
            
%             % Process mean error across all test cases
%             TotalMeanError = zeros(numel(interpolationPoints),1);
%             for i = 1:length(interpolationPoints)
%                 TotalMeanError(i) = mean(MeanErrorAllFiles(:,i));
%             end
% 
%             % Output final .mat file
%             if strcmpi(Model.Deployer.testScenario,'Modular')
%                 dataFolder = 'Data/ModularTest_4_9/Results';
%                 folderString = Model.Deployer.TruthFileName;
%                 save([pwd '/' dataFolder '/FINALERROR_Modular.mat'],'TotalMeanError');
%             elseif strcmpi(Model.Deployer.testScenario,'100m')
%                 dataFolder = 'Data/3_25_100m/Results';
%                 folderString = Model.Deployer.TruthFileName;
%                 save([pwd '/' dataFolder '/FINALERROR_100m.mat'],'TotalMeanError');
%             elseif strcmpi(Model.Deployer.testScenario,'Simulation')
%                 testNumber = 'notimplemented';
%             else
%                 error('invalid test type in Deployer.TruthFileName')
%             end
            %{

%}
            
        end
        
        function [p1,p2] = generateErrorPlot(~,d,mu,sigma,color)
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

%             % Testing
%             d = linspace(0,100,100);
%             mu = d.^2 ./ 100;
%             sigma = d.^(1.5) ./ 50 + 1;
            
            if nargin < 4
                
                color = 'flat';
                
            end

            n = 1000;
            s = 10;
            [ x, y1, y2, c1 ] = deal( zeros( n, length(d) ) );

            for i = 1 : length(d)

                idx = ( (i-1)*n + 1 ) : ( i*n );
                x(idx) = d(i) .* ones(1,n)';
                y1(idx) = mu(i)+linspace(0,s*sigma(i),n)';
                y2(idx) = mu(i)-linspace(0,s*sigma(i),n)';
                c1(idx) = exp( -linspace(0,s*sigma(i),n) ./ sigma(i) );

            end

            hold on
            p1 = surf( x, y1, 0.*x, 'FaceAlpha', 'flat', 'FaceColor', color, ...
                'AlphaDataMapping', 'scaled', 'AlphaData', c1, 'LineStyle', 'None' );
            p2 = surf( x, y2, 0.*x, 'FaceAlpha', 'flat', 'FaceColor', color, ...
                'AlphaDataMapping', 'scaled', 'AlphaData', c1, 'LineStyle', 'None' );

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
        function PlotResults(~,ModelRef,SensorData)
            
            TestType = ModelRef.Deployer.testScenario;
            
            matFilesFolder = 'Data/Results/matFiles/';
            switch TestType
                case '100m'
                    tmp = split(ModelRef.Deployer.TruthFileName,'/');
                    TestNum = tmp{3};
                    matFilesFolder = [matFilesFolder '100m/'];
                case 'Simulation'
                    tmp = split(SensorData.TOFData,'/');
                    TestNum = tmp{4};
                    matFilesFolder = [matFilesFolder 'Simulation_4_15/'];
                case 'Modular'
                    tmp = split(ModelRef.Deployer.TruthFileName,'/');
                    TestNum = tmp{3};
                    matFilesFolder = [matFilesFolder 'ModularTest_4_9/'];
            end
            
            AbsoluteErrorFiles = dir([matFilesFolder 'AbsErrorData*' num2str(TestNum) '.mat']);
            CubeSatDataFiles = dir([matFilesFolder 'CSData*' num2str(TestNum) '.mat']);
            TruthDataFiles = dir([matFilesFolder 'TruthData*' num2str(TestNum) '.mat']);
            XErrorFiles = dir([matFilesFolder 'XError*' num2str(TestNum) '.mat']);
            YErrorFiles = dir([matFilesFolder 'YError*' num2str(TestNum) '.mat']);
            ZErrorFiles = dir([matFilesFolder 'ZError*' num2str(TestNum) '.mat']);
            TimeFiles = dir([matFilesFolder 'CSTime*' num2str(TestNum) '.mat']);
            
            
            absError = load([AbsoluteErrorFiles.folder '/' AbsoluteErrorFiles.name]);
            CS = load([CubeSatDataFiles.folder '/' CubeSatDataFiles.name]);
            Truth = load([TruthDataFiles.folder '/' TruthDataFiles.name]);
            XError = load([XErrorFiles.folder '/' XErrorFiles.name]);
            YError = load([YErrorFiles.folder '/' YErrorFiles.name]);
            ZError = load([ZErrorFiles.folder '/' ZErrorFiles.name]);
            Time = load([TimeFiles.folder '/' TimeFiles.name]);
            
            cs1 = CS.CubeSatFitted{1};
            truth1 = Truth.TruthFitted{1};
            absError1 = absError.AbsoluteError{1};
            downrange1 = cs1(:,3);
            
            cs2 = CS.CubeSatFitted{2};
            truth2 = Truth.TruthFitted{2};
            absError2 = absError.AbsoluteError{2};
            downrange2 = cs2(:,3);
            
            cs3 = CS.CubeSatFitted{3};
            truth3 = Truth.TruthFitted{3};
            absError3 = absError.AbsoluteError{3};
            downrange3 = cs3(:,3);
            
            figure
            hold on
            plot(downrange1,absError1);
            plot(downrange2,absError2);
            plot(downrange3,absError3);
            
            xlabel('Range (m)');
            ylabel('Absolute Error (cm)')
            title([TestType ' Absolute Error - ' TestNum])
            
            figure
            hold on
            plot3(cs1(:,1),cs1(:,2),cs1(:,3));
            plot3(truth1(:,1),truth1(:,2),truth1(:,3));
            title([TestType ' 3D Position - ' TestNum])
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
                
                V_d = sqrt( VANTAGE_Data.x.^2 + VANTAGE_Data.y.^2 + VANTAGE_Data.z.^2 );
                VANTAGE_Data.d = [ min(V_d), max(V_d) ];
                
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
            
            % Define output path
            tmp = split(TruthFileName,'/');
            testNum = tmp{3};
            % cuts it up
            [filePath,fileName,fileExtension] = fileparts(fullPath);
            newTruthFileFullPath = [ filePath, '/', fileName, '_corrected', fileExtension ];
            jason = jsonencode(updatedTruth);
            fID = fopen( newTruthFileFullPath, 'w' );
            fprintf( fID, jason );
            fclose(fID);
            newTruthCorrectionsFullPath = [ filePath, '/', fileName, '_corrections', fileExtension ];
            jason = jsonencode({dt,n_vec,theta,offset_vec});
            fID = fopen( newTruthCorrectionsFullPath, 'w' );
            fprintf( fID, jason );
            fclose(fID);
        end
        
        %% Generates all of the plots
        %
        % This code shall produce four plots. 1) A contour plot which shows
        % the error of the VANTAGE system as a function of distance and
        % velocity 2) a plot of the velocity averaged errors as a function
        % of distance for each type of test as well as a "weighted average"
        % of the error using the errors of each of the three tests to
        % construct one continuous "expected performance" across the entire
        % distance domain; 3) a plot which shows the error in velocity as a
        % function of velocity; 4) a plot which shows the deployment error
        % as a function of velocity
        % 
        % @param   dataDirectory   the path to the directory containing
        %                          three folders: "Modular", "100m", and
        %                          "Simulation". in each folder will reside
        %                          all of the raw test .mat files which
        %                          contain structs with the following
        %                          fields: distance = the truth data
        %                          distance vector; velocity = the average
        %                          truth data velocity; t_err = the launch
        %                          time error; v_err = the error in the
        %                          measured average velocity as compared to
        %                          the truth average velocity; pos_err =
        %                          the magnitude of the error in position
        %                          at every point contained in the distance
        %                          field above
        %
        % @author Marshall Herr
        % @date   16-Apr-2019
        function [] = masterPlotter(obj,dataDirectory)
            %{
            dataDirectory:
                -> Modular
                    -> mat files
                -> 100m
                    -> mat files
                -> Simulation
                    -> mat files
            
            Steps:
            Find Simulation Folder
            Extract data from all tests
            sort tests by velocity
            Find Modular Folder
            Extract data from all tests
            sort tests by velocity
            Find 100m Folder
            Extract data from all tests
            sort tests by velocity
            Data formatting/analysis
            Generate plots
            %}
            fileType = '*.mat';
            
            % Simulation Data Extraction
            fileSearch = [ dataDirectory, '/Simulation/', fileType ];
            fileStruct = dir(fileSearch);
            numFilesS = length(fileStruct);
            [Simulation] = obj.extractMatFileData(fileStruct);
            
            % Modular Data Extraction
            fileSearch = [ dataDirectory, '/Modular/', fileType ];
            fileStruct = dir(fileSearch);
            numFilesM = length(fileStruct);
            [Modular] = obj.extractMatFileData(fileStruct);
            
            % 100m Data Extraction
            fileSearch = [ dataDirectory, '/100m/', fileType ];
            fileStruct = dir(fileSearch);
            numFiles100m = length(fileStruct);
            [m100] = obj.extractMatFileData(fileStruct);
            
            % convolve measurements
            [Simulation] = obj.convolveMeasurements(Simulation,numFilesS);
            [Modular] = obj.convolveMeasurements(Modular,numFilesM);
            [m100] = obj.convolveMeasurements(m100,numFiles100m);
            
            %%% Contour Plot of Error VS Distance & Velocity
            
            
            %%% Reduced Master Error VS Distance Plot
            req_distance = [ 0.1, 10, 10, 100 ];
            req_error = [ 0.1, 0.1, 1, 10 ];
            pointsOfInterest = [];
            
            % Fitted Curve to be representative of VANTAGE's performance
            % I will have to do this last, if at all
            fittedPositionDistance = req_distance;
            fittedPositionMeanError = req_error;
            fittedPositionSTDError = req_error;
            
            plotLabels = { 'Distance [m]', ...
                'Position Measurement Error [m]' };
            plotTitle = 'Position Measurement Error Versus Distance';
            distance_range = [1,100];
            
            [f_Master] = ...
                obj.errorPlot(req_distance,Simulation.distance_final,...
                Modular.distance_final,m100.distance_final,...
                fittedPositionDistance,req_error,...
                Simulation.mu_pos_err_final,Modular.mu_pos_err_final,...
                m100.mu_pos_err_final,fittedPositionMeanError,...
                Simulation.std_pos_err_final,Modular.std_pos_err_final,...
                m100.std_pos_err_final,fittedPositionSTDError,...
                pointsOfInterest,plotLabels,plotTitle,distance_range);
            
            %%% Velocity Error VS Velocity Plot
            req_distance = [ 0.1, 10, 10, 100 ];
            req_error = [ 0.1, 0.1, 1, 10 ];
            pointsOfInterest = [];
            
            % Fitted Curve to be representative of VANTAGE's performance
            % I will have to do this last, if at all
            fittedVelocityVelocity = req_distance;
            fittedVelocityMeanError = req_error;
            
            plotLabels = { 'Average Velocity [m/s]', ...
                'Average Velocity Measurement Error [m/s]' };
            plotTitle = 'Average Velocity Measurement Error Versus Average Velocity';
            velocity_range = [ -Inf, Inf ];
            
            [f_Velocity] = ...
                obj.errorPlot(req_distance,Simulation.mag_velocity,...
                Modular.mag_velocity,m100.mag_velocity,...
                fittedVelocityVelocity,req_error,...
                Simulation.mag_v_err,Modular.mag_v_err,...
                m100.mag_v_err,fittedVelocityMeanError,...
                {},{},{},{},pointsOfInterest,plotLabels,plotTitle,...
                velocity_range);
            
            %%% Launch Time Error VS Velocity Plot
            req_distance = [ 0.1, 10, 10, 100 ];
            req_error = [ 0.1, 0.1, 1, 10 ];
            pointsOfInterest = [];
            
            % Fitted Curve to be representative of VANTAGE's performance
            % I will have to do this last, if at all
            fittedTimeVelocity = req_distance;
            fittedTimeMeanError = req_error;
            
            plotLabels = { 'Average Velocity [m/s]', ...
                'Launch Time Measurement Error [m/s]' };
            plotTitle = 'Launch Time Measurement Error Versus Average Velocity';
            velocity_range = [ -Inf, Inf ];
            
            [f_Time] = ...
                obj.errorPlot(req_distance,Simulation.mag_velocity,...
                Modular.mag_velocity,m100.mag_velocity,...
                fittedTimeVelocity,req_error,...
                Simulation.t_err,Modular.t_err,...
                m100.t_err,fittedTimeMeanError,...
                {},{},{},{},pointsOfInterest,plotLabels,plotTitle,...
                velocity_range);
            
        end
        
        %% Convolves stuff
        %
        % This code shall produce things
        %
        % @author Marshall Herr
        % @date   16-Apr-2019
        function [structure] = convolveMeasurements(~,structure,numElements)
            
            % initialization
            [ structure.mean_distance, structure.mu_pos_err, ...
                structure.std_pos_err, structure.weights_pos_err ] = ...
                deal( cell( numElements, 1 ) );
            max_distance = Inf;
            min_distance = -Inf;
            num_Points = Inf;
            
            % averaging across cubesats during each test
            for i = 1 : numElements
                structure.mean_distance{i} = ...
                    mean([structure.distance{i}{:}],2);
                max_distance = min( [ max_distance, ...
                    max( structure.mean_distance{i} ) ] );
                min_distance = max( [ min_distance, ...
                    min( structure.mean_distance{i} ) ] );
                num_Points = min( num_Points, ...
                    length( structure.mean_distance{i} ) );
                
                structure.mu_pos_err{i} = ...
                    mean([structure.pos_err{i}{:}],2);
                structure.std_pos_err{i} = ...
                    std([structure.pos_err{i}{:}],0,2);
                structure.weights_pos_err{i} = ...
                    structure.std_pos_err{i}.^(-2);
                
            end
            
            % variance weighted averaging across tests
            structure.distance_final = linspace( min_distance, ...
                max_distance, num_Points )';
            structure.mu_pos_err_final = 0;
            structure.std_pos_err_final = 0;
            
            for i = 1 : numElements
                
                structure.mu_pos_err_final = ...
                    structure.mu_pos_err_final + ...
                    interp1( structure.mean_distance{i}, ...
                    structure.mu_pos_err{i}, structure.distance_final, ...
                    'pchip' ) .* interp1( structure.mean_distance{i}, ...
                    structure.weights_pos_err{i}, structure.distance_final, ...
                    'pchip' );
                
                structure.std_pos_err_final = ...
                    structure.std_pos_err_final + ...
                    interp1( structure.mean_distance{i}, ...
                    structure.weights_pos_err{i}, ...
                    structure.distance_final, 'pchip' );
                
            end
            
            structure.mu_pos_err_final = ...
                structure.mu_pos_err_final ./ ...
                structure.std_pos_err_final;
            
            structure.std_pos_err_final = ...
                structure.std_pos_err_final.^(-1/2);
            
        end
        
        %% Extracts all of the data from a folder of mat files
        %
        % This code shall produce a structure with the data contained in
        % the input folder
        % 
        % @param   fileStruct   a struct which is generated by a dir() call
        %                       and contains test data .mat files
        %
        % @return   outputStruct   a structure with the following fields:
        %                          distance = a cell array into which the
        %                          ith mat files distance cell goes
        %                          (cubesats are listed as farthest to
        %                          nearest), velocity = a cell into which
        %                          the ith mat files velocity goes, t_err =
        %                          a vector into which the ith mat files
        %                          t_err goes, v_err = a cell into which
        %                          the ith mat files v_err goes, pos_err =
        %                          a cell array into which the ith mat
        %                          files pos_err cell goes (cubesats are
        %                          listed as farthest to nearest)... this
        %                          struct is sorted into
        %                          outputStruct.velocity ascending order
        %
        % @author Marshall Herr
        % @date   16-Apr-2019
        function [outputStruct] = extractMatFileData(~,fileStruct)
            
            nFiles = length( fileStruct );
            
            % preallocation
            cm_to_m = 1 / 100; % m cm^-1
            intermediary = cell(nFiles,1);
            outputStruct.distance = intermediary;
            outputStruct.velocity = intermediary;
            outputStruct.v_err = intermediary;
            outputStruct.pos_err = intermediary;
            intermediary = NaN .* ones(nFiles,1);
            outputStruct.t_err = intermediary;
            outputStruct.mag_velocity = intermediary;
            outputStruct.mag_v_err = intermediary;
            
            for i = 1 : nFiles
                
                fileName = [ fileStruct(i).folder, '/', ...
                    fileStruct(i).name ];
                dataStruct = load( fileName );
                % lol
                try
                dataStruct = dataStruct.dataStruct;
                outputStruct.distance(i) = {dataStruct.distance(:)};
                outputStruct.velocity(i) = {dataStruct.velocity};
                outputStruct.mag_velocity(i) = norm(dataStruct.velocity);
                outputStruct.t_err(i) = dataStruct.t_err;
                outputStruct.v_err(i) = {dataStruct.v_err};
                outputStruct.mag_v_err(i) = norm(dataStruct.v_err);
                outputStruct.pos_err(i) = {dataStruct.pos_err(:)};
                catch
                    disp('fuckin hell yea break my shit')
                end
                
                % turns out the position error is given in cm not m
                for j = 1 : length(outputStruct.pos_err{i})
                    
                    outputStruct.pos_err{i}{j} = ...
                        outputStruct.pos_err{i}{j} .* cm_to_m;
                    
                end
                
            end
            
            % sort in ascending order by magnitude of velocity
            [ ~, sortedIndexes ] = ...
                sort( vecnorm( [ outputStruct.velocity{:} ] ) );
            outputStruct.distance = outputStruct.distance( sortedIndexes );
            outputStruct.velocity = outputStruct.velocity( sortedIndexes );
            outputStruct.t_err = outputStruct.t_err( sortedIndexes );
            outputStruct.v_err = outputStruct.v_err( sortedIndexes );
            outputStruct.pos_err = outputStruct.pos_err( sortedIndexes );
            
        end
        
        %% Generates all of the plots
        %
        % This code shall produce an analysis plot
        % 
        % @param   x_req   the independent variable of the plot vector
        %                  associated to the req points
        %
        % @param   x_S   the independent variable of the plot vector
        %                associated to the err_S points
        %
        % @param   x_M   the independent variable of the plot vector
        %                associated to the err_M points
        %
        % @param   x_100m   the independent variable of the plot vector
        %                   associated to the err_100m points
        %
        % @param   x_K   the independent variable of the plot vector
        %                  associated to the err_K points
        %
        % @param   err_S   the simulated test error vector
        %
        % @param   err_M   the modular test error vector
        %
        % @param   err_100m   the 100m test error vector
        %
        % @param   err_K   the knitted test error vector
        %
        % @param   req   an error requirements vector
        %
        % @param   x_c   important independent variable values
        %
        % @param   plotLabels   a cell containing {xlabel,ylabel}
        %
        % @param   plotTitle   a string containing the plot title
        %
        % @return   f   plot figure handle
        %
        % @author Marshall Herr
        % @date   16-Apr-2019
        function [f] = errorPlot(~,x_req,x_S,x_M,x_100m,x_K,req,...
                mu_err_S,mu_err_M,mu_err_100m,mu_err_K,sigma_err_S,...
                sigma_err_M,sigma_err_100m,sigma_err_K,x_c,plotLabels,...
                plotTitle,x_range)
            % Plotting parameters
            LINEWIDTH   = 2;
            FONTSIZE    = 24;
            MARKERSIZE  = 6;
            NumPlotsDefault = 9;
            %{
            colors:
            [ req;
                K;
                S;
                M;
             100m;
              x_c ]
            %}
            colors = [ 000, 000, 000; ...
                       162, 072, 145; ...
                       063, 064, 124; ...
                       111, 155, 117; ...
                       192, 174, 109; ...
                       091, 097, 103 ] ./ 255;
            legend_str = { 'Requirements', 'Variance Weighted Error', ...
                'Simulated Tests', 'Modular Tests', '100m Tests' };
            set(0, 'defaulttextInterpreter', 'latex')
            set(0, 'DefaultAxesLineStyleOrder', 'default')
            
            p = gobjects(NumPlotsDefault + length(x_c),1);
            
            %{
            plot order:
            everything but x_c, then x_c last
            %}
            f = figure();% 'Visible', 'off' );
            a = axes;
            p(1) = loglog( x_req, req, '--', 'color', colors(1,:), ...
                'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
            hold on
            p(2) = loglog( x_K, mu_err_K, '*-', 'color', colors(2,:), ...
                'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
            p(3) = loglog( x_S, mu_err_S, 'o-', 'color', colors(3,:), ...
                'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
            p(4) = loglog( x_M, mu_err_M, 's-', 'color', colors(4,:), ...
                'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
            p(5) = loglog( x_100m, mu_err_100m, 'd-', 'color', colors(5,:), ...
                'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
            index = 5;
            if ~isempty(sigma_err_K)
                p(index+1) = loglog( x_K, mu_err_K + sigma_err_K, '*-', 'color', colors(2,:), ...
                    'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
                index = index + 1;
            end
            if ~isempty(sigma_err_S)
                p(index+1) = loglog( x_S, mu_err_S + sigma_err_S, 'o-', 'color', colors(3,:), ...
                    'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
                index = index + 1;
            end
            if ~isempty(sigma_err_M)
                p(index+1) = loglog( x_M, mu_err_M + sigma_err_M, 's-', 'color', colors(4,:), ...
                    'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
                index = index + 1;
            end
            if ~isempty(sigma_err_100m)
                p(index+1) = loglog( x_100m, mu_err_100m + sigma_err_100m, 'd-', 'color', colors(5,:), ...
                    'LineWidth', LINEWIDTH, 'MarkerSize', MARKERSIZE );
                index = index + 1;
            end
            
            NumPlotsDefault = index;
            
            % extract x and y limits
            y_range = get(gca,'YLim');
            
            % plot x_c lines
            for i = 1 : length(x_c)
                
                p(NumPlotsDefault+i) = loglog( x_c(i).*[1,1], y_range, '--', ...
                    'color', colors(6,:), 'LineWidth', LINEWIDTH, ...
                    'MarkerSize', MARKERSIZE );
                
            end
            
            % in case one is missing above
            p = p(1:NumPlotsDefault+length(x_c));
            
            % reorder lines correctly
            a.Children = p(:);
            
            a.XLim = x_range;
            a.YLim = y_range;
            a.TickLabelInterpreter = 'latex';
            a.FontSize = FONTSIZE;
            a.XLabel.FontSize = FONTSIZE;
            a.YLabel.FontSize = FONTSIZE;
            a.XLabel.String = plotLabels{1};
            a.YLabel.String = plotLabels{2};
            a.Title.FontSize = FONTSIZE;
            a.Title.String = plotTitle;
            
            legend( p(1:NumPlotsDefault), legend_str, 'interpreter', 'latex', ...  
                'location', 'southeast', 'fontsize', FONTSIZE * 0.9)
            
            grid on
            grid minor
            f.Visible = 'on';
            
        end
    end
end