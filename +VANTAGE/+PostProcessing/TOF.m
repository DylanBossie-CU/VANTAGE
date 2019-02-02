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
        
        % Maximum allowed distance from a plane to be considered part of the plane
        ptMaxDistFromPlane = 0.001;
        
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
            
            for i = 1:length(CubeSats)
                % Identify visible planes for each cubesat
                error('unimplemented')

                % Calculate centroids by projecting from identified planes
                error('unimplemented')
            end
            
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
                    CubeSats(i) = VANTAGE.PostProcessing.CubeSat;
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
        function [CubeSat] = fitPlanesToCubesat(obj,CubeSat)
            %%% Allocation
            % Initial value for minimum number of points considered to make up a plane
            minPtsInPlane   = 10;

            % Initialize planes structure
            numPlanes = 0;
            planes = struct;
            
            % Tracking of points not associated with any plane
            remainPtCloud = CubeSat.pc;

            %%% Find planes until no more planes exist
            while remainPtCloud.Count > minPtsInPlane
                %%% Fit a plane to the ptCloud
                warning('off','vision:pointcloud:notEnoughInliers')
                [~,inlierIndices,outlierIndices] = pcfitplane(remainPtCloud,obj.ptMaxDistFromPlane);
                warning('on','vision:pointcloud:notEnoughInliers')
                
                %%% Quit now if no significant planes found
                if isempty(inlierIndices) || numel(inlierIndices) < minPtsInPlane
                    break
                else
                    numPlanes = numPlanes + 1; % add a plane if one was found
                end
                
                %%% Extract plane from ptCloud
                % extract
                plane = select(remainPtCloud,inlierIndices);
                % store in output
                planes(numPlanes).planeCloud = plane;
                
                %%% Extract remaining points from ptCloud
                remainPtCloud = select(remainPtCloud,outlierIndices);
                
                %%% Obtain planar basis
                [planes(numPlanes).n,planes(numPlanes).V,planes(numPlanes).o] = obj.affine_fit(planes(numPlanes).planeCloud.Location);
                
                %%% Remove if repeat plane (e.g. if this is the cubesat 'feet')
                % For all previous planes
                for i = 1:numPlanes-1
                    % if normal is within 45 deg of prior plane's normal or antinormal
                    if acosd(dot(planes(numPlanes).n,planes(i).n)) < 45 || acosd(dot(planes(numPlanes).n,planes(i).n)) > 135
                        % if this plane is smaller than previous plane
                        if planes(numPlanes).planeCloud.Count <= planes(i).planeCloud.Count
                            % delete this plane
                            planes(numPlanes) = [];
                            numPlanes = numPlanes -1;
                            % if the previous plane is smaller than this plane
                        else
                            % replace previous plane with this plane and delete this plane
                            planes(i) = planes(numPlanes);
                            planes(numPlanes) = [];
                        end
                    end
                end
                
                %%% Update minimum number points required to make a plane
                % Take minPoints to be one-tenth the number of points found in the first
                % plane
                if numPlanes == 1
                    minPtsInPlane = round(planes(numPlanes).planeCloud.Count/10);
                end
            end

            %%% Check that no more than three planes were identified
            if numPlanes > 3
              error('fitPlanesToCubesat identified more than three planes for a single cubesat')
            end

            %%% Check that planes are mutually orthogonal
            for i = 1:numPlanes
              for j = 1:numPlanes
                if abs(asind(dot(planes(i).n,planes(j).n))) > 2 && i~=j
                  warning('fitPlanesToCubesat identified planes which are not mutually orthogonal by greater than 2 deg')
                end
              end
            end
            
            %%% Package planes and numPlanes into CubeSat object
            CubeSat.faces = planes;
            CubeSat.numVisibleFaces = numPlanes;

        end
        
        %% Calculating centroids from identified planes for each cubesat
        %
        
        
        %% Subroutines
        %
        function [n,V,p] = affine_fit(obj,X)
            %Computes the plane that fits best (lest square of the normal distance
            %to the plane) a set of sample points.
            %INPUTS:
            %
            %X: a N by 3 matrix where each line is a sample point
            %
            %OUTPUTS:
            %
            %n : a unit (column) vector normal to the plane
            %V : a 3 by 2 matrix. The columns of V form an orthonormal basis of the
            %plane
            %p : a point belonging to the plane
            %
            %NB: this code actually works in any dimension (2,3,4,...)
            %Author: Adrien Leygue
            %Date: August 30 2013
            
            %the mean of the samples belongs to the plane
            p = mean(X,1);
            
            %The samples are reduced:
            R = bsxfun(@minus,X,p);
            %Computation of the principal directions if the samples cloud
            [V,~] = eig(R'*R);
            %Extract the output from the eigenvectors
            n = V(:,1);
            V = V(:,2:end);
        end
        
    end
    
    
    
end % classdef