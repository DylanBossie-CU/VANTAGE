function cornerPoints = findCorners(boundaryPixels,I_gray)
%%%%% Corners can be defined by having one of the following properties:
%%%%% 1. No adjacent pixels that are both up and left
%%%%% 2. No adjacent pixels that are both up and right
%%%%% 3. No adjacent pixels that are both down and left
%%%%% 4. No adjacent pixels that are both down and right
%%%%% This function will perform a search on the current object's boundary
%%%%% pixels, and search for pixels with one of these properties. If one of
%%%%% these conditions is satisfied, that pixel will be considered a corner
%%%%% pixel.
%%%%% This is super useless and is being reworked
boundary = boundaryPixels{1};
minboundrect(boundary(:,2),boundary(:,1),I_gray)
figure
imshow(I_gray)
hold on
plot(boundary(:,2),boundary(:,1))
title('Current Object Boundary')
for i = 1:length(boundary)
    print('test')
end

end