classdef TOF
    %% Classdef
    % A class representing the Time of Flight Camera used by VANTAGE
    %
    %% Properties
    properties (Access = private)
        %
        % Maximum range within which TOF data is considered valid, m
        maxAllowableRange = 8
        
        % The transform class used for frame changes
        % @see Transform
        Transform
        
    end
    
    
    %% Methods
    methods
        %% Getters
        %
        % Returns the maxAllowableRange
        function [maxAllowableRange] = getMaxAllowableRange(obj)
            maxAllowableRange = obj.maxAllowableRange;
        end
        
        %% Calculate centroids from file
        %
        % Produces centroids of all CubeSats naively identified in a single
        % point cloud
        %
        % @param        filename, string
        %
        % @return       3xn matrix of n CubeSat centroids in the TOF
        %               Cartestian frame, m
        function [centroids] = naiveFindCentroids(filename)
            % Load point cloud from file
            error('unimplemented')
            
            % Separate point cloud into identified cubesats
            error('unimplemented')
            
            % Identify visible planes for each cubesat
            error('unimplemented')
            
            % Calculate centroids by projecting from identified planes
            error('unimplemented')
            
        end
        
        
        
    end
    
    %% Private Methods
    methods (Access = private)
        %% Loading point clouds from files
        %
        % Loads data from a simulation file
        %
        % @param        filename, string
        % 
        % @return       point cloud obtained from file
        %
        % @author       Joshua Kirby
        % @date         24-Jan-2019
        function pc = loadSimFile(filename)
            error('unimplemented')
            % LEGACY CODE
            %{
            function ptNew = loadSimToF(filename)
            ptCloud = pcread(filename);
            
            if sum(sum(sum(~isnan(ptCloud.Location))))==0
                ptNew = struct('Count',0);
            else
                % Filter extraneous points
                pts = reshape(ptCloud.Location,ptCloud.Count,3);
                I = logical(prod(~isnan(pts),2));
                ptNew = pointCloud([pts(I,1:2),-pts(I,3)]);
            end
            
            
            figure
            pcshow(ptNew)
            hold on
            plot3(0,0,0,'k*')
            grid on
            axis equal
            %}
            
        end
        
        %
        % Loads experimental data from a VANTAGE TOF file
        %
        % @param        filename, string
        %
        % @return       point cloud obtained from file
        %
        % @author       Joshua Kirby
        % @date         24-Jan-2019
        function pc = loadExpData(filename)
            error('unimplemented')
        end
        
        %% Identifying cubesats within point clouds
        %
        % Separates a raw point cloud into the points associated with
        % distinct cubesats
        %
        % @param        raw point cloud from file
        %
        % @return       
        
        %% Identifying visible planes for each cubesat
        %
        
        %% Calculating centroids from identified planes for each cubesat
        %
        
    end
    
    
    
end % classdef