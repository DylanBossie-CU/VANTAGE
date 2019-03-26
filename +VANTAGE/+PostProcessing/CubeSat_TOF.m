classdef CubeSat_TOF
    %% Classdef
    % A class representing disposable CubeSats with data relevant to TOF
    % processing
    % @author   Joshua Kirby
    % @date 28-Feb-2019
    %
    %% Properties
    properties
        %%% Pre-defined Properties
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

        %%% Calculated Properties
        %
        % point cloud comprising cubesat
        pc
        
        % number of identified visible planes
        numVisibleFaces
        
        % guess at U inferred from geometry
        inferredU
        
        % struct containing information about each plane/face
        faces = struct('planeCloud',[],...
                       'n',[],...
                       'V',[],...
                       'o',[],...
                       'faceCentr',[],...
                       'corners',[],...
                       'fullFace',[],...
                       'trustedLen',[],...
                       'trustedCorners',[],...
                       'faceIndex',[]);
        
        % inferred plane/face U
        faceU
        
        % calculated centroid in TCF
        centroid_TCF
        
        %%% True Properties
        %
        % true U if known (here for aggregation/debugging)
        trueU
        
        % true centroid if known (here for aggregation/debugging)
        trueCentroid_TCF

        % occlusion tracking (integer)
        occlusion
        
        %%% Other properties
        %
        % timestamp of current point cloud
        time
        
        % says whether faces are good enough to be trusted, defaults to
        % true
        trustedSat = 1
        
        
    end
    
    %% Methods
    methods
        % Class Constructor:
        %
        % @return     A reference to an initialized Cubesat_TOF object
        %
        function obj = CubeSat_TOF(CubeSat)
            % Error checking
            if ~isa(CubeSat,'VANTAGE.PostProcessing.CubeSat')
                error('Input to CubeSat_TOF constructor must be a VANTAGE.PostProcessing.CubeSat')
            end
            % The following properties are used prospectively and not taken
            % as true since CubeSat_TOF will not necessarily contain the
            % same point cloud as the CubeSat with which it is initialized
            obj.name = CubeSat.name;
            obj.expectedU = CubeSat.expectedU;
            obj.actualDims = CubeSat.actualDims;
            obj.rangeOrder = CubeSat.rangeOrder;
        end
    end
    
    
    
end