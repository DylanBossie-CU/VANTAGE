clc
close all
clear
%%% Define the video to be processed for current deployment
VideoPath = 'Data/';
VideoFile = strcat(VideoPath,'Test_StraightTraj.mp4');
Video = VideoReader(VideoFile);

%%% Define optical camera class to perform post-processing of the input
%%% video
Optical = VANTAGE.PostProcessing.Optical;

Optical.Video = Video;

%Set P.P. to only process video at 1FPS (skip frames in between)
DesiredFPS = true;
PlotBinarizedImages = true;
PlotCentroids = true;
Optical = Optical.setOpticalData(DesiredFPS,PlotBinarizedImages,...
    PlotCentroids);

%%% Process input video frames through Optical class
Optical.CurrentFrameCount = 1;
while hasFrame(Optical.Video)
    [Optical,didRead] = Optical.readInputFrame();
    Optical.CurrentFrameCount = Optical.CurrentFrameCount + 1;
end