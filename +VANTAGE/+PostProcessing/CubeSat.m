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
        TOFfit_VCF
        
        % Range over which TOF centroiding was used
        TOFrange
        
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
        function this = CubeSat(name, rangeOrder, expectedU, actualDims)
            
            TOFfit_VCF{1,3} = cfit;
            
            if nargin == 4
                this.name = name;
                this.rangeOrder = rangeOrder;
                this.expectedU = expectedU;
                this.actualDims = actualDims;
            end
        end
        
        %
        % Evaluate TOFfit_VCF at some time
        %
        % @param    t   time at which to evaluate fit
        %
        % @return   TOF predicted centroid in VCF
        %
        % @author   Joshua Kirby
        % @date     06-Apr-2019
        function centroid_TofPredicted_VCF = evalTofFit(this,t)
            centroid_TofPredicted_VCF = cellfun(@(x) feval(x,t),this.TOFfit_VCF)';
        end
    end
    
    
    
end