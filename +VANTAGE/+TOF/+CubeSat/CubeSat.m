classdef CubeSat
    %% Classdef
    % A class representing cubesats found in TOF images, for use only in
    % the TOF class
    %
    %% Properties
    properties
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
        
    end
    
    
    
end