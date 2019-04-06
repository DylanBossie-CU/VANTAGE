classdef Test_Fusion < matlab.unittest.TestCase
    properties
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        function test100mData(testCase)
            close all
            import VANTAGE.PostProcessing.*
            truthFilename = 'config/Testing/TruthDataTest.json';
            manifestFilename = 'Config/Manifest.json';
            Model = VANTAGE.PostProcessing.Model(manifestFilename,'./Config/Testing/Dylan');
            OpticalTest = Model.Optical;
            
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
                Model.Deployer,'presentResults',1,'fileLims',fileLims,'showDebugPlots',0);
            % Truth Data processing
            Truth = obj.processTruthData(truthFileName);
            
            
            Model.ComputeStateOutput;
            
            % Unit vector to VCF origin from CCF
            UnitOriginVCF = Model.Deployer.GetCamOriginVCF;
        end
        
    end
    
    
    methods (Access = private)
    end

end
