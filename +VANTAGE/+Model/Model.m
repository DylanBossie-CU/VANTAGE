classdef Model
    properties (SetAccess = public)
        Deployer
    end
    properties (SetAccess = protected)
        Transform
    end
    properties (SetAccess = private)
        Optical
        TOF
    end

    methods
        %%%% Class Constructor:
        % @param      Deployer      The deployer class
        % @param      Transform     Transform class for all coordinate transformations
        % @param      Optical       Optical camera class used for image capture
        % @param      TOF           TOF camera class used for point cloud capture
        %
        % @return     A reference to an initialized model object
        %
        % @author     Justin Fay
        % @date       01/22/2019
        %%%%
        function obj = Model()
            %obj.Deployer = 
            %obj.Transform = 
            %obj.Optical = 
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
        
        % A method for propogating TOF predictions beyond the range of the TOF sensor
        %
        % @param      sampleparameter sampledescription
        %
        % @return     samplereturn
        function TOFPropagate(obj)
            
        end
        
        % A method for approximating a cubesat centroid using a weighted centroiding method
        %
        % @param      camOrigin camera origin in the VANTAGE Cartesian Frame
        % @param      camVec    vector pointing from camera origin to estimated centroid in VCF
        % @param      pos_TOF   VCF position estimate from the TOF sensor suite
        % @param      sig_cam   uncertainty in camera estimate in the VCF
        % @param      sig_TOF   uncertainty in the TOF estimate in the VCF
        %
        % @return     pos       position estimate of cubesat centroid in the VCF frame using both sensor method returns
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
        % @param      sampleparameter sampledescription
        %
        % @return     samplereturn
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