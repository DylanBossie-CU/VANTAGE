function FindCentroid(imageFiles,plotGrayscale,~,plotBinarized,...
    imageDirectory,imageType)

for i=1:length(imageFiles)
    I = imread(strcat(imageDirectory,imageFiles(i).name));
    %%%%% Remove camera blur from raw image
    %I = DeblurImage(I);
    I_gray = rgb2gray(I);
    
    binaryTolerance = 0.3;
    I_binarized = imbinarize(I_gray,binaryTolerance);
    [I_boundaries,~,~,~] = bwboundaries(I_binarized);
    
    %%%%% Find the largest boundaries (the cubesats)
    [s,~] = cellfun(@size,I_boundaries);
    [s,si] = sort(s,'descend');
    I_boundaries = I_boundaries(si,:);
    
    %%%%% Smooth the boundaries found in the image (currently extremely
    %%%%% broken lol)
    %I_boundaries = smoothBoundaries(I_boundaries,I_gray);

    objects = detectObjects(I_boundaries,s,si);
    
    boundingRectangles = findBoundingRectangles(objects,I_gray);
    %{
    if plotGrayscale == 1
        figure
        imshow(I_gray,'InitialMagnification',800);
        hold on
        sz = 600;
    end
    %}
    for j=1:length(objects)
        %objectBoundary = objects{j};
        boundingRectangle = boundingRectangles{j};
        %%%% Plotting grayscale image overlaid with cube outline
        %%%% and geometric centroid overlaid
        if plotGrayscale == 1
            %plotEdgeCentroid(objectBoundary,j)
            plotBoundingCentroid(boundingRectangle,j)
        end
    end
    [object_pixels,image_cropped] = FindCubeSatPixels(objects,I_binarized);
    
    if plotGrayscale == 1
        for j = 1:length(object_pixels)
            xLocation = 1*j;
            yLocation = 300*j;
            str = strcat(num2str(j),':  ', num2str(object_pixels(j)),'px');
            text(xLocation,yLocation,str,'Color','red',...
                    'FontSize',30);
        end
        saveas(gcf,['BoundingImageOutputs/',imageType,'/',...
            imageType,num2str(i),'.jpg'])
    end
    
    if plotBinarized == 1
        figure
        imshow(image_cropped,'InitialMagnification','fit');
        title({['Image Number: ' , imageFiles(i).name] , ...
            ['Number of Pixels: ',num2str(object_pixels)]})
        saveas(gcf,['BinarizedImageOutputs/','binarized_',imageFiles(i).name]);
    end
    
end

end