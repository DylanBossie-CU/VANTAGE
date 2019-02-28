classdef CubeSat_Optical
    %% Classdef
    % A class representing disposable CubeSats with data relevant to
    % Optical processing
    % @author       Dylan Bossie
    % @date         14-Feb-2019
    %% Properties
    properties
        % CubeSat identity (1-6, 1: first out, 6: last out)
        tag
        
        % Centroid pixel location
        centroid
    end
    
    %% Methods
    methods
        % Class Constructor:
        %
        % @param      name        The name
        % @param      rangeOrder  The range order
        % @param      pos_init    The initial position
        % @param      occlusion   CubeSat occluded? 0,1
        % @param      expectedU   The expected U
        %
        % @return     A reference to an initialized CubeSat object
        %
        function obj = CubeSat_Optical(tag, centroid)
            if nargin == 3
                obj.tag = tag;
                obj.centroid = centroid;
            end
        end
    end
    
    
    
end