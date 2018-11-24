function boundingRectangles = findBoundingRectangles(boundaryPixels,I_gray)
boundingRectangles = cell(length(boundaryPixels),1);
for i = 1:length(boundaryPixels)
    boundary = boundaryPixels{i};
    boundingRectangles{i} = ...
        minboundrect(boundary(:,2),boundary(:,1),'p',I_gray);
end
end