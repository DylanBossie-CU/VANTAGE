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
    methods % (Access = private)
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
        function pc = loadSimFile(obj,filename)
           
            ptCloud = pcread(filename);
            
            if sum(sum(sum(~isnan(ptCloud.Location))))==0
                pc = struct('Count',0);
            else
                % Filter extraneous points
                pts = reshape(ptCloud.Location,ptCloud.Count,3);
                I = logical(prod(~isnan(pts),2));
                pc = pointCloud([pts(I,1:2),-pts(I,3)]);
            end
            
            
            figure
            pcshow(pc)
            hold on
            plot3(0,0,0,'k*')
            grid on
            axis equal
            
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
        % @return       vector of identified cubesats (TOF.CubeSat class)
        function CubeSats = cubesatPointsFromPC(obj,pc)
            % Pull out z values
            z = sort(pc.Location(:,3));

            % Start the loop
            NPeaks = Inf;
            bw = 0.005;
            counter = 0;
            while NPeaks > 3
              % Calculate k-squares density
              [zDense,zBin] = ksdensity(z,'bandwidth',bw,'function','pdf');

              % Identify split locations
              c = 0;
              [~,locs] = findpeaks(-zDense,zBin,'MinPeakProminence',c);

              % Update bandwidth
              NPeaks = length(locs);
              bw = length(locs)/3*bw;

              % give up if too many tries
              counter = counter + 1;
              if counter > 10
                error('Bandwidth for point splitting ksdensity function did not converge in 10 tries, implement a better bw update')
              end
            end

            %       figure
            %       plot(zBin,-zDense);
            %       xlabel('z (m)')
            %       ylabel('Percent point density')

            nSplit = numel(locs);

            % Separate point cloud by split planes
            if nSplit>0
                for i = 1:nSplit
                    CubeSats(i) = VANTAGE.PostProcessing.CubeSat.CubeSat;
                    if i==1
                        I = pc.Location(:,3)<=locs(i);
                        CubeSats(i).pc = pointCloud(pc.Location(I,:));
                    else
                        I = pc.Location(:,3)<=locs(i);
                        CubeSats(i).pc = pointCloud(pc.Location(I,:));
                        I = CubeSats(i).pc.Location(:,3)>locs(i-1);
                        CubeSats(i).pc = pointCloud(CubeSats(i).pc.Location(I,:));
                    end
                end
                I = pc.Location(:,3)>locs(nSplit);
                CubeSats(nSplit+1).pc = pointCloud(pc.Location(I,:));
            else
                CubeSats = CubeSat;
                CubeSats.pc = pc;
            end

            % Reverse order so CubeSats are ordered first-out to last-out
            CubeSats = flip(CubeSats);
        end
        %% Identifying visible planes for each cubesat
        %
        
        %% Calculating centroids from identified planes for each cubesat
        %
        
    end
    
    
    
end % classdef