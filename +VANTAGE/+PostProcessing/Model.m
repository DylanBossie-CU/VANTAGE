classdef Model
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
    end

    methods
        % Class Constructor:
        % @param      Deployer   The deployer class
        % @param      Transform  Transform class for all coordinate transformations
        % @param      Optical    Optical camera class used for image capture
        % @param      TOF        TOF camera class used for point cloud capture
        %
        % @return     A reference to an initialized Model object
        %
        function obj = Model()

        	% Import child classes
        	import VANTAGE.PostProcessing.Deployer
        	import VANTAGE.PostProcessing.Transform
        	import VANTAGE.PostProcessing.Optical
        	import VANTAGE.PostProcessing.TOF

        	% Construct child classes
            obj.Deployer = Deployer('./Config/Manifest.json', './Config/Deployer.json')
            obj.Transform = Transform('./Config/Transform.json');
            obj.Optical = Optical(obj.Deployer)
            %obj.TOF = 
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
        % @return     pos_prop A Nx3 matrix of propogated positions
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
end