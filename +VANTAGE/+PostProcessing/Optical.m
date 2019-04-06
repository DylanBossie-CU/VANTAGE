classdef Optical 
  %% Classdef
  % This class will contain the functions necessary for the post-processing
  % of optical camera data
  %% Properties
  properties(Access = private)
    % Width of camera resolution
    width = 3088;
    
    % Height of camera resolution
    height = 2076;
  end
  
  properties(Access = public)
    
    % Current frame of data folder
    CurrentFrameCount
    
    % Desired FPS
    DesiredFPS
    
    % Current frame
    Frame
    
    % Processed frame
    Image
    
    % Timesteps corresponding to desired FPS
    FrameIntervals
    
    % CubeSat centroid data containers
    CubeSats

    % Daat directory
    DataDirec
    
    % Plotting option (binarized images)
    PlotBinarizedImages
    
    PlotCentroids
    
    PlotHist
    
    PlotAny
    
    PlotCubeSats

    % Model handle class
    ModelRef

    % Data filenames
    FileExtension
    
    % Pixel location of system centroid at 100m
    PixelLocation100m
    
    % Check if performing search for 100m pixel
    is100mSearch
    
    % Run occlusion centroid detection
    RunOcclusion
  end
  
  
  
  %% Methods
  methods

    %% Class Constructor:
    %
    % @param      configFilename  The configuration filename
    % @param      numCubesats     Number of expected cubesats
    %
    % @return     A reference to an initialized CubeSat object
    %
    function obj = Optical(ModelRef, configFilename, numCubesats)
        import VANTAGE.PostProcessing.CubeSat_Optical
        obj.ModelRef = ModelRef;
        SensorData = jsondecode(fileread(configFilename));
        obj.CubeSats = cell(numCubesats+1,1);
        for i = 1:numCubesats
            obj.CubeSats{i} = CubeSat_Optical;
            obj.CubeSats{i}.tag = i;
            obj.CubeSats{i}.unitvec = [0,0];
        end
        obj.CubeSats{end} = CubeSat_Optical;
        obj.CubeSats{end}.tag = 'systemCentroid';
        obj.CubeSats{end}.centroid = [0,0];
        
        FrameIntervals = linspace(0,1,SensorData.DesiredFPS+1);
        
        obj.DesiredFPS = SensorData.DesiredFPS;
        obj.PlotBinarizedImages = SensorData.PlotBinarizedImages;
        obj.PlotCentroids = SensorData.PlotCentroids;
        obj.DataDirec = SensorData.OpticalData;
        obj.FrameIntervals = FrameIntervals;
        obj.FileExtension = SensorData.OpticalFileExtension;
        obj.PlotHist = SensorData.PlotHist;
        obj.PlotAny = SensorData.PlotAny;
        obj.PlotCubeSats = SensorData.PlotCubeSats;
        obj.RunOcclusion = SensorData.RunOcclusion;
        obj.CurrentFrameCount = 1;
        
    end

    %% Read data directory
    % Read and process input from directory of images
    %
    % @param      obj   The object
    %
    % @return     didRead   returns true if the file was read successfully,
    %             false otherwise
    %
    % @author     Justin Fay
    % @date       10-Mar-2019
    %
    function [didRead,direc,timing] = readInputFramesFromImages(obj)

        
        % Read data directory
        direc = dir(strcat(obj.DataDirec,obj.FileExtension));
        numFile = numel(direc);        
        
        didRead = false;
        if numFile ~= 0
            didRead = true;
        end
        
        % Convert filenames into seconds for ordered processing
        
        timing = zeros(numFile,1);
        for i = 1:numFile
            filename = direc(i).name;
            splitname = strsplit(filename,'_');
            day = str2num(splitname{2})*24*60*60;
            hour = str2num(splitname{3})*60*60;
            minute = str2num(splitname{4})*60;
            second = str2num(splitname{5});
            
            fileSuffix = strsplit(splitname{6},'.');
            milli = str2num(strcat(['.',fileSuffix{1}]));
            
            timing(i) = day+hour+minute+second+milli;
        end
        
        [~,indices] = sort(timing);
    end


    %% Perform optical processing
    % Process optical frames to find the estimated cubesat positions
    %
    %               Dylan Bossie
    % @date         17-Mar-2019
    function [UnitVecsVCF,UnitOriginVCF,timestamp,...
            isSystemCentroid] = OpticalProcessing(obj,image,BackgroundPixels)
        % Grab image
        frameTitle = image.name;
        frame = imread(strcat(obj.DataDirec,frameTitle));
        
        % Process current frame
        [obj,isSystemCentroid] = obj.ImageProcessing(frame,BackgroundPixels);
            if isSystemCentroid == 'invalid'
                UnitVecsVCF = [1 1 1];
                UnitOriginVCF = [0 0 0];
                timestamp = 1;
                return
            end
        
        % Transform results to VCF unit vectors
        UnitVecsVCF = obj.PixelToUnitVec(isSystemCentroid);
        UnitOriginVCF = [0 0 0];

        % Get frame timestamp
        timestamp = image.date;
        
        % Perform object association
        if isSystemCentroid == 1
        end
    end
    
    %% Perform 100m data cleanup
    % Take data from 100m test and clean regions with background noise not
    % relevant to the test
    %
    %               Dylan Bossie
    % @date         17-Mar-2019
    function [frame] = Cleanup100mData(obj,frame,background)
        for i = 1:length(frame(:,1))
            for j = 1:length(frame(1,:))
                if background(i,j) == 1
                    frame(i,j) = 0;
                end
            end
        end
    end
    
    %% Background Subtraction - Find background noise
    % Take data from test and clean regions with background noise not
    % relevant to the test
    % @param        frame            Current binarized frame
    %
    % @author       Dylan Bossie
    % @date         31-Mar-2019
    function [binarizedFrame] = FindBackground(obj,firstFrame)
        frameTitle = firstFrame.name;

        frame = imread(strcat(obj.DataDirec,frameTitle));
        
        % Binarize frame with low threshold to find noisy background
        % objects
        binarizedFrame = imbinarize(frame,0.01);
    end
    
    %% Perform image processing
    % Execute frame processing based on provided automated naming
    % convention
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function [obj,isSystemCentroid] = ImageProcessing(obj,frame,BackgroundPixels)
        I = frame;
        if size(I,3) > 1
            I_gray = rgb2gray(I);
        else
            I_gray = I;
        end
        
        % Remove background
        I_gray = obj.Cleanup100mData(frame,BackgroundPixels);
        
        % Adaptive Thresholding Binarization
        I_binarized = obj.Binarization(I_gray);
        if I_binarized == 0
            isSystemCentroid = 'invalid';
            return
        end
        
        %I_binarized_mean = imbinarize(I_gray_mean/255,graythresh(I_gray_mean/255));
        
        I_boundaries = bwboundaries(I_binarized);
        
        %Isolate boundaries corresponding to CubeSats (remove noise)
        CubeSat_Boundaries = obj.detectObjects(I_boundaries,I_binarized);
        
        try
            if ~isempty(CubeSat_Boundaries)
                numCubeSats = length(obj.CubeSats);
                
                if obj.RunOcclusion
                    % Occlusion cutting
                    CubeSat_Boundaries_Cut = cell(0);
                    CubeSat_polyshapes = cell(0);
                    n = 1;
                    for i = 1:numel(CubeSat_Boundaries)
                        cutPoly = obj.occlusionCut(CubeSat_Boundaries{i}(:,1),CubeSat_Boundaries{i}(:,2),obj.ModelRef.Deployer.GetNumCubesats());
                        for j = 1:numel(cutPoly)
                            CubeSat_Boundaries_Cut{n} = cutPoly{j}.Vertices;
                            CubeSat_polyshapes{n} = cutPoly{j};
                            n = n+1;
                        end
                    end
                end
                %
                %Find CubeSat centroids
                centroids = obj.findCentroids(CubeSat_Boundaries);

                if obj.is100mSearch
                    meanCent = zeros(1,2);
                    for i = 1:numel(centroids)
                        meanCent = meanCent + centroids{i};
                    end
                    meanCent = meanCent./numel(centroids);
                    obj.PixelLocation100m = meanCent;
                    isSystemCentroid = true;
                    return
                end

                % Check number of found centroids - 
                % If centroids found are the same as expected, do not use
                % system centroid - set to false. If different amount is
                % detected, set use system centroid to true
                if numel(centroids) == numCubeSats && ~obj.is100mSearch
                    % Use centroids of individual CubeSats
                    isSystemCentroid = false;

                    % Assign the system centroid for this image to [0 0]
                    obj.CubeSats{end}.centroid = [0 0];
                elseif ~obj.is100mSearch
                    % Use system centroid 
                    isSystemCentroid = true;
                    meanCent = zeros(1,2);
                    for i = 1:numel(centroids)
                        meanCent = meanCent + centroids{i};
                    end
                    meanCent = meanCent./numel(centroids);

                    %Assign the mean centroid to the system centroid CubeSat
                    obj.CubeSats{end}.centroid = meanCent;
                end

                if ~obj.is100mSearch && ~isSystemCentroid
                    % Perform object association
                    obj.objectAssociation(centroids);
                end

                %Plot results
                if obj.PlotBinarizedImages && obj.PlotAny && ~obj.is100mSearch && ~isSystemCentroid
                    obj.plotObjectBoundaries(I_gray,CubeSat_Boundaries,centroids,isSystemCentroid)
                elseif obj.PlotBinarizedImages && obj.PlotAny && ~obj.is100mSearch && isSystemCentroid
                    obj.plotObjectBoundaries(I_gray,CubeSat_Boundaries,meanCent,isSystemCentroid)
                end
            end
        catch
            isSystemCentroid = true;
            return

        end

    end
    
    %% Plot boundaries
    % Plot results from bwboundaries
    % @param        CubeSats         Contains a cell array of object
    %                                boundaries found in the image
    % @param        centerpoint      Pixel location of image center
    %
    % @param        centroids        Centroid locations of objects
    %                                found in image
    %
    % @author       Dylan Bossie
    % @date         26-Jan-2019
    function plotObjectBoundaries(obj,grayImage,boundaries,centroids,isSystemCentroid)
        figure
        imshow(grayImage)
        hold on
        if ~isSystemCentroid
            for i = 1:length(boundaries)
                %bwboundaries has an odd convention for placing X in col. 2 and
                %Y in col. 1
                X = boundaries{i}(:,2);
                Y = boundaries{i}(:,1);
                %Plot boundary for obj{i}
                plot(X,Y)
            end
            for i = 1:numel(centroids)
                %Plot centroids for obj{i}
                scatter(centroids{i}(1),centroids{i}(2),'r','+','LineWidth',30)
                text(centroids{i}(1)+centroids{i}(1)*.05,centroids{i}(2)+...
                    centroids{i}(2)*.05,['Calculated Centroid ' num2str(i)],'Color','r')
            end
        else
            scatter(centroids(1),centroids(2),'r','+','LineWidth',30)
            text(centroids(1)+centroids(1)*.05,centroids(2)+...
                    centroids(2)*.05,'Calculated System Centroid','Color','r')
        end
        saveas(gcf,[obj.DataDirec 'GrayscaleOut/' num2str(obj.CurrentFrameCount) '.jpg']);
    end
    
    
    %% Find Object Centroids
    % Use CubeSat boundaries to determine their respective centroid
    % locations in the image
    % @param        CubeSats        Boundaries of each CubeSat
    %
    % @author       Dylan Bossie
    % @date         26-Jan-2019
    function centroids = findCentroids(~,CubeSats)
        centroids = cell(length(CubeSats),1);
        for i = 1:length(CubeSats)
%             [y,x] = centroid(CubeSats{i});
            x = mean(CubeSats{i}(:,2));
            y = mean(CubeSats{i}(:,1));
            centroids{i} = [x,y];
        end
    end
    
    %% Object Association
    % After bodies and their respective centroids have been identified,
    % this function will associate them with relative identities based on
    % their launch order 1-6 (1: first launched, 6: last launched)
    % @param        centroids     Pixel location of centroid in image
    %
    % @param        centerpoint   Pixel location of image center
    %
    % @param        occlusion     Boolean array of tags descripting whether
    %                             or not the respective centroid contains 
    %                             occluded bodies
    %
    % @author       Dylan Bossie
    % @date         14-Feb-2019
    function obj = objectAssociation(obj,centroids)
        %Get distance from final pixel location for all objects
        distance = zeros(length(centroids),1);
        for i=1:length(centroids)
            distance(i) = norm(abs(obj.PixelLocation100m-centroids{i}));
        end

        %Relative positioning from the final pixel location should remain
        %ordered for the CubeSat launch positions. Therefore, centroids can
        %be associated based on their relative distance from the final
        %location.
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    end
    
    %% Detect 100m Pixel Location
    % Object association requires knowledge of the final pixel location of
    % the system at 100m. This method will analyze the last image and
    % detect this point
    % @param        centroids     Pixel location of centroid in image
    %
    % @param        centerpoint   Pixel location of image center
    %
    % @param        occlusion     Boolean array of tags descripting whether
    %                             or not the respective centroid contains 
    %                             occluded bodies
    %
    % @author       Dylan Bossie
    % @date         14-Feb-2019
    function obj = find100mPixel(obj,imageFile,BackgroundPixels)
         % Grab image
        frameTitle = imageFile.name;
        frame = imread(strcat(obj.DataDirec,frameTitle));
        
        % Process current frame
        obj.is100mSearch = true;
        [obj,~] = obj.ImageProcessing(frame,BackgroundPixels);
        
        % Disable 100m pixel search
        obj.is100mSearch = false;
    end
    
    %% Linear Extrapolation
    % For the case of an object being occluded, a linear extrapolation will
    % be performed based on its velocity calculated in the previous frames
    % to approximate its next centroid location
    % @param        centroids     Pixel location of centroid in image
    %
    % @param        centerpoint   Pixel location of image center
    %
    % @param        occlusion     Boolean array of tags descripting whether
    %                             or not the respective centroid contains 
    %                             occluded bodies
    %
    % @author       Dylan Bossie
    % @date         14-Feb-2019
    function [] = linearExtrapolation(obj,centroids,centerpoint,occlusion)
        %Get distance from centerpoint for all objects
        distance = zeros(length(centroids),1);
        for i=1:length(centroids)
            distance(i) = norm(abs(centerpoint-centroids{i}));
        end
        
        j = 1;
        for i=1:length(occlusion)
            if occlusion(i) == false
                obj.CubeSats{i}.centroid = centroids{j};
                j = j + 1;
            else
                % Current CubeSat is occluded
                obj.CubeSats{i}.centroid = [0 0];%obj.linearExtrapolation(obj.CubeSats{i})
            end
        end
    end
    
    %% Detect Objects
    % Remove noisy boundaries found in the image, leaving only distinct
    % boundaries corresponding to CubeSats
    % @param        I_boundaries     Contains a cell array of object
    %                                boundaries found in the image
    %
    % @author       Dylan Bossie
    % @date         26-Jan-2019
    function CubeSats = detectObjects(obj,I_boundaries,I_binarized)
        if isempty(I_boundaries)
            CubeSats = [];
            return
        end
        
        %Find number of pixels within each detected boundary for weighting
        %based on size
        objectSizes = ones(numel(I_boundaries),1);
        for i = 1:numel(I_boundaries)
            objectSizes(i) = bwarea(I_boundaries{i});
        end
        
        %Set minimum size an object must meet relative to largest object to
        %be considered for processing
        objectSizeThreshold = 0.05*max(objectSizes);
        for i = 1:numel(objectSizes)
            if objectSizes(i) < objectSizeThreshold
                objectSizes(i) = 0;
            end
        end
        
        %Find centroid locations of all boundaries detected
        boundaryCentroids = zeros(numel(I_boundaries),2);
        for i = 1:numel(I_boundaries)
            boundary = I_boundaries{i};
            boundaryCentroids(i,1) = mean(boundary(:,2));
            boundaryCentroids(i,2) = mean(boundary(:,1));
        end
        
        PixelLocationsX = boundaryCentroids(:,1);
        PixelLocationsY = boundaryCentroids(:,2);
        
        %Assign overall boundary centroid for X and Y based on their size
        %as a weight for a weighted mean
        weightedPixelLocX = sum(PixelLocationsX.*objectSizes)/sum(objectSizes);
        weightedPixelLocY = sum(PixelLocationsY.*objectSizes)/sum(objectSizes);
        
        CubeSats = [];
        for i = 1:numel(I_boundaries)
            if boundaryCentroids(i,1) > weightedPixelLocX...
                    && objectSizes(i) >= objectSizeThreshold...
                    && boundaryCentroids(i,2) < weightedPixelLocY...
                CubeSats = [CubeSats I_boundaries(i)];
            end
        end
        
        if isempty(CubeSats)
            potentialCubeSats = {};
            boundaryCentroids = [];
            for i = 1:numel(I_boundaries)
                if objectSizes(i) >= objectSizeThreshold
                    potentialCubeSats = [potentialCubeSats I_boundaries(i)];
                end
            end
            
            XPosition = zeros(length(boundaryCentroids),1);
            for i = 1:numel(potentialCubeSats)
                boundary = potentialCubeSats{i};
                X = mean(boundary(:,2));
                Y = mean(boundary(:,1));
                %boundaryCentroids = [boundaryCentroids {[X,Y]}];
                XPosition(i) = X;
            end
            
            [~,maxXIndex] = max(XPosition);
            rightmostCubeSat = potentialCubeSats(maxXIndex);
            CubeSats = rightmostCubeSat;
        end
        
        if obj.PlotCubeSats
            imshow(I_binarized)
            hold on
            for i = 1:length(CubeSats)
                plot(CubeSats{i}(:,2),CubeSats{i}(:,1))
            end
%               for i = 1:length(I_boundaries)
%                   if objectSizes(i) >= objectSizeThreshold
%                      plot(I_boundaries{i}(:,2),I_boundaries{i}(:,1))
%                      scatter(boundaryCentroids(i,1),boundaryCentroids(i,2))
%                   end
%               end
        end
    end

    %% obfuscation identification for cubesat boundaries
    %
    % This function takes boundary points found after binarization and separates
    % it into multiple cubesats that are obfuscated
    %
    % @param      x            x coordinates of boundary
    % @param      y            y coordinates of boundary
    % @param      posCase      The position case 'd' is diagonal, 'h' is
    %                          horizontal, 'v' is vertical
    % @param      numCubesats  The number cubesats
    %
    % @return     cell array of identified polyshapes
    %
    % @author     Justin Fay
    % @date       21-Feb-2019
    %
    function cutPoly = occlusionCut(obj,x,y,numCubesats)
        
        method1Success = true;

        %% concavity finding stuff        
        % find convex hull
        k = convhull(x,y);
        convHull_poly = polyshape(x(k),y(k));

        % calculate distance from boundary to convex hull
        [xMesh,yMesh] = meshgrid(1:max(x)+2,1:max(y)+2);
        xMesh = xMesh - 1;
        yMesh = yMesh - 1;
        convHullOuterBin = ~isinterior(convHull_poly,xMesh(:),yMesh(:));
        convHullOuterBin = reshape(convHullOuterBin,[max(y)+2,max(x)+2]);
        D = bwdist(convHullOuterBin);
        r = zeros(numel(x),1);
        for i = 1:numel(x)
            r(i) = D(y(i),x(i));
            if r(i) < 1
                r(i) = 0;
            end
        end

        % estimate number of concavity points
        windowSize = ceil(numel(r)*0.05);
        b = (1/windowSize)*ones(1,windowSize);
        a = 1;
        r_filter = filter(b,a,r);
        minProm = 0.1;
        [tmpPks,fitPks,~,tmpProm] = findpeaks(r_filter,'SortStr','descend');
        [tmpPks,fitPks,~,tmpProm] = findpeaks(r,'SortStr','descend',...
            'MinPeakProminence',max(tmpProm.*0.05),...
            'MinPeakWidth',0,...
            'MaxPeakWidth',Inf,...
            'MinPeakHeight',0,...
            'MinPeakDistance',numel(r_filter)*0.25);
        %fitPks = fitPks - round(windowSize./2);
        maxPks = numCubesats*2-2;
        if numel(tmpPks)>maxPks
            [~,diffI] = max(diff(sort(tmpProm)));
            expectedPks = numel(tmpProm)-diffI;
            if expectedPks > maxPks
                expectedPks = maxPks;
            end
        else
            expectedPks = numel(tmpPks);
        end
        expectedPks = expectedPks - mod(expectedPks,2);
        fitPks = fitPks(1:expectedPks);

        if expectedPks==0
            cutPoly{1} = polyshape(x,y);
            method1Success = false;
        end

        % filter method for finding concavity
        %{
        numFiltTrials = 20;
        windowPercent = linspace(1,10,numFiltTrials)'./100;
        pks = zeros(numFiltTrials,expectedPks);
        locs = zeros(numFiltTrials,expectedPks);
        numGoodTrials = numFiltTrials;
        for i = 1:numFiltTrials
            windowSize = ceil(numel(r)*windowPercent(i));
            b = (1/windowSize)*ones(1,windowSize);
            a = 1;

            r_filter = filter(b,a,r);
            [tmpPks,tmpLocs,tmpWidth,tmpProm] = findpeaks(r_filter,'SortStr','descend','NPeaks',expectedPks);
            if numel(tmpPks)<expectedPks
                numGoodTrials = i-1;
                break;
            end
            pks(i,1:numel(tmpPks)) = tmpPks;
            locs(i,1:numel(tmpLocs)) = tmpLocs;
            [locs(i,:),I] = sort(locs(i,:));
            pks(i,:) = pks(i,I);
        end
        locs = locs(1:numGoodTrials,:);
        windowPercent = windowPercent(1:numGoodTrials,:);

        % fit lines to filtered results to find concavity indices
        fitPks = zeros(expectedPks,1);
        for i = 1:expectedPks
            p = obj.ransacLine(windowPercent,locs(:,i),10);
            fitPks(i,1) = round(polyval(p,windowPercent(1)));
        end
        %}

        %% Associating Concavity Points
        if method1Success

            % Determine the position case for the launch
            oMat = zeros(8,1);
            oMat(obj.ModelRef.Deployer.GetVantageTube()) = 1;
            oMat(obj.ModelRef.Deployer.GetDeploymentTube()) = 1;
            oMat = reshape(oMat,[4,2])';

            if max(sum(oMat,1)) == 2
                posCase = 'v';
            elseif max(sum(oMat,2)) == 2
                posCase = 'h';
            else
                posCase = 'd';
            end
            posCase = lower(posCase);
            numSets = size(fitPks,1)/2;

            % Experimental method for associating points
            %{
            if numSets > 1
                tmp = nchoosek(1:numPts,numSets);
                tmp = [tmp,numPts-tmp];
                for i = 1:size(tmp,1)
                    intersectionPts = zeros(factorial(numSets),2);
                end
            end
            %}

            % Current working method for associating points
            if numSets > 1
                ptSets = zeros(numSets,2);
                switch posCase
                    case 'v' % vertical tube
                        % order x positions
                        [~,iOrder] = sort(x(fitPks));
                        fitPksOrder = fitPks(iOrder);

                        % find midline
                        midLine = mean(y(fitPks));

                        % find points greater than midline
                        gtMid = y(fitPksOrder) > midLine;

                        % store ordered connection points
                        ptSets(:,1) = fitPksOrder(~gtMid);
                        ptSets(:,2) = fitPksOrder(gtMid);    
                    case 'h' % horizontal tube
                        % order x positions
                        [~,iOrder] = sort(y(fitPks));
                        fitPksOrder = fitPks(iOrder);

                        % find midline
                        midLine = mean(x(fitPks));

                        % find points greater than midline
                        gtMid = x(fitPksOrder) > midLine;

                        % store ordered connection points
                        ptSets(:,1) = fitPksOrder(~gtMid);
                        ptSets(:,2) = fitPksOrder(gtMid);
                    case 'd' % diagonal tube

                        % extract coordinate points
                        xPks = x(fitPks);
                        yPks = y(fitPks);

                        % order points by x position
                        [xPks,iOrder] = sort(xPks);
                        yPks = yPks(iOrder);
                        fitPksOrder = fitPks(iOrder);

                        % fit line to points
                        midLine = polyfit(xPks,yPks,1);
                        midY = polyval(midLine,xPks);
                        gtMid = yPks > midY;

                        % store ordered connection points
                        ptSets(:,1) = fitPksOrder(~gtMid);
                        ptSets(:,2) = fitPksOrder(gtMid);
                end
            else
                ptSets = fitPks';
            end
            %{
            figure
            plot(frame_poly)
            hold on
            plot(x(fitPks),y(fitPks),'ro')
            for i = 1:numSets
                plot( [x(ptSets(i,1)), x(ptSets(i,2))] , [y(ptSets(i,1)), y(ptSets(i,2))], 'k-+')
            end
            %}

            %% Cutting into multiple polyshapes           
            numShapes = numSets+1;
            numPts = numel(x);
            cutPoly = cell(numShapes,1);

            % sort concavity point sets
            tmp = min(ptSets,[],2);
            [~,ptSort] = sort(tmp);
            ptSets = ptSets(ptSort,:);
            ptSets = [1,numPts;ptSets];

            % re-organize point sets for ease of use
            for i = 1:numShapes
                [~,tmpI] = min(ptSets(i,:));
                if tmpI == 2
                    ptSets(i,:) = fliplr(ptSets(i,:));
                end  
            end

            % identify shape boundary indices and create cut poyshapes
            for i = 1:numSets
                tmpPolyI = [ptSets(i,1):ptSets(i+1,1),ptSets(i+1,2):ptSets(i,2)];
                cutPoly{i} = polyshape(x(tmpPolyI),y(tmpPolyI));
            end
            tmpPolyI = ptSets(numShapes,1):ptSets(numShapes,2);
            cutPoly{numShapes} = polyshape(x(tmpPolyI),y(tmpPolyI));

            % remove cuts smaller than 5% max area
            cutArea = zeros(numShapes,1);
            for i = 1:numShapes
                cutArea(i) = area(cutPoly{i});
            end   
            I = cutArea./max(cutArea) < 0.05;
            cutPoly = cutPoly(~I);
        else

            %% image gradient method for obfuscation
            %{
            % pull frame
            inFrame = obj.Frame(min(x):max(x),min(y):max(y));
            
            % find gradient magnitude
            [Gmag,Gdir] = imgradient(inFrame);
            
            % adjust x from cropping
            tmpX = x - min(x);
            tmpY = y - min(y);
            polyTmp = polyshape(tmpY,tmpX);
            
            % ignore edge points
            polyBuff = polybuffer(polyTmp,-5);
            
            in = false(size(Gmag,1),size(Gmag,2));
            for i = 1:size(Gmag,1)
                    in(i,1:size(Gmag,2)) = isinterior(polyBuff,1:size(Gmag,2), i.*ones(1,size(Gmag,2)));
            end
            
            Gmag(~in) = 0;
            
            figure
            surf(Gmag)
            shading interp
        %}
        end

        % plotting if you want it
        %{
        figure
        for i = 1:numShapes
            plot(cutPoly{i})
            hold on
        end
        %}
    end
    
    % Convert pixel location to VCF unit vector
    %
    %
    % @param      CubeSats     List of CubeSat classes containing centroid
    %                          locations
    % @return     CubeSatUnitVectors    List of unit vectors in VCF for
    %                                   each centroid input
    %
    % @author     Dylan Bossie
    % @date       4-Mar-2019
    %
    function CubeSatUnitVectors = PixelToUnitVec(obj,isSystemCentroid)
        %Read optical camera parameters
        CameraParameters = jsondecode(fileread('./Config/Sensors.json'));
        focalLength = CameraParameters.OpticalFocalLength;
        pixelSize = CameraParameters.OpticalPixelSize;
        gridSize = CameraParameters.OpticalResolution;
        origin = [gridSize(1)/2 gridSize(2)/2];
        pixelSizeX = pixelSize;%m
        pixelSizeY = pixelSize;%m

        if ~isSystemCentroid
            numCubeSats = length(obj.CubeSats);
            CubeSatUnitVectors = cell(numCubeSats,1);
            for i = 1:numCubeSats
                Centroid = obj.CubeSats{i}.centroid;
                %Distance from origin in pixels
                p_x = Centroid(1) - origin(1);
                p_y = -(Centroid(2) - origin(2));
                f = -focalLength;

                x = p_x*pixelSizeX;
                y = p_y*pixelSizeY;
                S = [x y f];

                CubeSatUnitVectors{i} = S/norm(S);
            end
        else
            Centroid = obj.CubeSats{end}.centroid;
            %Distance from origin in pixels
            p_x = Centroid(1) - origin(1);
            p_y = -(Centroid(2) - origin(2));
            f = -focalLength;

            x = p_x*pixelSizeX;
            y = p_y*pixelSizeY;
            S = [x y f];

            CubeSatUnitVectors = S/norm(S);
        end
        
    end
    
    % Convert pixel location to VCF unit vector
    %
    %
    % @param      UnitVecs          List of unit vectors for each C.S. centroid
    % @return     PixelLocations    List of pixel locations for each C.S. centroid 
    %
    % @author     Dylan Bossie
    % @date       5-Mar-2019
    %
    function PixelLocations = UnitVecToPixels(~,UnitVecs)
        %Read optical camera parameters
        CameraParameters = jsondecode(fileread('./Config/Sensors.json'));
        focalLength = CameraParameters.OpticalFocalLength;
        pixelSize = CameraParameters.OpticalPixelSize;
        gridSize = CameraParameters.OpticalResolution;
        origin = [gridSize(1)/2 gridSize(2)/2];
        pixelSizeX = pixelSize;%m
        pixelSizeY = pixelSize;%m
        f = focalLength;

        numCubeSats = length(UnitVecs);
        PixelLocations = cell(numCubeSats,1);
        for i = 1:numCubeSats
            centroidVec = UnitVecs(i);

            x = centroidVec(0)/pixelSizeX;
            y = centroidVec(1)/pixelSizeY;

            PixelLocations{i} = [x y];
        end
    end
    
    % Convert pixel location to VCF unit vector
    %
    %
    % @param      I_gray            Grayscale image of current frame
    % @return     I_binarized       Adaptively binarized image
    %
    % @author     Dylan Bossie
    % @date       16-Mar-2019
    %
    function I_binarized = Binarization(obj,I_gray)
        try
            baseThreshold = 0.17;
            I_basebinarized = imbinarize(I_gray,baseThreshold);

            %Update I_gray with [0 0 0] for values below base threshold
            for i = 1:length(I_gray(:,1))
                for j = 1:length(I_gray(1,:))
                    if I_basebinarized(i,j) == 0
                        I_gray(i,j) = 0;
                    end
                end
            end

            debugging = true;
            if debugging == true
                I_binarized = I_basebinarized;
                return
            end
            %Find distribution of visual magnitude remaining in I_gray
            visualMagnitudes = I_gray(I_gray~=0);
            bins = 80;
            [histValues,histEdges,~] = histcounts(visualMagnitudes,bins);
            histValues = sgolayfilt(histValues,2,obj.roundToNearestOdd(bins/10));
            if obj.PlotHist
                figure
                plot(histEdges(1:end-1),histValues)
            end

            [~,maxIndex] = max(histValues);

            %Set adaptive threshold based 10% of the maximum bin count
            %(brightest part of CubeSat, peak of image)
            adaptiveThreshold = histEdges(floor(maxIndex/10))/255;

            %Binarize image again using the new adapative threshold
            I_binarized = imbinarize(I_gray,adaptiveThreshold);
        catch
            warning('Binarization did not detect any objects, skipping')
            I_binarized = 0;
            return
        end
    end
end
    
    
    
    
    
  %% Private methods
  methods (Access = private)
    %% ransac line fitting for obfuscation
    %
    % This function uses the ransac algorithm to fit a line to noisy data
    %
    % @param      x            x coordinates to fit
    % @param      y            y coordinates to fit
    % @param      maxDistance  The maximum distance from the line
    %
    % @return     polyfit line
    %
    % @author       Justin Fay
    % @date         21-Feb-2019
    function p = ransacLine(obj,x,y,maxDistance)
        points = [x,y];
        sampleSize = 2; % number of points to sample per trial

        fitLineFcn = @(points) polyfit(points(:,1),points(:,2),1); % fit function using polyfit
        evalLineFcn = ...   % distance evaluation function
          @(model, points) sum((points(:, 2) - polyval(model, points(:,1))).^2,2);

        [~, inlierIdx] = ransac(points,fitLineFcn,evalLineFcn, ...
          sampleSize,maxDistance);
      
        p = polyfit(points(inlierIdx,1),points(inlierIdx,2),1);
    end
    
    % 
        % Round to nearest odd integer
        %
        % @param    x   number to be rounded
        % 
        % @return   nearest odd integer
        %
        % @author   Joshua Kirby
        % @date     06-Mar-2019
        function y = roundToNearestOdd(obj,x)
            y = 2*round(x/2) + obj.binarySign(x-round(x));
        end
        
        % 
        % Return sign without zero, for zero return 1
        %  
        % @param    num     number whose sign is to be determined
        %
        % @return   binary sign
        %
        % @author   Joshua Kirby
        % @date     06-Mar-2019
        function bSign = binarySign(~,num)
            bSign = sign(num);
            if ~bSign
                bSign = 1;
            end
        end
  end
  
  
  
  
  
  
end