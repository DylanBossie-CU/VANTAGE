classdef TOF
    %% Classdef
    % A class representing the Time of Flight Camera used by VANTAGE
    %
    %% Properties
    properties
        %
        % Current number of visible CubeSats, very mutable
        currNumSatsVisible
        
    end
        
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
        % @param    filename    filename, string
        % @param    SensorData  struct of information about sensors
        %
        % @return	3xn matrix of n CubeSat centroids in the TOF
        %        	Cartestian frame, m
        function [Deployer] = naiveFindCentroids(obj,filename,Deployer,...
                SensorData)
            % Extract point cloud locations
            filename = strcat(SensorData.TOFData,filename.name);
            % Extract CubeSats from Deployer
            CubeSats = Deployer.CubesatArray;
            
            % Load point cloud from file
            warning('Load script is currently only designed to work with simulated .pcd files')
            pc = obj.loadSimFile(filename);
            
            % Separate point cloud into identified cubesats
            CubeSats = cubesatPointsFromPC(pc);
            
            for i = 1:length(CubeSats)
                % Identify visible planes for each cubesat
                CubeSats(i) = fitPlanesToCubesat(CubeSats(i));

                % Calculate centroids by projecting from identified planes
                CubeSats(i) = findCentroidFromFaces(CubeSats(i));
            end
            
            % Repackage CubeSats into Deployer
            Deployer.CubesatArray = CubeSats;
            
        end
        
    end
    
    %% Private Methods
    methods % (Access = private)
        %% Loading point clouds from files
        %
        % Loads data from a simulation file
        %
        % @param    filename	filename, string
        % 
        % @return	point cloud obtained from file
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
        % @param	filename    filename, string
        %
        % @return	point cloud obtained from file
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
        % @param    pc          raw point cloud from file
        % @param    CubeSats    array of initialized CubeSat objects
        %                       @see CubeSat
        %
        % @return	vector of identified cubesats (TOF.CubeSat class)
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
        function [obj,CubeSats] = cubesatPointsFromPC(obj,pc,CubeSats)
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

            locs = flip(locs);
            nSplit = numel(locs);

            % Separate point cloud by split planes
            if nSplit>0
                for i = 1:nSplit
                    if i==1
                        I = pc.Location(:,3)>=locs(i);
                        CubeSats(i).pc = pointCloud(pc.Location(I,:));
                    else
                        I = pc.Location(:,3)>=locs(i);
                        CubeSats(i).pc = pointCloud(pc.Location(I,:));
                        I = CubeSats(i).pc.Location(:,3)<locs(i-1);
                        CubeSats(i).pc = pointCloud(CubeSats(i).pc.Location(I,:));
                    end
                end
                I = pc.Location(:,3)<locs(nSplit);
                CubeSats(nSplit+1).pc = pointCloud(pc.Location(I,:));
            else
                CubeSats.pc = pc;
            end
        
        % Save number of visible CubeSats
        obj.currNumSatsVisible = nSplit + 1;
        
        end
        
        %% Identifying visible planes for a cubesat
        %
        % Identifies visible faces in a cubesat point cloud
        %
        % @param    CubeSat     CubeSat object to be analyzed
        %                       @see CubeSat
        %
        % @return   CubeSat     CubeSat object with identified planes
        %                       populated
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
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
                % Take minPoints to be one-twentieth the number of points found in the first
                % plane
                if numPlanes == 1
                    minPtsInPlane = round(planes(numPlanes).planeCloud.Count/20);
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
        
        %% Calculating centroids from identified planes for a cubesat
        %
        % Calculates centroids in TCF for a cubesat based on identified faces
        %
        % @param    CubeSat     CubeSat object to be analyzed
        %                       @see CubeSat
        % 
        % @return   CubeSat     CubeSat object with calculated centroid
        %                       populated
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
        function [CubeSat] = findCentroidFromFaces(obj,CubeSat)
            % Find centroid based on number of identified faces
            if CubeSat.numVisibleFaces==1
                CubeSat = obj.centroid1(CubeSat);
            elseif CubeSat.numVisibleFaces==2
                CubeSat = obj.centroid2(CubeSat);
            elseif CubeSat.numVisibleFaces==3
                CubeSat = obj.centroid3(CubeSat);
            end
        end
        
        %% Subroutines
        %
        % Computes the plane that fits best (least square of the normal
        % distance to the plane) a set of sample points.
        %
        % @param    X   a N by 3 matrix where each line is a sample point
        %
        % @return 	a unit(column) vector normal to the plane
        % @return  	a 3 by 2 matrix, the columns of V form an
        %           orthonormal basis of the plane
        % @return  	a point belonging to the plane
        %
        % @author   Adrien Leygue
        % @date     30-Aug-2013
        function [n,V,p] = affine_fit(obj,X)
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
        
        %
        % Bla
        %
        % @param   
        %
        % @return  
        %
        % @author   Joshua Kirby
        % @date     07-Feb-2019
        function [d] = distInFromFace(obj,CubeSat,face)
            
            % Calculate standard areas
            Avec = CubeSat.Avec;
            
            % Calculate face area
            A = area(face);
            
            % Calculate difference between area and standard areas
            dA = abs(A-Avec);
            
            % Choose nearest size
            [~,I] = min(dA);
            
            % Set distance in for centroid
            dVec = 0.5.*CubeSat.Lvec;
            if I==7
                d = dVec(CubeSat.expectedU);
            else
                d = dVec(7);
            end
        end
        
        %
        % Makes a unit vector
        %
        % @param    vec     A vector
        % 
        % @return   the norm of vec
        %
        % @author   Joshua Kirby
        % @date     07-Feb-2019
        function [uvec] = unitvec(obj,vec)
            uvec = vec./norm(vec);
        end
        
        %
        % Calculates the CubeSat centroid from a single identified face by
        % projecting inward from the face to the centroid.  Assumes that
        % the expectedU of the CubeSat is the actual U
        %
        % @param    CubeSat     CubeSat object to be analyzed
        %                       @see CubeSat
        %
        % @return   CubeSat     CubeSat object with calculated TCF centroid
        %                       populated
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
        function CubeSat = centroid1(obj,CubeSat)
            %%% Data extract
            %error('unfinished, working with distances')
            planes = CubeSat.faces;
            
            %%% Project points onto 2d plane
            plane = planes(1).planeCloud;
            inPlane = double( (plane.Location-planes(1).o)*planes(1).V );
            
            %%% Find plane boundary points
            I_bound = boundary(inPlane(:,1),inPlane(:,2));
            
            %%% Find preliminary centroid of boundary
            face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
            [x,y] = centroid(face);
            
            %%% Find corners of face
            r = zeros(size(face.Vertices,1),1);
            theta = zeros(size(r));
            for i = 1:size(face.Vertices,1)
                r(i)     = norm(face.Vertices(i,:)-[x y]);
                theta(i) = atan2d(face.Vertices(i,2)-y,face.Vertices(i,1)-x);
            end
            [~,I] = sort(theta);
            r = r(I);
            face.Vertices(:,1) = face.Vertices(I,1);
            face.Vertices(:,2) = face.Vertices(I,2);
            % locs identify the corner indices in face.Vertices
            %tmp = smooth(abs(gradient(r,theta)));
            [~,locs] = findpeaks(padarray(r,10),'NPeaks',4,'sortstr','descend');
            locs = locs - 10;
            
            %%% Identify centroid, algorithm varies based on number of corners found
            warning(['THIS NEEDS TO BE CHANGED TO USE THE IDENTIFIED U OF THE CUBESAT IN',...
                     ' ORDER TO ACCOUNT FOR ERRORS FROM TOF DATA NOT COVERING FULL SIDE OF',...
                     ' CUBESAT'])
            switch length(locs)
                % Four Corners
                case 4
                    % Centroid is simple average
                    tmp = mean(face.Vertices(locs,:),1);
                    x = tmp(1);
                    y = tmp(2);
                    % Three Corners
                case 3
                    foundOffDiagonal = 0;
                    % Loop through points
                    for i = 1:3
                        vc = 1;
                        % produce vectors from current point to other two points
                        for j = [1:i-1 i+1:3]
                            vec{vc} = face.Vertices(locs(j),:)-face.Vertices(locs(i),:);
                            vc = vc + 1;
                        end
                        % if angle between those vectors is within 10 deg of 90 deg, the
                        % current index (i) corresponds to the off-diagonal point
                        if abs(acosd(dot(vec{1},vec{2})/(norm(vec{1})*norm(vec{2})))-90) < 10
                            foundOffDiagonal = 1;
                            break
                        end
                    end
                    % Error if no off diagonal was found
                    if ~foundOffDiagonal
                        error('Off diagonal corner could not be found when findpeaks returns three corners, catching point could be method or 10 degree orthogonality threshold')
                    end
                    % centroid is mean of on-diagonal points
                    tmp = mean(face.Vertices(locs([1:i-1 i+1:3]),:),1);
                    x = tmp(1);
                    y = tmp(2);
                    
                    % Two Corners
                case 2
                    error('findpeaks only identifed two corners in the cubesat face, add the ability to handle this')
                    
                    % One corner
                case 1
                    error('findpeaks only identified one corner in the cubesat face, create the ability to handle this?')
                    
                    % No corners
                case 0
                    error('findpeaks could not identify any corners in the cubesat face')
                    
                    % Some other number
                otherwise
                    error('findpeaks identified an unhandled number of corners in the cubesat face, time to investigate...')
            end
            
            %%% Convert plane centroid to 3d point
            outPlane = planes(1).o + (x*planes(1).V(:,1))' + (y*planes(1).V(:,2))';
            
            %%% Project inward to volumetric centroid
            d = obj.distInFromFace(CubeSat,face);
            CubeSat.centroid_TCF = outPlane + d.*(sign(dot(outPlane,planes(1).n))*planes(1).n)';
            
        end
        
        %
        % Calculates the CubeSat centroid from two identified faces by
        % projecting inward from the faces to the centroid.  Assumes that
        % the expectedU of the CubeSat is the actual U
        %
        % @param    CubeSat     CubeSat object to be analyzed
        %                       @see CubeSat
        %
        % @return   CubeSat     CubeSat object with calculated TCF centroid
        %                       populated
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
        function CubeSat = centroid2(obj,CubeSat)
            %%% Extract data
            planes = CubeSat.faces;
            
            %%% Not currently used, but does check if planes intersect as expected
            for i = 1:2
                M(i,1:3) = planes(i).n;
                c(i,1)   = planes(i).n(1)*planes(i).o(1) + planes(i).n(2)*planes(i).o(2) + planes(i).n(3)*planes(i).o(3);
            end
            
            if rank(M) ~= 2
                error('The two planes identified do not intersect as a line')
            end
            
            %%% Determine parameters corresponding to the plane intersect
            % unit vector parallel to intersection
            V = cross(planes(1).n,planes(2).n);
            
            % project point cloud onto this unit vector direction
            pc_alongIntersect = [planes(1).planeCloud.Location;planes(2).planeCloud.Location]*V;
            
            % Determine length of intersection
            lengthIntersect   = max(pc_alongIntersect)-min(pc_alongIntersect);
            
            % Determine mean value along the intersection direction
            mean_alongIntersect = 0.5*(max(pc_alongIntersect)+min(pc_alongIntersect));
            
            %%% Locate mean point along intersect
            [min_alongIntersect,I]  = min(pc_alongIntersect);
            tmp    = [planes(1).planeCloud.Location;planes(2).planeCloud.Location];
            if I <= planes(1).planeCloud.Count % if point is in plane 1
                t = (dot(planes(2).n,planes(2).o)-dot(planes(2).n,tmp(I,:)))/norm(planes(2).n)^2;
                minpt = tmp(I,:)' + t*planes(2).n;
            else % if point is in plane 2
                t = (dot(planes(1).n,planes(1).o)-dot(planes(1).n,tmp(I,:)))/norm(planes(1).n)^2;
                minpt = tmp(I,:)' + t*planes(1).n;
            end
            meanpt = minpt + (mean_alongIntersect-min_alongIntersect)*V;
            
            %%% Determine best-guess face size and calculate centroid
            % ensure that unitvectors are inward facing
            n(:,1) = sign(dot(meanpt,planes(1).n))*planes(1).n;
            n(:,2) = sign(dot(meanpt,planes(2).n))*planes(2).n;
            switch CubeSat.expectedU
                % 1U Cubesat
                case 0
                    % mid-plane diagonal unit vector
                    innerDiag = (n(:,1)+n(:,2))./norm(n(:,1)+n(:,2));
                    % centroid (project along innerDiag by half a diagonal face length)
                    pos       = meanpt + 0.5*CubeSat.D_short*innerDiag;
                case {1,2,3,4,5,6}
                    % Make best guess at which face is which
                    As = CubeSat.Avec;
                    for i = 1:2
                        inPlane = double( (planes(i).planeCloud.Location-planes(i).o)*planes(i).V );
                        I_bound = boundary(inPlane(:,1),inPlane(:,2));
                        face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
                        A = area(face);
                        [dAmin(i),J(i)] = min(abs(As-A));
                    end
                    [~,I] = min(dAmin);
                    bestI = I; % this index is the plane corresonding to bestSize
                    offI = [1 2];
                    offI = offI(offI~=bestI); % this index is the other plane
                    bestSize = J(bestI); % this value says whether bestI is a 1U, 2U, or 3U length side
                    if bestSize == 1 && CubeSat.expectedU ~= 1
                        bestSize = 7;
                    end
                    switch bestSize
                        case {1,2,3,4,5,6}
                            [~,offSize]  = min(abs(CubeSat.Lvec-lengthIntersect)); % this value says the U of the off-size
                        case 7
                            offSize = u;
                    end
                    % mid-plane diagonal unit vector
                    innerDiag = obj.unitvec(CubeSat.Lvec(offSize)*n(:,bestI) + CubeSat.Lvec(bestSize)*n(:,offI));
                    % centroid (project along innerDiag by half a diagonal face length)
                    CubeSat.centroid_TCF = meanpt + 0.5*sqrt((CubeSat.Lvec(offSize))^2+(CubeSat.Lvec(bestSize))^2)*innerDiag;
                otherwise
                    error('Invalid u has been given as an input')
            end
        end
        
        %
        % Calculates the CubeSat centroid from three identified faces by
        % projecting inward from the faces to the centroid.  Assumes that
        % the expectedU of the CubeSat is the actual U
        %
        % @param    CubeSat     CubeSat object to be analyzed
        %                       @see CubeSat
        %
        % @return   CubeSat     CubeSat object with calculated TCF centroid
        %                       populated
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
        function CubeSat = centroid3(obj,CubeSat)
            %%% Extract data
            planes = CubeSat.faces;
            
            %%% Find plane intersection
            for i = 1:3
                A(i,1:3) = planes(i).n;
                b(i,1)   = planes(i).n(1)*planes(i).o(1) + planes(i).n(2)*planes(i).o(2) + planes(i).n(3)*planes(i).o(3);
            end
            
            if rank(A) ~= 3
                error('Planes do not intersect in the expected way')
            end
            
            ptIntersect = (A\b);
            
            %%% Find Best-Guess Face
            % ensure that unitvectors are inward facing
            for i = 1:3
                n(:,i) = sign(dot(ptIntersect,planes(i).n))*planes(i).n;
            end
            % Make best guess for which face is which
            As = CubeSat.Avec;
            for i = 1:3
                inPlane = double( (planes(i).planeCloud.Location-planes(i).o)*planes(i).V );
                I_bound = boundary(inPlane(:,1),inPlane(:,2));
                face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
                A = area(face);
                [dAmin(i),J(i)] = min(abs(As-A));
            end
            [~,I] = min(dAmin);
            bestI = I; % this index is the plane corresonding to bestSize
            bestSize = J(bestI); % this value says what U is bestI
            
            %%% Determine other face sizes based on intersect length with best-guess face
            c = 1;
            for i = [1:I-1 I+1:3]
                % unit vector parallel to intersection
                V = cross(planes(bestI).n,planes(i).n);
                % project point cloud onto this unit vector direction
                pc_alongIntersect = [planes(bestI).planeCloud.Location;planes(i).planeCloud.Location]*V;
                % Determine length of intersection
                lengthIntersect   = max(pc_alongIntersect)-min(pc_alongIntersect);
                
                % Make best guess at which face this is
                switch bestSize
                    case {1,7}
                        offI(c)    = i;
                        offSize(c) = u;
                    case u
                        offI(c)        = i;
                        [~,offSize(c)] = min(abs(CubeSat.Lvec-lengthIntersect));
                    otherwise
                        error('Best-guess for face size does not match an allowable size (1 or U)')
                end
                c = c + 1;
            end
            
            %%% Find volumetric centroid
            % volumetric diagonal
            switch bestSize
                case {1,7}
                    innerDiag = unitvec(n(:,bestI)*CubeSat.u_long*u + n(:,offI(1))*CubeSat.Lvec(bestSize) + n(:,offI(2))*CubeSat.Lvec(bestSize));
                case u
                    innerDiag = unitvec(n(:,bestI)*CubeSat.Lvec(7) + n(:,offI(1))*CubeSat.Lvec(offSize(2)) + n(:,offI(2))*CubeSat.Lvec(offSize(1)));
                otherwise
                    error('Best-guess for face size does not match an allowable size (1 or U)')
            end
            CubeSat.centroid_TCF = ptIntersect + 0.5*sqrt((u*CubeSat.u_long)^2+2*(CubeSat.u_short)^2)*innerDiag;
            
        end
        
    end
    
    
    
end % classdef