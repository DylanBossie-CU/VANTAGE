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
    CurrentFrame
    
    % Desired FPS
    DesiredFPS
  end
  
  
  
  %% Methods
  methods
    %% Read input video file
    % Execute video frame processing based on provided automated naming
    % convention
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function [output] = readInputFrame(obj)
        frame = readFrame(obj.Video);
        %Grab data in intervals of the desired FPS
        if mod(obj.Video.CurrentTime,obj.DesiredFPS) == 0
            %Process image
            image = obj.ImageProcessing(frame);
            %{
            distanceErrorFrame = FindCentroid_Video(frame,...
                plotGrayscale,1,plotBinarized,1,1,i);
            distanceError = [distanceError distanceErrorFrame];
            %imshow(frame);
            %}
        end
    end
    
    %% Perform image processing
    % Execute video frame processing based on provided automated naming
    % convention
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function [image] = ImageProcessing(obj,frame)
        image = frame;
        imshow(image)
    end
    
    %% Set desired initial properties of the class
    %
    % Records transform matrices and translation vectors between specific
    % frames in TDATA
    %
    % @param        Sets timesteps for video frames to be processed (FPS)
    %
    % @author       Dylan Bossie
    % @date         24-Jan-2019
    function obj = setOpticalData(obj,DesiredFPS)
        obj.DesiredFPS = DesiredFPS;
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