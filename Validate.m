classdef Validate
    %% Properties
    properties(Access = public)
        %
        
    end
    
    
    %% Public methods
    methods
        %% Class Constructor
        %
        % Constructs class 
        %
        % @param    
        %
        % @return   Class instance
        % 
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function obj = Validate()
            
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
        function validateTOF()
            
        end
        
        %% Validate Optical
        %
        % 
        % 
        % @param    
        %
        % @return 
        %
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function validateOptical()
            
        end
        
        %% Validate VANTAGE
        %
        % 
        % 
        % @param    
        %
        % @return 
        %
        % @author   Joshua Kirby
        % @date     19-Mar-2019
        function validateVantage()
            
        end
    end
    %% Private methods
    methods (Access = private)
        %% Present TOF Errors vs Reqs
        function TOFpresentErrorsVsReqs(Model,SensorData,truthFileName)
            % TOF Processing
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
                Model.Deployer,'presentResults',0,'fileLims',fileLims,'showDebugPlots',0);

            % Truth Data processing
        end
    end
    
    
    
    
    
    
end