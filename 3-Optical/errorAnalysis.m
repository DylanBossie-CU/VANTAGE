function distanceError = errorAnalysis(centroid,centerpoint,boundary,depth)
%%%% CubeSat horizontal dimension is 10cm, this will be used to determine
%%%% the length of a single pixel
[pixelsize,cubesatLength] = CameraFOV(centerpoint(1)*2,centerpoint(2)*2,depth);
scatter(centerpoint(1),centerpoint(2),500,'r','+','LineWidth',3);
pixelerror = norm(centroid-centerpoint);
distanceError = pixelerror*pixelsize;
end