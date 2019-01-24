clc
close all
clear
%%% Define the video to be processed for current deployment
VideoPath = 'Data/';
VideoFile = strcat(VideoPath,'Test_StraightTraj.mp4');
Video = VideoReader(VideoFile);

%%% Define optical camera class to perform post-processing of the input
%%% video
Optical = VANTAGE.Optical.Optical;

Optical.Video = Video;

%Set P.P. to only process video at 1FPS (skip frames in between)
DesiredFPS = 1;
Optical = Optical.setOpticalData(DesiredFPS);

%%% Process input video frames through Optical class
Optical.CurrentFrame = 1;
while hasFrame(Optical.Video)
    Optical.readInputFrame()
    Optical.CurrentFrame = Optical.CurrentFrame + 1;
end