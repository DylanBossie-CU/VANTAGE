function FindCentroid(imageFiles,plotGrayscale,~,plotBinarized,imageDirectory)

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
    [~,si] = sort(s,'descend');
    I_boundaries = I_boundaries(si,:);
    
    %%%%% Smooth the boundaries found in the image (currently operates on
    %%%%% the first boundary only)
    %smoothBoundary = smoothBoundaries(I_boundaries,I_gray);

    objects = detectObjects(I_boundaries,s,si);
    
    for j =1:length(objects)
        boundingRectangles = findBoundingRectangles(objects(j),I_gray);
    end
    
    if plotGrayscale == 1
        figure
        imshow(I_gray,'InitialMagnification',800);
        hold on
        sz = 600;
    end
    
    for j=1:length(objects)
        b = objects{j};
        %%%% Plotting grayscale image overlaid with cube outline
        %%%% and geometric centroid overlaid
        if plotGrayscale == 1
            scatter(mean(b(:,2)),mean(b(:,1)),sz,'r','+','LineWidth',5);
            str = num2str(j);
            text(mean(b(:,2)),mean(b(:,1))+200,str,'Color','red',...
                'FontSize',60);
            plot(b(:,2),b(:,1),'g','LineWidth',3);
        end
    end
    
    [object_pixels,image_cropped] = FindCubeSatPixels(objects,I_binarized);
    
    if plotGrayscale == 1
        for j = 1:length(object_pixels)
            xLocation = 1*j;
            yLocation = 60*j;
            str = strcat(num2str(j),':  ', num2str(object_pixels(j)),'px');
            text(xLocation,yLocation,str,'Color','red',...
                    'FontSize',30);
        end
        title(['Image ' , num2str(i)]);
        saveas(gcf,['OutlinedImageOutputs/','outlined_'...
            ,imageFiles(i).name]);
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