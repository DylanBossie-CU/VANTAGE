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
    function [I_binarized] = ImageProcessing(obj,frame)
        I = frame;
        centerpoint = [ceil(obj.width/2),ceil(obj.height/2)];
        I_gray = rgb2gray(I);
        binaryTolerance = 0.1;
        I_binarized = imbinarize(I_gray,binaryTolerance);
        I_boundaries = bwboundaries(I_binarized);
        
        %Isolate boundaries corresponding to CubeSats (remove noise)
        CubeSat_Boundaries = obj.detectObjects(I_boundaries);
        
        %Find CubeSat centroids
        centroids = obj.findCentroids(CubeSat_Boundaries);
        
        %Perform object association
        %%%%% Fake occlusion results
        occlusion = [true,false];
        %%%%%
        obj = obj.objectAssociation(centroids,centerpoint,occlusion);
        
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
        
        CubeSats = [];
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
  end
  
  
  
  
  
  
end