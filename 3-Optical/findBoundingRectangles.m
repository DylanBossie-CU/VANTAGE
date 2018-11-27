function boundingRectangles = findBoundingRectangles(boundaryPixels,I_gray)
boundingRectangles = cell(length(boundaryPixels),1);
figure
imshow(I_gray)
hold on
for i = 1:length(boundaryPixels)
    boundary = boundaryPixels{i};
    [rectx,recty] = ...
        minboundrect(boundary(:,2),boundary(:,1),'p',I_gray);
    boundingRectangles{i} = [rectx,recty];
end
title('CubeSat Boundary with Minimum Bounding Rectangle')
end