function boundingRectangles = findBoundingRectangles_Video(boundaryPixels,I_gray)
boundingRectangles = cell(length(boundaryPixels),1);
for i = 1:length(boundaryPixels)
    boundary = boundaryPixels{i};
    [rectx,recty] = ...
        minboundrect_Video(boundary(:,2),boundary(:,1),'p',I_gray);
    boundingRectangles{i} = [rectx,recty];
end
end