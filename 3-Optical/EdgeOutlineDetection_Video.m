%%%%%Dylan Bossie
%%%%%10/7/2018
%%%%%VANTAGE
%%%%%CDR Object Detection Solution
clc
close all
clear

%Resolution of Jerry's camera
%jerry = 7360*4912;

%%% Options to plot grayscale centroid and binarized images
plotGrayscale = 1;
plotBinarized = 0;


%%% Search desired image folder for relevant images
videoDirectory = 'Videos/';
videoFile = 'Videos/vantagecamstraighttraj.mp4';
videoType = 'StraightOn';
video = VideoReader(videoFile);

grabFrames(video);

%Preallocate array to store number of pixels for the cubesat+s in each image
tic
number_of_pixels = zeros(length(videoFiles),1);
FindCentroid(videoFiles,plotGrayscale,number_of_pixels,plotBinarized,...
    videoDirectory,videoType);
toc
fprintf('Number of images processed: %d\n',length(videoFiles));
