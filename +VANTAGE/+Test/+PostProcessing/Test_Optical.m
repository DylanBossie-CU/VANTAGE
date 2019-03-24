classdef Test_Optical < matlab.unittest.TestCase
    properties
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        
        function testModularData(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            OpticalTest = Optical('yes','./Config/OpticalTest.json',6);
            
            frame = 'Data/Cropped/AIAA1.jpg';
            frame = imread(frame);
            OpticalTest.Frame = frame;
            [~,centroids,...
                CubeSatBoundaries] = OpticalTest.ImageProcessing(frame);
        end
        
        function testPipeline(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            truthFilename = 'config/Testing/TruthDataTest.json';
            manifestFilename = 'Config/Manifest.json';
            Model = VANTAGE.PostProcessing.Model(manifestFilename,'./Config');
            OpticalTest = Model.Optical;
            
            Model.ComputeStateOutput;
            
            % Unit vector to VCF origin from CCF
            UnitOriginVCF = Model.Deployer.GetCamOriginVCF;
        end
        
        function test100mData(testCase)
            close all
            import VANTAGE.PostProcessing.*
            truthFilename = 'config/Testing/TruthDataTest.json';
            manifestFilename = 'Config/Manifest.json';
            Model = VANTAGE.PostProcessing.Model(manifestFilename,'./Config/Testing/Dylan');
            OpticalTest = Model.Optical;
            
            Model.ComputeStateOutput;
            
            % Unit vector to VCF origin from CCF
            UnitOriginVCF = Model.Deployer.GetCamOriginVCF;
        end
        
    end
    
    
    methods (Access = private)
    end

end
