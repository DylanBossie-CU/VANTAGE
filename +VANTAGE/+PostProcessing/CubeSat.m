classdef CubeSat
    %% Classdef
    % A class representing cubesats found in TOF images, for use only in
    % the TOF class
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

        %% Calculated Properties
        %
        % point cloud comprising cubesat
        pc
        
        % number of identified visible planes
        numVisibleFaces
        
        % guess at U inferred from geometry
        inferredU
        
        % struct containing information about each plane/face
        faces
        
        % inferred plane/face U
        faceU
        
        % calculated centroid in TCF
        centroid_TCF
        
        %% True Properties
        %
        % true U if known (here for aggregation/debugging)
        trueU
        
        % true centroid if known (here for aggregation/debugging)
        trueCentroid_TCF

        % occlusion tracking (integer)
        occlusion
        
        
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
        function obj = CubeSat(name, rangeOrder, pos_init, expectedU)
            if nargin == 4
                obj.name = name;
                obj.rangeOrder = rangeOrder;
                obj.pos_init = pos_init;
                obj.expectedU = expectedU;
            end
        end
    end
    
    
    
end