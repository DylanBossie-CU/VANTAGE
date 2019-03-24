classdef CubeSat
    %% Classdef
    % A class for holding persistent CubeSats and their states over time
    %
    %% Properties
    properties (Access = public)
        %%% Pre-defined Properties
        %
        % string containing cubesat name
        name

        % expected range ordered position (integer)
        rangeOrder

        % expected cubesat size in U
        expectedU

        % dimensions of cubesat in meters
        actualDims

        %%% Calculated Properties
        %
        % all centroids over time 3xn
        centroids_VCF
        
        % time vector, length-n
        time
        
        % number of points (time(i), centroids_VCF(:,i))
        numTOFpoints
        
        % Final 3D model fit to the TOF centroids, used for propagation
        TOFfit
        
    end
    
    %% Methods
    methods
        % Class Constructor:
        %
        % @param      name        The name
        % @param      rangeOrder  The range order
        % @param      occlusion   CubeSat occluded? 0,1
        % @param      expectedU   The expected U
        %
        % @return     A reference to an initialized CubeSat object
        %
        function obj = CubeSat(name, rangeOrder, expectedU, actualDims)
            
            TOFfit{1,3} = cfit;
            
            if nargin == 4
                obj.name = name;
                obj.rangeOrder = rangeOrder;
                obj.expectedU = expectedU;
                obj.actualDims = actualDims;
            end
        end
    end
    
    
    
end