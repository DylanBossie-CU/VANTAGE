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
imageDirectory = 'Images/';
imageSet = 'multi*';
if strcmp(imageSet,'8*')
    imageType = 'RealImages';
elseif strcmp(imageSet,'multi')
    imageType = 'MultiObject';
else
    imageType = 'Misc';
end
images = strcat(imageDirectory,imageSet);
imageFiles = dir(images);

%Preallocate array to store number of pixels for the cubesat+s in each image
tic
number_of_pixels = zeros(length(imageFiles),1);
FindCentroid(imageFiles,plotGrayscale,number_of_pixels,plotBinarized,...
    imageDirectory,imageType);
toc
fprintf('Number of images processed: %d\n',length(imageFiles));
