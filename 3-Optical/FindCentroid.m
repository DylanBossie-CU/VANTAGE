function FindCentroid(imageFiles,plotGrayscale,~,plotBinarized,...
    imageDirectory,imageType)

for i=1:length(imageFiles)
    I = imread(strcat(imageDirectory,imageFiles(i).name));
    %%%% Remove camera blur from raw image
    {
    imshow(I)
    title('Blurry')
    I = DeblurImage(I);
    figure
    imshow(I)
    title('Deblurry boi')
    }
    I_gray = rgb2gray(I);
    
    binaryTolerance = 0.3;
    I_binarized = imbinarize(I_gray,binaryTolerance);
    imshow(I_binarized)
    [I_boundaries,~,~,~] = bwboundaries(I_binarized);
    
    %%%% Find the largest boundaries (the cubesats)
    [s,~] = cellfun(@size,I_boundaries);
    [s,si] = sort(s,'descend');
    I_boundaries = I_boundaries(si,:);
    
    %%%% Smooth the boundaries found in the image (currently extremely
    %%%% broken lol)
    I_boundaries = smoothBoundaries(I_boundaries,I_gray);

    objects = detectObjects(I_boundaries,s,si);
    
    edgeImage = createEdgeImage(objects{1},I_gray);
    
    boundingRectangles = findBoundingRectangles(objects,I_binarized);

    for j=1:length(objects)
        objectBoundary = objects{j};
        boundingRectangle = boundingRectangles{j};
        %%% Plotting grayscale image overlaid with cube outline
        %% and geometric centroid overlaid
        if plotGrayscale == 1
            plotEdgeCentroid(objectBoundary,j)
            plotBoundingCentroid(boundingRectangle,j)
        end
    end
    [object_pixels,image_cropped] = FindCubeSatPixels(objects,I_binarized);
    
    if plotGrayscale == 1
        for j = 1:length(object_pixels)
            xLocation = 0*j;
            yLocation = 0.2*j;
            str = strcat(num2str(j),':  ', num2str(object_pixels(j)),'px');
            text(xLocation,yLocation,str,'Color','red',...
                    'FontSize',10,'Units','normalized');
        end
        title(['Image ' num2str(i)])
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