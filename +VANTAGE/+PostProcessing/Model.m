classdef Model < handle
    % Model is a class used to store the estimation methods that utilize both
    % TOF and camera data.

    properties (SetAccess = public)
        % Deployer class
        Deployer
        
        % Truth data
        Truth_VCF
    end
    
    properties (SetAccess = protected)
        % Transform class
        Transform
        
        % Time Manager Class
        TimeMan
    end
    
    properties (SetAccess = private)
        % Optical camera class
        Optical

        % TOF camera class
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
        function obj = Model(manifestFilename,configDirecName)
            
        	% Import child classes
        	import VANTAGE.PostProcessing.Deployer
        	import VANTAGE.PostProcessing.Transform
        	import VANTAGE.PostProcessing.Optical
        	import VANTAGE.PostProcessing.TOF
            import VANTAGE.PostProcessing.TimeManager

        	% Construct child classes and process truth data
            % NOTE: The order of these lines is very important
            obj.Deployer = Deployer(manifestFilename, strcat(configDirecName,'/Deployer.json'),obj);
            obj.Transform = Transform(strcat(configDirecName,'/Transform.json'));
            SensorData = jsondecode(fileread(strcat(configDirecName,'/Sensors.json')));
            obj.TimeMan = TimeManager(SensorData);
            obj.Truth_VCF = obj.processTruthData(obj.Deployer.TruthFileName);
            obj.TimeMan.syncTruthData(obj);
            obj.Optical = Optical(obj,strcat(configDirecName,'/Optical.json'), obj.Deployer.GetNumCubesats());
            obj.TOF = TOF(obj,strcat(configDirecName,'/TOF.json'));
            
            % Error catching
            if obj.Deployer.numCubesats ~= obj.Truth_VCF.numCubeSats
                error('Truth data and manifest do not agree on the number of Cubesats')
            end
        end

        % A method for looping through optical data and performing sensor fusion
        % to return a final state output
        %
        % @param      obj   The object
        %
        %
        function obj = ComputeStateOutput(obj)
        	% Get directory of optical frames
        	[didRead,direc,timestamps] = obj.Optical.readInputFramesFromImages(obj);

            [~,finalImageIndex] = max(timestamps);
            [~,firstImageIndex] = min(timestamps);
            
            [~,timestampIndices]= sort(timestamps);
            
            % Find background pixels in first optical image for background
            % subtraction
            BackgroundPixels = obj.Optical.FindBackground(direc(firstImageIndex));
            
            % Find pixel location of CubeSats in last image for object
            % association in optical data
            obj.Optical = obj.Optical.findLastImagePixel(direc(finalImageIndex),BackgroundPixels);
            
            tic
        	if didRead
	        	% Loop though optical frames
	        	for i = 2:numel(direc)
                    % Find current frame's index in timestamps
                    currentTime = find(timestampIndices==i);
                    
	        		% Read frame
	        		obj.Optical.Frame = direc(currentTime);

	        		% Run optical processing
	        		[obj.Optical,CamOriginVCF, CamTimestamp, isSystemCentroid] =...
                        obj.Optical.OpticalProcessing(obj.Optical.Frame,BackgroundPixels);

                    CamUnitVecsVCF = cell(numel(obj.Optical.CubeSats),1);
                    for j = 1:length(CamUnitVecsVCF)
                        CamUnitVecsVCF{j} = obj.Optical.CubeSats{j}.unitvec;
                    end
                    
                    if isSystemCentroid == 'invalid'
                        continue
                    end
                    
                    % Get propogated TOF positions
                    CubeSats = obj.Deployer.CubesatArray;
                    pos_TOF = zeros(numel(CubeSats),1);
                    
                    for j = 1:numel(CubeSats)
                        pos_TOF(j) = CubeSats(j).evalTofFit(t);
                    end

                    % Assign sensor fusion weights based on pos_TOF
                    
                    
	        		% Run sensor fusion
	        		%[pos] = RunSensorFusion(obj, isSystemCentroid, obj.Deployer.GetCamOriginVCF(), CamUnitVecsVCF, pos_TOF);
                    
                    obj.Optical.CurrentFrameCount = obj.Optical.CurrentFrameCount + 1;
                end
            toc
	        else
	        	error(strcat('Unable to read optical data files from ', obj.Optical.DataDirec));
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
        % @param      obj               The object
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
        function [pos] = RunSensorFusion(obj, isSystemCentroid, camOrigin, camVecs, pos_TOF)

        	% Initialize position cell array
        	numCubesats = obj.Deployer.GetNumCubesats();
        	pos = cell(numCubesats,1);

        	% Perform sensor fusion
            if isSystemCentroid

        		% Find system centroid from TOF estimates
        		meanTOF = zeros(1,3);
        		for i = 1:numCubesats
        			meanTOF = meanTOF + pos_TOF{i};
        		end
        		meanTOF = meanTOF./numCubesats;

        		% Run sensor fusion on system centroid estimates
        		tmp = SensorFusion(obj, camOrigin, camVecs{1}, meanTOF);

        		% Calculate adjustment vector
        		sensorFusionDiff = tmp-mean_TOF;

        		% Adjust TOF vectors to find new centroids
        		for i = 1:numCubesats
        			pos{i} = pos_TOF{i} + sensorFusionDiff;
        		end
        	else
        		% Loop through estimates individually and perform sensor fusion
                for i = 1:numCubesats
        			pos{i} = SensorFusion(obj, camOrigin, camVecs{i}, pos_TOF{i});
                end
            end
        end
        
        % A method for approximating a cubesat centroid using a weighted
        % centroiding method
        %
        % @param      obj        The object
        % @param      camOrigin  camera origin in the VANTAGE Cartesian Frame
        % @param      camVec     vector pointing from camera origin to estimated
        %                        centroid in VCF
        % @param      pos_TOF    VCF position estimate from the TOF sensor suite
        %
        % @return     pos       position estimate of cubesat centroid in the VCF
        %                       frame using both sensor method returns
        %
        function [pos] = SensorFusion(obj, camOrigin, camVec, pos_TOF)
            % Normalize camera vector
            camVec = camVec./norm(camVec);

            % Define line vector
            a = camOrigin - camVec;
            
            % Define vector from line to TOF point
            b = pos_TOF - camVec;
            
            % Calculate vector cross product
            vec = cross(a,b);
            
            % Calculate cam and TOF weights
            sig_TOF = obj.TOF.TofWeighting(norm(pos_TOF));
            sig_cam = obj.Optical.OpticalWeighting(norm(pos_TOF));
            
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
        % @param      obj   The object
        %
        % @return     { description_of_the_return_value }
        %
        function PredictionModel(obj)
            
        end
        
        % A method for outputting the state vectors for all identified cubesats
        %
        % @param      sampleparameter sampledescription
        %
        % @return     samplereturn
        function OutputStateVector(obj)
            
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
        function Truth = processTruthData(obj,truthFilename)
            % Read json truth file
            tmp = jsondecode(fileread(truthFilename));
            
            % extract date0
            Truth = struct;
            Truth.t0_datevec = datevec(tmp{1},obj.TimeMan.TruthDateFormat);
            tmp = tmp{2}; % reset tmp
            
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
                    Truth.Cubesat(i).pos(j,:) = tmp(j).pos.(cubesatNames{i});
                end
                if strcmpi(obj.Deployer.testScenario,'Simulation')
                    Truth.Cubesat(i).pos = Truth.Cubesat(i).pos./100;
                elseif strcmpi(obj.Deployer.testScenario,'100m') || strcmpi(obj.Deployer.testScenario,'Modular')
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