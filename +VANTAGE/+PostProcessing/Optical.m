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
    
    % Current frame of input video
    CurrentFrameCount
    
    % Desired FPS
    DesiredFPS
    
    % Current frame
    Frame
    
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
        if mod(obj.Video.CurrentTime,obj.DesiredFPS) == 0
            didRead = true;
            obj.Frame = frame;
            %Process image
            image = obj.ImageProcessing(frame);
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
        binaryTolerance = 0.3;
        I_binarized = imbinarize(I_gray,binaryTolerance);
        I_boundaries = bwboundaries(I_binarized);
        
        %Isolate boundaries corresponding to CubeSats (remove noise)
        CubeSats = obj.detectObjects(I_boundaries);
        
        %Find CubeSat centroids
        centroids = obj.findCentroids(CubeSats);
        if obj.PlotBinarizedImages
            imshow(I_binarized);
            title('Binarized Frame');
            hold on
            obj.plotObjectBoundaries(CubeSats,centerpoint,centroids)
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
    function plotObjectBoundaries(obj,CubeSats,centerpoint,centroids)
        if obj.PlotCentroids
            scatter(centerpoint(1),centerpoint(2),'g','x','LineWidth',30)
            text(centerpoint(1)+centerpoint(1)*.05,centerpoint(2)+...
                centerpoint(2)*.05,'Truth Centroid','Color','g')
        end
        for i = 1:length(CubeSats)
            %bwboundaries has an odd convention for placing X in col. 2 and
            %Y in col. 1
            X = CubeSats{i}(:,2);
            Y = CubeSats{i}(:,1);
            plot(X,Y)
            
            if obj.PlotCentroids
                scatter(centroids(i,1),centroids(i,2),'r','+','LineWidth',30)
                text(centroids(i,1)+centroids(i,1)*.05,centroids(i,2)+...
                    centroids(i,2)*.05,'Calculated Centroid','Color','r')
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
    function centroids = findCentroids(obj,CubeSats)
        centroids = [];
        for i = 1:length(CubeSats)
            centroids = [centroids mean(CubeSats{i}(:,2)),...
                mean(CubeSats{i}(:,1))];
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
        objectSizeThreshold = 0.8*max(objectSizes);
        
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
            PlotCentroids)
        obj.DesiredFPS = DesiredFPS;
        obj.PlotBinarizedImages = PlotBinarizedImages;
        obj.PlotCentroids = PlotCentroids;
    end
    
end
    
    
    
    
    
  %% Private methods
  methods (Access = private)
    %% Index into nomen
    %
    % Return row number of string location in obj.nomen
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