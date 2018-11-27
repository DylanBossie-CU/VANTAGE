%%%%%Dylan Bossie
%%%%%10/7/2018
%%%%%VANTAGE
%%%%%PDR Object Detection Feasibility
clc
close all
clear

%Resolution of Jerry's camera
%jerry = 7360*4912;

%%% Options to plot grayscale centroid and binarized images
plotGrayscale = 0;
plotBinarized = 0;


%%% Search desired image folder for relevant images
imageDirectory = 'Images/';
<<<<<<< HEAD
imageSet = '8*';
if strcmp(imageSet,'8*')
    imageType = 'RealImages';
elseif strcmp(imageSet,'multi')
    imageType = 'MultiObject';
else
    imageType = 'Misc';
end
images = strcat(imageDirectory,imageSet);
=======
images = strcat(imageDirectory,'8*');
>>>>>>> c195aac1374340632a182fc09e5f0ee7ef6e124e
imageFiles = dir(images);

%Preallocate array to store number of pixels for the cubesats in each image
tic
number_of_pixels = zeros(length(imageFiles),1);
FindCentroid(imageFiles,plotGrayscale,number_of_pixels,plotBinarized,imageDirectory);
toc
fprintf('Number of images processed: %d\n',length(imageFiles));
