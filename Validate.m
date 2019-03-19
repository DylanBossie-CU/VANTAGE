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
            if nargin == 1
                % Obtain data from json file
                config = jsondecode(fileread(configFilename));

                % Set transforms between TCF and VCF
                obj = obj.setTdata('VCF',config.VT_DCM,config.V_TCF2VCF,'TCF');

                % Set transforms between CCF and VCF
                obj = obj.setTdata('VCF',config.VC_DCM,config.V_CCF2VCF,'CCF');
            else
                error('Incorrect number of input arguments for initialization of Transform Class instance')
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
        function validateTOF()
            
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
        function validateTOF()
            
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