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
            import VANTAGE.PostProcessing.Optical
            
            %Define test video path
            VideoPath = 'Data/';
            VideoFile = strcat(VideoPath,'Test_StraightTraj.mp4');
            Video = VideoReader(VideoFile);
            
            %Load test frame to be compared
            testFrame = load('Data/Test_Optical/FirstFrame_Test.mat');
            
            %Create optical class to be tested
            OpticalTest = Optical;
            OpticalTest.Video = Video;
            
            %Set P.P. to only process video at 1FPS (skip frames in between)
            DesiredFPS = 1;
            FrameIntervals = linspace(0,1,DesiredFPS+1);
            PlotBinarizedImages = false;
            PlotCentroids = false;
            VideoType = 'StraightOn';
            OpticalTest = OpticalTest.setOpticalData(DesiredFPS,PlotBinarizedImages,...
                PlotCentroids,VideoType,FrameIntervals);
            
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
        
        function testMultiObjectDetection(testCase)
            return
            close all
            import VANTAGE.PostProcessing.Optical
            
            %Define test video path
            VideoPath = 'Data/';
            VideoFile = strcat(VideoPath,'TwoObjectsRender.mp4');
            Video = VideoReader(VideoFile);
            
            %Create optical class to be tested
            OpticalTest = Optical;
            OpticalTest.Video = Video;
            
            %Set P.P. to only process video at desired FPS, skipping frames
            %Cannot set higher than actual camera FPS
            DesiredFPS = 10;
            FrameIntervals = linspace(0,1,DesiredFPS+1);
            PlotBinarizedImages = true;
            PlotCentroids = true;
            VideoType = 'TwoObjectsRender';
            OpticalTest = OpticalTest.setOpticalData(DesiredFPS,PlotBinarizedImages,...
                PlotCentroids,VideoType,FrameIntervals);
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            while hasFrame(OpticalTest.Video)
                [OpticalTest,~] = OpticalTest.readInputFrame();
                OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
            end
        end

        
        function testAlignedCubeSats(testCase)
            close all
            import VANTAGE.PostProcessing.Optical
            
            %Define test video path
            VideoPath = 'Data/';
            VideoFile = strcat(VideoPath,'AlignedCubeSats.mp4');
            Video = VideoReader(VideoFile);
            
            %Create optical class to be tested
            OpticalTest = Optical;
            OpticalTest.Video = Video;
            
            %Set P.P. to only process video at desired FPS, skipping frames
            %Cannot set higher than actual camera FPS
            DesiredFPS = 1;
            FrameIntervals = linspace(0,1,DesiredFPS+1);
            PlotBinarizedImages = true;
            PlotCentroids = true;
            VideoType = 'AlignedCubeSats';
            OpticalTest = OpticalTest.setOpticalData(DesiredFPS,PlotBinarizedImages,...
                PlotCentroids,VideoType,FrameIntervals);
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            while hasFrame(OpticalTest.Video)
                [OpticalTest,~] = OpticalTest.readInputFrame();
                OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
            end
        end
    end
    
    
    methods (Access = private)
    end

end
