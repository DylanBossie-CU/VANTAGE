classdef VANTAGE < handle
    properties
        Deployer
        Transform
        OpticalCamera
        TOFCamera
    end
    methods
        %%%% Class Constructor:
        % @param      Deployer    The deployer tube VANTAGE is mounted in
        % @param      Transform   unsure, filling out in a second
        % @param      OpticalCamera Optical camera used for image capture
        % @param      TOFCamera TOF camera used for point cloud capture
        %
        % @return     A reference to an initialized model object
        %
        % @author     Dylan Bossie
        % @date       11/13/2018
        %%%%
        function obj = VANTAGE()
            obj.Deployer = 1;
            obj.Transform = "VCF";
            obj.OpticalCamera = "Nikon Tactical Nuclear Missile";
            obj.TOFCamera = "Cloudy boi";
        end
        
        % A method for synchronizing timestamps between the TOF and optical
        % cameras.
        %
        % @param      sampleparameter sampledescription
        %
        % @return     samplereturn
        function TimeStampSync(obj)
            
        end
        
        function TOFPropagate(obj)
            
        end
        
        function SensorFusion(obj)
            
        end
        
        function PredictionModel(obj)
            
        end
        
        function OutputStateVector(obj)
            
        end
        
    end
end