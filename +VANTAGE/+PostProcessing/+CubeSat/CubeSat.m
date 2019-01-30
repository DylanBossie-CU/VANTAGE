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

        % occlusion tracking (integer)
        occlusion

        % expected cubesat size in U
        expectedU

        %% Calculated Properties
        %
        % point cloud comprising cubesat
        pc
        
        % number of identified visible planes
        numVisiblePlanes
        
        % guess at U inferred from geometry
        inferredU
        
        % point clouds comprising identified planes/faces, cell
        pc_face
        
        % struct containing information about each plane/face
        face
        
        % inferred plane/face U
        faceU
        
        %% True Properties
        %
        % true U if known (here for aggregation/debugging)
        trueU
        
        % true centroid if known (here for aggregation/debugging)
        trueCentroid_TCF
        
        
    end
    
    %% Methods
    methods
        % Class Constructor:
        %
        % @param      name        The name
        % @param      rangeOrder  The range order
        % @param      pos_init    The initial position
        % @param      occlusion   The occlusion
        % @param      expectedU   The expected U
        %
        % @return     A reference to an initialized Cubesat object
        %
        function obj = Cubesat(name, rangeOrder, pos_init, occlusion, expectedU)
            obj.name = name;
            obj.rangeOrder = rangeOrder;
            obj.pos_init = pos_init;
            obj.occlusion = occlusion;
            obj.expectedU = expectedU;
        end
    end
    
    
    
end