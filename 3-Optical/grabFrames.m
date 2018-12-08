function distanceError = grabFrames(video)
%%% Options to plot grayscale centroid and binarized images
plotGrayscale = 0;
plotBinarized = 0;
i = 1;
distanceError = [];
while hasFrame(video)
    frame = readFrame(video);
    i = i + 1;
    %Grab data in intervals of 1sec
    if video.currentTime == floor(video.currentTime)
        distanceErrorFrame = FindCentroid_Video(frame,...
            plotGrayscale,1,plotBinarized,1,1,i);
        distanceError = [distanceError distanceErrorFrame];
        %imshow(frame);
    end
end
end