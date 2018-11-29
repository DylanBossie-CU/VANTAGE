function grabFrames(video)
i = 1;
while hasFrame(video)
    if video.currentTime == 1.0
        frame = readFrame(video);
        imshow(frame);
    end
    i = i + 1;
end
end