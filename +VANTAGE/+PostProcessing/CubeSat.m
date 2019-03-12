classdef CubeSat
    %% Classdef
    % A class for holding persistent CubeSats and their states over time
    %
    %% Properties
    properties
        %% Pre-defined Properties
        %
        % string containing cubesat name
        name

        % expected range ordered position (integer)
        rangeOrder

        % initial position vector in VCF
        pos_init

        % expected cubesat size in U
        expectedU

        % dimensions of cubesat in meters
        actualDims

        %% Calculated Properties
        %
        % all centroids over time nx3
        centroids_VCF
        
        % time vector, length-n
        time
        
        % boolean vector for whether each centroid point is an outlier,
        % length-n
        isOutlier
        
        
        %% True Properties
        %
        
        
        
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
        function obj = CubeSat(name, rangeOrder, pos_init, expectedU, actualDims)
            if nargin == 5
                obj.name = name;
                obj.rangeOrder = rangeOrder;
                obj.pos_init = pos_init;
                obj.expectedU = expectedU;
                obj.actualDims = actualDims;
            end
        end
    end
    
    
    
end