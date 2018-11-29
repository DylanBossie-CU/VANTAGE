function grabFrames(video)
%%% Options to plot grayscale centroid and binarized images
plotGrayscale = 1;
plotBinarized = 0;
i = 1;
while hasFrame(video)
    frame = readFrame(video);
    i = i + 1;
    %Grab data in intervals of 1sec
    if video.currentTime == floor(video.currentTime)
        FindCentroid_Video(frame,plotGrayscale,1,plotBinarized,...
            1,1,i)
        imshow(frame);
    end
    
end
end