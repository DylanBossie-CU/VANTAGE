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
    
    % Daat directory
    DataDirec
    
    % Plotting option (binarized images)
    PlotBinarizedImages
    
    PlotCentroids

    % Model handle class
    ModelRef

    % Data filenames
    FileExtension
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
        obj.DataDirec = SensorData.OpticalData;
        obj.FrameIntervals = FrameIntervals;
        obj.FileExtension = SensorData.OpticalFileExtension;
    end

    %% Read data directory
    % Read and process input from directory of images Execute image frame processing from
    % directory of images
    %
    % @param      obj   The object
    %
    % @return     didRead   returns true if the file was read successfully,
    %             false otherwise
    %
    % @author     Justin Fay
    % @date       10-Mar-2019
    %
    function [obj,didRead] = processInputFramesFromImages(obj)
        
        % Initialize read status
        didRead = false;
        
        % Read data directory
        direc = dir(obj.DataDirec);
        numFile = numel(direc);
        
        % Exclude files that aren't data
        tmp = false(numFile,1);
        for i = 1:numFile
            tmp(i) = contains(direc(i).name,'.jpg');
        end
        direc = direc(tmp);
        
        % Check read status
        numFile = numel(direc);
        if numFile<0
            didRead = false;
            return;
        end
        
        % Process frames
        for i = 1:numFile
            obj.Frame = imread(strcat(obj.DataDirec,'/',direc(i).name));           
            image = obj.ImageProcessing(obj.Frame);
            obj.Image = image;
        end
    end

    %% Read input video file
    % Execute video frame processing based on provided automated naming
    % convention
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function [obj,didRead] = readInputFrameFromVideo(obj)
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
    function [I_binarized,centroids,CubeSat_Boundaries_Cut] = ImageProcessing(obj,frame)
        I = frame;
        
        centerpoint = [ceil(length(frame(:,1)/2)),ceil(length(frame(1,:))/2)];
        if size(I,3) > 1
            I_gray = rgb2gray(I);
        else
            I_gray = I;
        end
        
        %%
        %I_gray_d=double(I_gray);
        
        %maxI=max(max(I_gray_d));
        
        %I_gray_norm=I_gray_d * (255/maxI);
        %minI=min(min(I_gray_norm));
        
        %I_gray_std=I_gray_norm - minI;
        
        % I_gray_gmean=meanFilter(I_gray_std);
        
        %I_gray_mean=imgaussfilt(I_gray_std,1);
        %Placeholder - remove Gaussian filter
        %I_gray_mean = I_gray_std;
        
        %% memoized values
        %adapt_Tresh: table of frame distributions saved for training test
        %background: background pixel values to remove background
        %idx=I_gray_mean>background;
        % adapt_Thresh=prctile(I_gray_mean(idx),saved_threshhold(distance));
        % I_binarized_mean=imbinarize(I_gray_mean/255,adapt_Thresh/255);
        
        
        %I_binarized = imbinarize(I_gray_norm,binaryTolerance);
        %I_binarized_norm = imbinarize(I_gray_norm,binaryTolerance);
        %I_binarized_std = imbinarize(I_gray_std,binaryTolerance);
        
        % Adaptive Thresholding Binarization
        I_binarized = obj.Binarization(I_gray);
        
        
        %I_binarized_mean = imbinarize(I_gray_mean/255,graythresh(I_gray_mean/255));
        
        I_boundaries = bwboundaries(I_binarized);
        
        %Isolate boundaries corresponding to CubeSats (remove noise)
        CubeSat_Boundaries = obj.detectObjects(I_boundaries);
        
        if ~isempty(CubeSat_Boundaries)
            numCubeSats = length(obj.CubeSats);
            
            % Occlusion cutting
            CubeSat_Boundaries_Cut = cell(0);
            CubeSat_polyshapes = cell(0);
            n = 1;
            for i = 1:numel(CubeSat_Boundaries)
                cutPoly = obj.occlusionCut(CubeSat_Boundaries{i}(:,1),CubeSat_Boundaries{i}(:,2),'h',6);
                for j = 1:numel(cutPoly)
                    CubeSat_Boundaries_Cut{n} = cutPoly{j}.Vertices;
                    CubeSat_polyshapes{n} = cutPoly{j};
                    n = n+1;
                end
            end
            
            % Check for occlusion
            numOccluded = numCubeSats-length(CubeSat_Boundaries_Cut);

            %
            %Find CubeSat centroids
            centroids = obj.findCentroids(CubeSat_polyshapes);
            
            % Perform object association
            obj.objectAssociation(centroids,centerpoint,numOccluded);

            centroids = obj.findCentroids(CubeSat_polyshapes);
            
            %Plot results
            if obj.PlotBinarizedImages
                obj.plotObjectBoundaries(I_gray,CubeSat_Boundaries,centroids)
            end

        end
        
        %Transform centroid locations for output to sensor fusion
        % To be input soon :) - method exists
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
    function plotObjectBoundaries(~,grayImage,boundaries,centroids)
        figure
        imshow(grayImage)
        hold on
        for i = 1:length(boundaries)
            %bwboundaries has an odd convention for placing X in col. 2 and
            %Y in col. 1
            X = boundaries{i}(:,2);
            Y = boundaries{i}(:,1);
            %Plot boundary for obj{i}
            plot(X,Y)
            
            %Plot centroids for obj{i}
            scatter(centroids{i}(1),centroids{i}(2),'r','+','LineWidth',30)
            text(centroids{i}(1)+centroids{i}(1)*.05,centroids{i}(2)+...
                centroids{i}(2)*.05,'Calculated Centroid','Color','r')
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
            [y,x] = centroid(CubeSats{i});
            centroids{i} = [x,y];
            %{
            centroids{i} = zeros(1,2);
            centroids{i}(1) = mean(CubeSats{i}(:,2));
            centroids{i}(2) = mean(CubeSats{i}(:,1));
            %}
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
        objectSizes = ones(numel(I_boundaries),1);
        for i = 1:numel(I_boundaries)
            objectSizes(i) = bwarea(I_boundaries{i});
        end
        %Set minimum size an object must meet relative to largest object to
        %be considered for processing
        objectSizeThreshold = 0.1*max(objectSizes);
       
        for i = 1:numel(objectSizes)
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
        %{
        tmp = polyshape(x,y);
        for i = 1:size(xMesh,1)
            i
            [I,J] = isinterior(tmp,[xMesh(i,:)',yMesh(i,:)']);
            D(i,:) = D(i,:).*J';
        end
        %}
        r = zeros(numel(x),1);
        for i = 1:numel(x)
            r(i) = D(y(i),x(i));
            if r(i) <= 3
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
    function I_binarized = Binarization(~,I_gray)
        baseThreshold = 0.05;
        I_basebinarized = imbinarize(I_gray,baseThreshold);
        
        %Update I_gray with [0 0 0] for values below base threshold
        for i = 1:length(I_gray(:,1))
            for j = 1:length(I_gray(1,:))
                if I_basebinarized(i,j) == 0
                    I_gray(i,j) = 0;
                end
            end
        end
        
        %Find distribution of visual magnitude remaining in I_gray
        visualMagnitudes = I_gray(I_gray~=0);
        [histValues,histEdges,~] = histcounts(visualMagnitudes,80);
        [~,maxIndex] = max(histValues);
        
        %Set adaptive threshold based 10% of the maximum bin count
        %(brightest part of CubeSat, peak of image)
        adaptiveThreshold = histEdges(floor(maxIndex/10))/255;
        
        %Binarize image again using the new adapative threshold
        I_binarized = imbinarize(I_gray,adaptiveThreshold);
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
  end
  
  
  
  
  
  
end