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
    % Container for the input video
    Video
    
    % General description of the input video
    VideoType
    
    % Current frame of input video
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
    
    % Video file
    VideoFilename
    % Plotting option (binarized images)
    PlotBinarizedImages
    
    PlotCentroids
  end
  
  
  
  %% Methods
  methods
    %% Read input video file
    % Execute video frame processing based on provided automated naming
    % convention
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function [obj,didRead] = readInputFrame(obj)
        didRead = false;
        frame = readFrame(obj.Video);
        %Grab data in intervals of the desired FPS
        FrameTimeStep = obj.Video.CurrentTime - ...
            floor(obj.Video.CurrentTime);
        if any(FrameTimeStep==obj.FrameIntervals)
            didRead = true;
            obj.Frame = frame;
            %Process image
            image = obj.ImageProcessing(frame);
            obj.Image = image;
        end
    end
    
    %% Perform image processing
    % Execute video frame processing based on provided automated naming
    % convention
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function [I_binarized_mean] = ImageProcessing(obj,frame)
        I = frame;
        
        centerpoint = [ceil(obj.width/2),ceil(obj.height/2)];
        I_gray = rgb2gray(I);
        
        %%
        I_gray_d=double(I_gray);
        
        maxI=max(max(I_gray_d));
        
        I_gray_norm=I_gray_d * (255/maxI);
        minI=min(min(I_gray_norm));
        
        I_gray_std=I_gray_norm - minI;
        
        % I_gray_gmean=meanFilter(I_gray_std);
        
        I_gray_mean=imgaussfilt(I_gray_std,3);
        
        
        %% memoized values
        %adapt_Tresh: table of frame distributions saved for training test
        %background: background pixel values to remove background
        %idx=I_gray_mean>background;
        % adapt_Thresh=prctile(I_gray_mean(idx),saved_threshhold(distance));
        % I_binarized_mean=imbinarize(I_gray_mean/255,adapt_Thresh/255);
        
        %% adaptive
        
        %I_binarized = imbinarize(I_gray_norm,binaryTolerance);
        %I_binarized_norm = imbinarize(I_gray_norm,binaryTolerance);
        %I_binarized_std = imbinarize(I_gray_std,binaryTolerance);
        
        I_binarized_mean = imbinarize(I_gray_mean/255,graythresh(I_gray_mean/255));
        imshow(I_binarized_mean)
        pause(0.5)
        
        I_boundaries = bwboundaries(I_binarized_mean);
        
        
        %Isolate boundaries corresponding to CubeSats (remove noise)
        CubeSat_Boundaries = obj.detectObjects(I_boundaries);
        
        % Occlusion cutting
        for i = 1:numel(I_boundaries)
            cutPoly = obj.occlusionCut(I_boundaries{i}(:,1),I_boundaries{i}(:,2),'h',6);
            %{
            figure
            for i = 1:numel(cutPoly)
                plot(cutPoly{i})
                hold on
            end
            pause(0.5)
            close all
            %}
        end
        %{
        %Find CubeSat centroids
        centroids = obj.findCentroids(CubeSat_Boundaries);
        
        %Perform object association
        occlusion = [false,false];
        obj.objectAssociation(centroids,centerpoint,occlusion)
        
        if obj.PlotBinarizedImages
            close all
            imshow(I_binarized);
            title('Binarized Frame');
            hold on
            obj.plotObjectBoundaries(CubeSat_Boundaries,centerpoint,centroids)
            
            saveas(gcf,...
                ['OpticalImageOutputs/' obj.VideoType ...
                num2str(obj.CurrentFrameCount) '.jpg'])
        end
        
        %Transform centroid locations for output to sensor fusion
        %}
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
    function plotObjectBoundaries(obj,CubeSats,~,centroids)
        for i = 1:length(CubeSats)
            %bwboundaries has an odd convention for placing X in col. 2 and
            %Y in col. 1
            X = CubeSats{i}(:,2);
            Y = CubeSats{i}(:,1);
            plot(X,Y)
            
            if obj.PlotCentroids
                scatter(centroids{i}(1),centroids{i}(2),'r','+','LineWidth',30)
                text(centroids{i}(1)+centroids{i}(1)*.05,centroids{i}(2)+...
                    centroids{i}(2)*.05,'Calculated Centroid','Color','r')
            end
        end
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
            centroids{i} = zeros(1,2);
            centroids{i}(1) = mean(CubeSats{i}(:,2));
            centroids{i}(2) = mean(CubeSats{i}(:,1));
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
    function obj = objectAssociation(obj,centroids,centerpoint,occlusion)
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
    function CubeSats = detectObjects(obj,I_boundaries)
        CubeSats = [];
        if isempty(I_boundaries)
            return
        end
        objectSizes = ones(length(I_boundaries),1);
        for i = 1:length(I_boundaries)
            objectSizes(i) = bwarea(I_boundaries{i});
        end
        %Set minimum size an object must meet relative to largest object to
        %be considered for processing
        objectSizeThreshold = 0.1*max(objectSizes);
       
        for i = 1:length(objectSizes)
            if objectSizes(i) >= objectSizeThreshold
                CubeSats = [CubeSats I_boundaries(i)];
            end
        end
    end
    
    %% Set desired initial properties of the class
    %
    % Records transform matrices and translation vectors between specific
    % frames in TDATA
    %
    % @param        DesiredFPS      Sets timesteps for video frames to be 
    %                               processed (FPS)
    % @param        PlotBinarizedImages     Set if user desires for plots
    %                               to be generated
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function obj = setOpticalData(obj,DesiredFPS,PlotBinarizedImages,...
            PlotCentroids,VideoType,FrameIntervals,amount)
        import VANTAGE.PostProcessing.CubeSat_Optical
        obj.CubeSats = cell(amount,1);
        for i = 1:amount
            obj.CubeSats{i} = CubeSat_Optical;
            obj.CubeSats{i}.tag = i;
            obj.CubeSats{i}.centroid = [0,0];
        end
        
        obj.DesiredFPS = DesiredFPS;
        obj.PlotBinarizedImages = PlotBinarizedImages;
        obj.PlotCentroids = PlotCentroids;
        obj.VideoType = VideoType;
        obj.FrameIntervals = FrameIntervals;
    end
    
    % Class Constructor:
    %
    % @param      configFilename  The configuration filename
    % @param      numCubesats     Number of expected cubesats
    %
    % @return     A reference to an initialized CubeSat object
    %
    function obj = Optical(configFilename, numCubesats)
        import VANTAGE.PostProcessing.CubeSat_Optical
        SensorData = jsondecode(fileread(configFilename));
        obj.CubeSats = cell(numCubesats,1);
        for i = 1:numCubesats
            obj.CubeSats{i} = CubeSat_Optical;
            obj.CubeSats{i}.tag = i;
            obj.CubeSats{i}.centroid = [0,0];
        end
        filename = strcat(SensorData.OpticalData,...
            SensorData.OpticalVideoInput);
        FrameIntervals = linspace(0,1,SensorData.DesiredFPS+1);
        
        obj.DesiredFPS = SensorData.DesiredFPS;
        obj.PlotBinarizedImages = SensorData.PlotBinarizedImages;
        obj.PlotCentroids = SensorData.PlotCentroids;
        obj.VideoFilename = filename;
        obj.FrameIntervals = FrameIntervals;
        obj.VideoType = SensorData.OpticalVideoInput;
        if exist(obj.VideoFilename,'file')
            obj.Video = VideoReader(obj.VideoFilename);
        end
    end

    % obfuscation identification for cubesat boundaries
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
    function cutPoly = occlusionCut(obj,x,y,posCase,numCubesats)

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
        end

        % estimate number of concavity points
        windowSize = ceil(numel(r)*0.1);
        b = (1/windowSize)*ones(1,windowSize);
        a = 1;
        r_filter = filter(b,a,r);
        minProm = 0.01;
        [tmpPks,~,tmpWidth,tmpProm] = findpeaks(r_filter,'SortStr','descend','MinPeakProminence',minProm,'MaxPeakWidth',5);
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

        if expectedPks==0
            cutPoly{1} = polyshape(x,y);
            return;
        end

        % filter method for finding concavity
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

        %% Associating Concavity Points
        posCase = lower(posCase);
        numSets = size(fitPks,1)/2;

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
        
        cutArea = zeros(numShapes,1);
        for i = 1:numShapes
            cutArea(i) = area(cutPoly{i});
        end
        
        I = cutArea(i)./max(cutArea) < 0.05;
        cutPoly = cutPoly{~I};

        % plotting if you want it
        %
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
    function CubeSatUnitVectors = PixelToUnitVec(~,CubeSats)
        %Read optical camera parameters
        CameraParameters = jsondecode(fileread('./Config/Sensors.json'));
        focalLength = CameraParameters.OpticalFocalLength;
        pixelSize = CameraParameters.OpticalPixelSize;
        gridSize = CameraParameters.OpticalResolution;
        origin = [gridSize(1)/2 gridSize(2)/2];
        pixelSizeX = pixelSize;%m
        pixelSizeY = pixelSize;%m

        numCubeSats = length(CubeSats);
        CubeSatUnitVectors = cell(numCubeSats,1);
        for i = 1:numCubeSats
            centroid = CubeSats{i}.centroid;
            %Distance from origin in pixels
            p_x = centroid(1) - origin(1);
            p_y = -(centroid(2) - origin(2));
            f = -focalLength;

            x = p_x*pixelSizeX;
            y = p_y*pixelSizeY;
            S = [x y f];

            CubeSatUnitVectors{i} = S/norm(S);
        end
    end
end
    
    
    
    
    
  %% Private methods
  methods (Access = private)
    %% Index into nomen
    %
    % Random example of a private method
    %
    % @param        string to be found in obj.nomen
    %
    % @return       index of string in obj.nomen, set to 0 if string not
    %               found
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function [row] = findNomen(obj,str)

    end

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
  end
  
  
  
  
  
  
end