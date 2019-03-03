classdef CubeSat_TOF
    %% Classdef
    % A class representing disposable CubeSats with data relevant to TOF
    % processing
    % @author   Joshua Kirby
    % @date 28-Feb-2019
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
        
        % Lots of geometric stuff...a better method for this should
        % be developed
        Avec
        Lvec
        u_short
        u_long
        Dvec
        D_short
        D_long

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
        % @return     A reference to an initialized Cubesat_TOF object
        %
        function obj = CubeSat_TOF()
            
            % JMK HEY YOU, ALL THIS GEOMETRY SUCKS AND SHOULD BE TURNED INTO
            % SOMETHING MUCH MORE INTELLIGENT
            % (U)nit length
            obj.u_short = 0.1; % m
            obj.u_long  = 0.1125; % m
            % U
            U = [1 2 3 4 5 6];
            % Side Lengths
            L_short = obj.u_short;
            L_long  = obj.u_long .*U;
            obj.Lvec = [L_long L_short];
            % Face Areas
            A_short = obj.u_short^2;
            % Note this is a hack, A_long should use obj.u_long but this works better for
            % now, should use better heuristics so this hack is not necessary
            A_long  = U.*obj.u_short^2;
            obj.Avec = [A_long A_short];
            % Face Diagonals
            obj.D_short = sqrt(obj.u_short^2 + L_short^2);
            obj.D_long  = sqrt(obj.u_short^2 + L_long.^2);
            obj.Dvec = [obj.D_long obj.D_short];
        end
    end
    
    
    
end