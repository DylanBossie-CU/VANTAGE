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
        % all centroids over time nx3
        centroids_VCF
        
        % time vector, length-n
        time
        
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
            if nargin == 4
                obj.name = name;
                obj.rangeOrder = rangeOrder;
                obj.expectedU = expectedU;
                obj.actualDims = actualDims;
            end
        end
    end
    
    
    
end