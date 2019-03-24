classdef Model < handle
    % Model is a class used to store the estimation methods that utilize both
    % TOF and camera data.

    properties (SetAccess = public)
        % Deployer class
        Deployer
    end
    properties (SetAccess = protected)
        % Transform class
        Transform
    end
    properties (SetAccess = private)
        % Optical camera class
        Optical

        % TOF camera class
        TOF
        
        % Truth data
        Truth_VCF
        
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

        	% Construct child classes and process truth data
            obj.Deployer = Deployer(manifestFilename, strcat(configDirecName,'/Deployer.json'),obj);
            obj.Truth_VCF = obj.processTruthData(obj.Deployer.TruthFileName);
            obj.Transform = Transform(strcat(configDirecName,'/Transform.json'));
            obj.Optical = Optical(obj,strcat(configDirecName,'/Testing/Dylan/Optical.json'), obj.Deployer.GetNumCubesats());
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
        	[didRead,direc] = readInputFramesFromImages(obj.Optical);

            % Process last frame of optical data to find 100m pixel
            % location
            obj.Optical = obj.Optical.find100mPixel(direc(end));
            
        	if didRead
	        	% Loop though optical frames
	        	for i = 1:numel(direc)
	        		% Read frame
	        		obj.Optical.Frame = direc(i);

	        		% Run optical processing
	        		[CamUnitVecsVCF,CamOriginVCF, CamTimestamp, isSystemCentroid] =...
                        obj.Optical.OpticalProcessing(obj.Optical.Frame);

	        		% Get propogated TOF positions
                    %lol
	        		%pos_TOF = obj.TOF.propogatedShit(camTimestep)

	        		% Run sensor fusion
	        		%[pos] = RunSensorFusion(obj, isSystemCentroid, obj.Deployer.GetCamOriginVCF(), CamUnitVecsVCF, pos_TOF, sig_cam, sig_TOF);
	        	end
	        else
	        	error(strcat('Unable to read optical data files from ', obj.Optical.DataDirec));
        	end
        end
        
        % A method for synchronizing timestamps between the TOF and optical
        % cameras.
        %
        % @param      sampleparameter sampledescription
        %
        % @return     samplereturn
        function TimeStampSync(obj)
            
        end
        
        % A method for propogating TOF predictions beyond the range of the TOF
        % sensor for a single cubesat
        %
        % @param      obj       The object
        % @param      pos_init  The expected initial position of the cubesat at
        %                       the time of deployment in the VCF frame
        % @param      pos_TOF   A Nx3 matrix of VCF position data from the TOF
        %                       sensor
        % @param      t_TOF     The timestamps that correspond to the TOF
        %                       position data
        % @param      t_cam     The timestamps that correspond to the camera
        %                       position data
        %
        % @return     A cell array of propogated positions
        %
        function [pos_prop] = TOFPropagate(obj, pos_init, pos_TOF, t_TOF, t_cam)

            % Calculate mean TOF velocity from captured data
            V_TOF = mean(diff(pos_TOF)./diff(t_TOF),1);

            % Estimate position of cubesat centroid prior ro propogation
            pos_preProp = pos_init + t_TOF.*V_TOF;

            % Propogate position to camera timestamps
            pos_prop = pos_init + t_cam(1).*V_TOF + (t_cam-t_cam(1)).*V_TOF;
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
        % @param      sig_cam           uncertainty in camera estimates in the
        %                               VCF
        % @param      sig_TOF           uncertainty in the TOF estimates in the
        %                               VCF
        %
        % @return     pos       position estimate of cubesat centroids in the VCF
        %             frame using both sensor method returns
        %
        function [pos] = RunSensorFusion(obj, isSystemCentroid, camOrigin, camVecs, pos_TOF, sig_cam, sig_TOF)

        	% Initialize position cell array
        	numCubesats = obj.Deployer.GetNumCubesats();
        	pos = cell{numCubesats,1};

        	% Perform sensor fusion
        	if isSystemCentroid

        		% Find system centroid from TOF estimates
        		meanTOF = zeros(1,3);
        		for i = 1:numCubesats
        			meanTOF = meanTOF + pos_TOF{i};
        		end
        		meanTOF = meanTOF./numCubesats;

        		% Run sensor fusion on system centroid estimates
        		tmp = SensorFusion(obj, camOrigin, camVecs{1}, meanTOF, sig_cam, sig_TOF);

        		% Calculate adjustment vector
        		sensorFusionDiff = tmp-mean_TOF;

        		% Adjust TOF vectors to find new centroids
        		for i = 1:numCubesats
        			pos{i} = pos_TOF{i} + sensorFusionDiff;
        		end
        	else
        		% Loop through estimates individually and perform sensor fusion
        		for i = 1:numCubesats
        			pos{i} = SensorFusion(obj, camOrigin, camVecs{i}, pos_TOF{i}, sig_cam, sig_TOF);
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
        % @param      sig_cam    uncertainty in camera estimate in the VCF
        % @param      sig_TOF    uncertainty in the TOF estimate in the VCF
        %
        % @return     pos       position estimate of cubesat centroid in the VCF
        %                       frame using both sensor method returns
        %
        function [pos] = SensorFusion(obj, camOrigin, camVec, pos_TOF, sig_cam, sig_TOF)
            % Normalize camera vector
            camVec = camVec./norm(camVec);

            % Define line vector
            a = camOrigin - camVec;
            
            % Define vector from line to TOF point
            b = pos_TOF - camVec;
            
            % Calculate vector cross product
            vec = cross(a,b);
            
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