%%%%%Dylan Bossie
%%%%%10/7/2018
%%%%%VANTAGE
%%%%%CDR Object Detection Solution
clc
close all
clear

%%% Search desired image folder for relevant images
videoDirectory = 'Videos/';
videoFile = 'Videos/vantagecamstraighttraj.mp4';
videoType = 'StraightOn';
video = VideoReader(videoFile);

distanceError = grabFrames(video);

DR6_1 = ones(100,1)*10;

figure
plot(DR6_1)
hold on
plot(distanceError)



%Preallocate array to store number of pixels for the cubesat+s in each image
%{
tic
number_of_pixels = zeros(length(videoFiles),1);
distanceError = FindCentroid(videoFiles,plotGrayscale,number_of_pixels,plotBinarized,...
    videoDirectory,videoType);
toc
fprintf('Number of images processed: %d\n',length(videoFiles));
%}