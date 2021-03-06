classdef Model < handle
    % Model is a class used to store the estimation methods that utilize both
    % TOF and camera data.

    properties (SetAccess = public)
        % Deployer class
        % @type Deployer
        Deployer
        
        % Truth data
        Truth_VCF
    end
    
    properties (SetAccess = protected)
        % Transform class
        % @type Transform
        Transform
        
        % Time Manager Class
        % @type TimeMan
        TimeMan
    end
    
    properties (SetAccess = private)
        % Optical camera class
        % @type Optical
        Optical

        % TOF camera class
        % @type TOF
        TOF
    end

    methods
        % Class Constructor:
        %
        % @param      manifestFilename  full filename of manifest JSON file
        % @param      configDirecName   The configuration directory name
        %
        % @return     A reference to an initialized Model object
        %
        function this = Model(manifestFilename,configDirecName,dataprocessing)
            
        	% Import child classes
        	import VANTAGE.PostProcessing.Deployer
        	import VANTAGE.PostProcessing.Transform
        	import VANTAGE.PostProcessing.Optical
        	import VANTAGE.PostProcessing.TOF
            import VANTAGE.PostProcessing.TimeManager

            if dataprocessing
                % Construct child classes and process truth data
                % NOTE: The order of these lines is very important
                this.Deployer = Deployer(manifestFilename, strcat(configDirecName,'/Deployer.json'),this);
                this.Transform = Transform(strcat(configDirecName,'/Transform.json'));
                SensorData = jsondecode(fileread(strcat(configDirecName,'/Sensors.json')));
                this.TimeMan = TimeManager(SensorData,this.Deployer.testScenario,this);
                this.Truth_VCF = this.processTruthData(this.Deployer.TruthFileName);
                this.TimeMan.syncTruthData(this);
                this.Optical = Optical(this,strcat(configDirecName,'/Optical.json'), this.Deployer.GetNumCubesats());
                this.TOF = TOF(this,strcat(configDirecName,'/TOF.json'));

                % Error catching
                if this.Deployer.numCubesats ~= this.Truth_VCF.numCubeSats
                    error('Truth data and manifest do not agree on the number of Cubesats')
                end
            else
                this.Deployer = Deployer(manifestFilename, strcat(configDirecName,'/Deployer.json'),this);
                SensorData = jsondecode(fileread(strcat(configDirecName,'/Sensors.json')));
                this.TimeMan = TimeManager(SensorData,this.Deployer.testScenario,this);
                this.Truth_VCF = this.processTruthData(this.Deployer.TruthFileName);
            end
        end

        % A method for looping through optical data and performing sensor fusion
        % to return a final state output
        %
        % @param      this   The object
        %
        %
        function [] = ComputeStateOutput(this)
        	% Get directory of optical frames
        	[didRead,direc,timestamps] = this.Optical.readInputFramesFromImages(this);

            [~,finalImageIndex] = max(timestamps);
            [~,firstImageIndex] = min(timestamps);
            
            [~,timestampIndices]= sort(timestamps);
            direc = direc(timestampIndices);
            
            if strcmpi(this.Deployer.testScenario,'Simulation')
                timestamps_VANTAGE = timestamps;
            else
                timestamps_VANTAGE = this.TimeMan.VantageTime(vertcat({direc(:).name})','Optical',this.Deployer.testScenario);
            end
            
            % Find time index right after TOF predicts the cubesats will be
            % at 10m
            t_start = fsolve(@(t) norm(this.Deployer.CubesatArray(1).evalTofFit(t)) - this.Optical.rangeStart, this.Optical.rangeStart);
            I_start = find(timestamps_VANTAGE>t_start,1,'first');
            t_10m = fsolve(@(t) norm(this.Deployer.CubesatArray(1).evalTofFit(t)) - 10, 10 );
            I_10m = find(timestamps_VANTAGE>=t_10m,1,'first');
            if isempty(I_10m)
                I_10m = numel(direc);
            end
            I_stop = numel(direc);
            if strcmpi(this.Deployer.testScenario,'Modular')
                I_stop = I_10m-1;
            end
            
            
            % Find background pixels in first optical image for background
            % subtraction
            [BackgroundPixels,firstFrame] = this.Optical.FindBackground(direc(firstImageIndex));
            
            % Find pixel location of CubeSats in last image for object
            % association in optical data
            this.Optical = this.Optical.findLastImagePixel(direc(finalImageIndex),BackgroundPixels,firstFrame);
            this.Optical.Timestamps = timestamps;
            
        	if didRead
                if ~isempty(I_start) && ~isempty(I_stop)
    	        	% Loop though optical frames
                    n = numel(direc) - I_start;
                    pos = cell(n,this.Deployer.GetNumCubesats());
                    posCount = 1;
                    for i = I_start:I_stop
                        if posCount==202
                        end
    	        		% Read frame
    	        		this.Optical.Frame = direc(i);
                        
                        % Get current time in VANTAGE global time [s]
                        currentTime = timestamps_VANTAGE(i);

    	        		% Run optical processing
    	        		[this.Optical,~, ~, isSystemCentroid] =...
                            this.Optical.OpticalProcessing(this.Optical.Frame,BackgroundPixels,firstFrame);

                        CamUnitVecsVCF = cell(numel(this.Optical.CubeSats),1);
                        for j = 1:length(CamUnitVecsVCF)
                            CamUnitVecsVCF{j} = this.Optical.CubeSats{j}.unitvec;
                        end

                        if strcmpi(isSystemCentroid,'invalid')
                            continue
                        end
                        
                        % Get propogated TOF positions
                        CubeSats = this.Deployer.CubesatArray;
                        pos_TOF = cell(numel(CubeSats),1);
                        
                        % pos_TOF: cell array for propagated TOF
                        % CubeSat positions, (:,1) = CS1, (:,2) = CS2,...
                        for j = 1:numel(CubeSats)
                            pos_TOF{j} = CubeSats(j).evalTofFit(currentTime);
                        end

    	        		% Run sensor fusion
    	        		pos(posCount,:) = RunSensorFusion(this, isSystemCentroid, this.Deployer.GetCamOriginVCF(), CamUnitVecsVCF, pos_TOF)';
                        t(posCount) = currentTime;
                        posCount = posCount + 1;
                        %plot3(pos{i,1}(1),pos{i,1}(2),pos{i,1}(3),'*r')
                        %drawnow
                        %hold on
                        this.Optical.CurrentFrameCount = this.Optical.CurrentFrameCount + 1;
                    end
                    %if strcmpi(this.Deployer.testScenario,'100m')
                    %    pos = pos(1:(I_stop-I_start),1:3);
                    %else
                    %    pos = pos(1:(I_stop-I_start)+1,1:3);
                    %end
                    
                    this.CombineResults(pos,t);
                else
                    % Just extrapolate TOF data to 10m
                    n = 100;
                    pos = cell(n,this.Deployer.GetNumCubesats());
                    t = linspace(0,t_10m,n);
                    for i = 1:n
                        for j = 1:this.Deployer.GetNumCubesats()
                            pos{i,j} = this.Deployer.CubesatArray(j).evalTofFit(t(i));
                        end
                    end
                    
                    for i = 1:this.Deployer.numCubesats
                        this.Deployer.CubesatArray(i).centroids_VCF = pos(:,i);
                        this.Deployer.CubesatArray(i).time = t;
                    end
                end

	        else
	        	error(strcat('Unable to read optical data files from ', this.Optical.DataDirec));
        	end
        end
        
        % Combine results from ComputeStateOutput and TOF processing
        %
        %
        % @return   this             Updated object with centroids_VCF and
        %                           time parameters
        % @author   Dylan Bossie
        % @date     11-Apr-2019
        function [this] = CombineResults(this,pos,t)
            CubeSats = this.Deployer.CubesatArray;
            
            for i = 1:length(CubeSats)
                
                iter_t = 1;
                iter_cs = 1;
                counter = 1;
                
                CubeSat_positions = cell(length(t)+length(CubeSats(i).time),1);
                CubeSat_times = zeros(length(t)+length(CubeSats(i).time),1);
                
                positions = pos(:,i);
                % Iterate through the two sets of time values to zip
                % together
                while iter_t <= length(t) && iter_cs <= length(CubeSats(i).time)
                    if t(iter_t) < CubeSats(i).time(iter_cs)
                        % Next time to save is from pos & t
                        CubeSat_times(counter) = t(iter_t);
                        CubeSat_positions(counter) = positions(iter_t);
                        iter_t = iter_t + 1;
                        counter = counter + 1;
                    elseif t(iter_t) > CubeSats(i).time(iter_cs)
                        % Next time to save is from CubeSats
                        CubeSat_times(counter) = CubeSats(i).time(iter_cs);
                        CubeSat_positions(counter) = {CubeSats(i).centroids_VCF(:,iter_cs)};
                        iter_cs = iter_cs + 1;
                        counter = counter + 1;
                    else
                        % Times magically align perfectly
                        CubeSat_times(counter) = t(iter_t);
                        CubeSat_positions(counter) = pos(iter_t);
                        iter_t = iter_t + 1;
                        counter = counter + 1;
                    end
                end
                
                % For when MATLAB ignores the rest of the data after one of
                % the indices reaches end
                if iter_cs <= length(CubeSats(i).time)
                    iter_init = iter_cs;
                    for j = iter_init:length(CubeSats(i).time)
                        CubeSat_times(counter) = CubeSats(i).time(iter_cs);
                        CubeSat_positions(counter) = {CubeSats(i).centroids_VCF(:,iter_cs)};
                        iter_cs = iter_cs + 1;
                        counter = counter + 1;
                    end
                elseif iter_t <= length(t)
                    iter_init = iter_t;
                    for j = iter_init:length(t)
                        CubeSat_times(counter) = t(iter_t);
                        CubeSat_positions(counter) = positions(iter_t);
                        iter_t = iter_t + 1;
                        counter = counter + 1;
                    end   
                else
                    continue
                end
                
                this.Deployer.CubesatArray(i).centroids_VCF = CubeSat_positions;
                this.Deployer.CubesatArray(i).time = CubeSat_times;
            end
        end
        
        %
        % Evaluate a given CubeSats TOFfit at a certain timestamp
        %
        % @param	CubeSat     an instance of the CubeSat class containing
        %                       a populated 1x3 TOFfit cell array
        % @param    predTime    time at which to predict the CubeSats
        %                       location, must be in same timeframe as
        %                       CubeSat.time
        %
        % @return   1x3 predicted CubeSat location
        %
        % @author   Joshua Kirby
        % @date     24-Mar-2019
        function [predPos] = PredictPositionFromTOF(~,CubeSat,time)
            predPos = zeros(3,1);
            for i = 1:3
                predPos(i) = CubeSat.TOFfit{i}(time);
            end
        end

        % A method for approximating a cubesat centroid using a weighted
        % centroiding method
        %
        % @param      this               The object
        % @param      isSystemCentroid  Indicates if system centroid
        % @param      camOrigin         camera origin in the VANTAGE Cartesian
        %                               Frame
        % @param      camVecs           cell array of vectors pointing from
        %                               camera origin to estimated centroid in
        %                               VCF
        % @param      pos_TOF           VCF position estimates from the TOF
        %                               sensor suite
        %
        % @return     pos       position estimate of cubesat centroids in the VCF
        %             frame using both sensor method returns
        %
        function [pos] = RunSensorFusion(this, isSystemCentroid, camOrigin, camVecs, pos_TOF)

        	% Initialize position cell array
        	numCubesats = this.Deployer.GetNumCubesats();
        	pos = cell(numCubesats,1);
            
            if numel(camVecs)>1
            end

        	% Perform sensor fusion
            if isSystemCentroid

        		% Find system centroid from TOF estimates
        		meanTOF = zeros(3,1);
        		for i = 1:numCubesats
        			meanTOF = meanTOF + pos_TOF{i};
        		end
        		meanTOF = meanTOF./numCubesats;

        		% Run sensor fusion on system centroid estimates
        		tmp = SensorFusion(this, camOrigin, camVecs{end}', meanTOF);

        		% Calculate adjustment vector
        		sensorFusionDiff = tmp-meanTOF;

        		% Adjust TOF vectors to find new centroids
        		for i = 1:numCubesats
        			pos{i} = pos_TOF{i} + sensorFusionDiff;
        		end
        	else
        		% Loop through estimates individually and perform sensor fusion
                for i = 1:numCubesats
        			pos{i} = SensorFusion(this, camOrigin, camVecs{i}, pos_TOF{i});
                end
            end
        end
        
        % A method for approximating a cubesat centroid using a weighted
        % centroiding method
        %
        % @param      this        The object
        % @param      camOrigin  camera origin in the VANTAGE Cartesian Frame
        % @param      camVec     vector pointing from camera origin to estimated
        %                        centroid in VCF
        % @param      pos_TOF    VCF position estimate from the TOF sensor suite
        %
        % @return     pos       position estimate of cubesat centroid in the VCF
        %                       frame using both sensor method returns
        %
        function [pos] = SensorFusion(this, camOrigin, camVec, pos_TOF)
            
            % Make everything a column vector
            if numel(camOrigin) > size(camOrigin,1)
                camOrigin = camOrigin';
            end
            if numel(camVec) > size(camVec,1)
                camVec = camVec';
            end
            if numel(pos_TOF) > size(pos_TOF,1)
                pos_TOF = pos_TOF';
            end
            
            % Normalize camera vector
            camVec = camVec./norm(camVec);

            % Define line vector
            a = camOrigin - camVec;
            
            % Define vector from line to TOF point
            b = pos_TOF - camVec;
            
            % Calculate vector cross product
            vec = cross(a,b);
            
            % Calculate cam and TOF weights
            sig_TOF = this.TOF.TofWeighting(norm(pos_TOF));
            sig_cam = this.Optical.OpticalWeighting(norm(pos_TOF));
            
            % Calculate distance along line for weigthed center point
            d = norm(vec) / norm(a);
            if sig_TOF~=0
                q = sig_TOF./(sig_cam+sig_TOF);
            else
                q = 0;
            end
            d_q = d.*q;
            
            % Calculate position for camera
            mag_TOF = norm(pos_TOF);
            mag_cam = sqrt(mag_TOF.^2 - d.^2);
            pos_cam = camOrigin + mag_cam.*camVec;
            
            % Calculate error-weighted centroid position
            posDir = pos_cam - pos_TOF; posDir = posDir./norm(posDir);
            pos = pos_TOF + posDir.*d_q;
        end
        
        % I don't know what exactly this method is supposed to do
        %
        % @param      this   The object
        %
        % @return     { description_of_the_return_value }
        %
        function PredictionModel(~)
            
        end
        
        % A method for outputting the state vectors for all identified cubesats
        %
        % @param      sampleparameter sampledescription
        %
        % @return     samplereturn
        function OutputStateVector(~)
            
        end

        %% ransac line fitting for obfuscation
        %
        % This function uses the ransac algorithm to fit a line to noisy data
        %
        % @param      x            x coordinates to fit
        % @param      y            y coordinates to fit
        % @param      maxDistance  The maximum distance from the line
        %
        % @return     polyfit line
        %
        % @author       Justin Fay
        % @date         21-Feb-2019
        function p = ransacLine(this,x,y,maxDistance)
            points = [x,y];
            sampleSize = 2; % number of points to sample per trial

            fitLineFcn = @(points) polyfit(points(:,1),points(:,2),1); % fit function using polyfit
            evalLineFcn = ...   % distance evaluation function
              @(model, points) sum((points(:, 2) - polyval(model, points(:,1))).^2,2);

            [~, inlierIdx] = ransac(points,fitLineFcn,evalLineFcn, ...
              sampleSize,maxDistance);
          
            p = polyfit(points(inlierIdx,1),points(inlierIdx,2),1);
        end
        
    end
    
    methods (Access = private)
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
        function Truth = processTruthData(this,truthFilename)
            % Read json truth file
            tmp = jsondecode(fileread(truthFilename));
            
            % extract date0
            Truth = struct;
            if strcmpi(this.Deployer.testScenario,'Simulation')
                Truth.t0_datevec = datevec(tmp(1).t);
            else
                Truth.t0_datevec = datevec(tmp{1},this.TimeMan.TruthDateFormat);
                tmp = tmp{2}; % reset tmp
            end
            
            
            % extract timesteps
            Truth.t = [tmp.t];
            
            % Order Cubesats [first-out to last-out]
            cubesatNamesUnordered = fieldnames(tmp(1).pos);
            z = zeros(length(cubesatNamesUnordered),1);
            for i = 1:length(cubesatNamesUnordered)
                z(i) = tmp(1).pos.(cubesatNamesUnordered{i})(3);
            end
            [~,I] = sort(z,'descend');
            cubesatNames = cubesatNamesUnordered(I);
            
            % Extract cubesat position data in meters
            for i = 1:length(cubesatNames)
                for j = 1:length(tmp)
                    Truth.Cubesat(i).pos(j,:) = tmp(j).pos.(cubesatNames{i});
                end
                if strcmpi(this.Deployer.testScenario,'Simulation')
                    Truth.Cubesat(i).pos = Truth.Cubesat(i).pos./100;
                elseif strcmpi(this.Deployer.testScenario,'100m') || strcmpi(this.Deployer.testScenario,'Modular')
                    % do nothing
                else
                    error('unhandled testScenario in Deployer object, must be ''100m'', ''Modular'', or ''Simulation''')
                end
            end
            
            % Extract number cubesats
            Truth.numCubeSats = length(cubesatNames);
        end
        
        
    end
end