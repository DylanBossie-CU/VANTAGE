classdef Model < handle
    properties
        Deployer
        Transform
        Optical
        TOF
    end
    methods
        %%%% Class Constructor:
        % @param      Deployer      The deployer tube VANTAGE is mounted in
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
            obj.Deployer = 1;
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
        % @param      sampleparameter sampledescription
        %
        % @return     samplereturn
        function SensorFusion(obj)
            
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