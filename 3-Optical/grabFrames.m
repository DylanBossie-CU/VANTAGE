function grabFrames(video)
i = 1;
while hasFrame(video)
    frame = readFrame(video);
    %Grab data in intervals of 1sec
    if video.currentTime == floor(video.currentTime)
        %disp(video.currentTime)
        imshow(frame);
    end
    i = i + 1;
end
end