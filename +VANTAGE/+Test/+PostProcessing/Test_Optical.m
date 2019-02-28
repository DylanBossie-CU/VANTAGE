classdef Test_Optical < matlab.unittest.TestCase
    properties
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        %% Test input frame reading
        % This function tests if the code grabs the first frame of a video
        % correctly, based on a provided FPS to be examined
        
        function testReadInputFrame(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            %Initialize Deployer
            DeployerTest = Deployer('config/Manifest.json');
            %Create optical class to be tested
            OpticalTest = Optical('./Config/Optical.json', DeployerTest.GetNumCubesats());
            
            %Load test frame to be compared
            testFrame = load('Data/Test_Optical/FirstFrame_Test.mat');
            
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            FirstFrameGrabbed = false;
            while hasFrame(OpticalTest.Video)
                [OpticalTest,didRead] = OpticalTest.readInputFrame();
                if didRead && FirstFrameGrabbed == false
                    FirstFrame = OpticalTest.Frame;
                    FirstFrameGrabbed = true;
                end
                
                if didRead && OpticalTest.PlotBinarizedImages == false
                    break
                end

                OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
            end
            
            % Perform comparison
            testCase.verifyEqual(FirstFrame,testFrame.frame)
        end

        function testOcclusionCut(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            %Initialize Deployer
            DeployerTest = Deployer('config/Manifest.json');
            %Create optical class to be tested
            OpticalTest = Optical('./Config/Optical.json', DeployerTest.GetNumCubesats());
        end
        
        function testMultiObjectDetection(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            %Initialize Deployer
            DeployerTest = Deployer('config/Manifest.json');
            %Create optical class to be tested
            OpticalTest = Optical('./Config/Optical.json', DeployerTest.GetNumCubesats());
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            while hasFrame(OpticalTest.Video)
                [OpticalTest,~] = OpticalTest.readInputFrame();
                OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
            end
        end

        
        function testAlignedCubeSats(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            %Initialize Deployer
            DeployerTest = Deployer('config/Manifest.json');
            %Create optical class to be tested
            OpticalTest = Optical('./Config/Optical.json', DeployerTest.GetNumCubesats());
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            while hasFrame(OpticalTest.Video)
                [OpticalTest,~] = OpticalTest.readInputFrame();
                OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
            end
        end
        
        function testObjectAssociation(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            %Initialize Deployer
            DeployerTest = Deployer('config/Manifest.json');
            %Create optical class to be tested
            OpticalTest = Optical('./Config/Optical.json', DeployerTest.GetNumCubesats());
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            while hasFrame(OpticalTest.Video)
                [OpticalTest,~] = OpticalTest.readInputFrame();
                OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
            end
        end
        
        function testOpticalConstruction(testCase)
            close all
            import VANTAGE.PostProcessing.*
            %Initialize Deployer
            DeployerTest = Deployer('config/Manifest.json');
            %Create optical class to be tested
            OpticalTest = Optical('./Config/Optical.json', DeployerTest.GetNumCubesats());
        end
    end
    
    
    methods (Access = private)
    end

end
