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
            DesiredFPS = true;
            PlotBinarizedImages = true;
            PlotCentroids = true;
            OpticalTest = OpticalTest.setOpticalData(DesiredFPS,PlotBinarizedImages,...
                PlotCentroids);
            
            %%% Process input video frames through Optical class
            OpticalTest.CurrentFrameCount = 1;
            while hasFrame(OpticalTest.Video)
                [OpticalTest,didRead] = OpticalTest.readInputFrame();
                if didRead
                    break
                end
                OpticalTest.CurrentFrameCount = OpticalTest.CurrentFrameCount + 1;
            end
            
            % Perform comparison
            testCase.verifyEqual(OpticalTest.Frame,testFrame.frame)
        end

    end
    
    
    methods (Access = private)
    end

end
