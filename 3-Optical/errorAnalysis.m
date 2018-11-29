function distanceError = errorAnalysis(centroid,centerpoint,boundary)
%%%% CubeSat horizontal dimension is 10cm, this will be used to determine
%%%% the length of a single pixel
boundaryLength = boundary(2,1)-boundary(2,2)+1;
CubeSatLength = 10; %10cm
% Length of a single pixel for current image (cm)
pixelSize = CubeSatLength/boundaryLength;
pixelerror = norm(centroid-centerpoint);
distanceError = pixelerror*pixelSize;
end