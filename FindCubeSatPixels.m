%function number_of_pixels = FindCubeSatPixels(b,imageName,I_binarized,...
function [object_pixels,image_cropped] = FindCubeSatPixels(objects,I_binarized)
    object_pixels = zeros(length(objects),1);
    for i = 1:length(objects)
        object = objects(i);
        %%%%% Crop binarized image to the region containing the cube, allowing
        %%%%% for accurate pixel count (binary pixels == 1)
        boundaryXStart = min(object{1}(:,2));
        boundaryXEnd = max (object{1}(:,2));
        boundaryYStart = min(object{1}(:,1));
        boundaryYEnd = max(object{1}(:,1));

        image_cropped = imcrop(I_binarized,[boundaryXStart,boundaryYStart,...
            boundaryXEnd-boundaryXStart,boundaryYEnd-boundaryYStart]);
        object_pixels(i) = length(find(image_cropped==1));
    end
end