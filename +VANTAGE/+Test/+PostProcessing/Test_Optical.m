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
            return
            close all
            import VANTAGE.PostProcessing.*
            %Commenting this out while Model/Deployer constructor work is
            %being done
            %ModelTest = Model;
            %Initialize Deployer
            %DeployerTest = Deployer('config/Manifest.json','config/Deployer.json',ModelTest);
            %Create optical class to be tested
            OpticalTest = Optical('./Config/Optical.json', 6);
        end
        
        function testPixelTransform(testCase)
            return
            close all
            import VANTAGE.PostProcessing.*
            OpticalTest = Optical('./Config/Optical.json',6);
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            while hasFrame(OpticalTest.Video)
                if OpticalTest.Frame ~= []
                    [OpticalTest,~] = OpticalTest.readInputFrame();
                    OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
                else
                    CubeSatUnitVectors = OpticalTest.PixelToUnitVec(OpticalTest.CubeSats);
                    break
                end
            end
        end
        
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
        
        function testSimulatedData(testCase)
            close all
            import VANTAGE.PostProcessing.*
            truthFilename = 'config/Testing/TruthDataTest.json';
            manifestFilename = 'Config/Manifest.json';
            Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename);
            OpticalTest = Model.Optical;
            
            dataFiles = dir(strcat(OpticalTest.DataDirec,OpticalTest.FileExtension));
            for i = 1:length(dataFiles)
                if ~dataFiles(i).isdir
                    frameTitle = dataFiles(i).name;
                    frame = imread(strcat(OpticalTest.DataDirec,frameTitle));
                    [~,centroids,CubeSatBoundaries] = OpticalTest.ImageProcessing(frame);
                end
            end
            
        end
        
    end
    
    
    methods (Access = private)
    end

end
