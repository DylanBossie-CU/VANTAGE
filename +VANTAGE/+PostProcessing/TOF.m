classdef TOF
    %% Classdef
    % A class representing the Time of Flight Camera used by VANTAGE
    %
    %% Properties       
    properties (Access = private)
        %
        % Maximum range within which TOF data is considered valid, m
        maxAllowableRange
        
        % The transform class used for frame changes
        % @see Transform
        Transform
        
        % Maximum allowed distance from a plane to be considered part of the plane
        ptMaxDistFromPlane
        
        % Truth data struct
        Truth_VCF
        
        % Face length matching tolerance, for use in centroid1 method
        faceLenMatchTol = 0.02; % m
        
    end
    
    %% Methods
    methods
        %% Constructor
        %
        % @param      configFilename    The configuration filename
        % @param      Model             Model class instance (@see Model)
        %
        % @return     A reference to an initialized TOF object
        %
        function obj = TOF(configFileneame,Model)
            % Read data from configuration file
            configData = jsondecode(fileread(configFileneame));

            % Initialize confuiguration parameters
            obj.maxAllowableRange = configData.maxAllowableRange;
            obj.ptMaxDistFromPlane = configData.ptMaxDistFromPlane;
            
            % Obtain truth data from Model
            if ~isa(Model,'VANTAGE.PostProcessing.Model')
                error('Model input to TOF class object constructor must be a VANTAGE.PostProcessing.Model instance')
            end
            obj.Truth_VCF = Model.Truth_VCF;
            
            % Obtain transform instance from Model
            obj.Transform = Model.Transform;
        end

        %% Getters
        %
        % Returns the maxAllowableRange
        function [maxAllowableRange] = getMaxAllowableRange(obj)
            maxAllowableRange = obj.maxAllowableRange;
        end
        
        %% TOF Processing
        %
        % Performs all TOF processing for VANTAGE
        %
        % @param    SensorData  struct of information about sensors
        % @param    Deployer    Deployer class instance, containing
        %                       Cubesats and manifest data
        %
        % @return   Deployer    Deployer class instance, containing
        %                       Cubesats populated with TOF centroid 
        %                       results
        %
        % @author   Joshua Kirby
        % @date     01-Mar-2019
        function [Deployer] = TOFProcessing(obj,SensorData,Deployer,varargin)
            % Extract relevant data from inputs
            CubeSats = Deployer.CubesatArray;
            
            % Obtain filenames from SensorData TOFData dir
            ls = dir(SensorData.TOFData);
            ls = ls([ls.bytes]~=0);
            % Error check data against truth
            if length(obj.Truth_VCF.t) < length(ls)
                error('There are more data files than truth data points')
            end
            
            % Initialize varargin mutable parameters
            fileLims = [1,length(ls)];
            presentResults = 0;
            
            % Process varargin
            if ~isempty(varargin)
                for i = 1:length(varargin)
                    % limiting which files to process from the directory
                    if strcmpi(varargin{i},'fileLims')
                        if size(varargin{i+1},1) == 1 && size(varargin{i+1},2) == 2
                            fileLims = varargin{i+1};
                            if fileLims(1) < 1 || fileLims(2)>length(ls)
                                error('fileLims arguments are out of range for the files in SensorData.TOFData')
                            end
                        else
                            error('fileLims requires a following [minFileIndex maxFileIndex] argument')
                        end
                    end
                    % whether or not to plot results for every file
                    % processed
                    if strcmpi(varargin{i},'presentResults')
                        if length(varargin{i+1}) == 1
                            if varargin{i+1}
                                presentResults = 1;
                            end
                        else
                            error('presentResults requires a following 1x1 argument that is convertible to logical')
                        end
                    end
                end
            end
            
            % Initialize CubeSats_TOF
            for i = 1:length(CubeSats)
                CubeSats_TOF(i) = VANTAGE.PostProcessing.CubeSat_TOF(CubeSats(i));
            end
            
            % Loop over files for as long as there are files or until the
            % Cubesats leave the maxAllowableRange of the TOF camera
            stopProcessing = 0;
            outOfRange = 0;
            ii = fileLims(1);
            warning('TOFProcessing currently processes all files, set a stopping condition based on maxAllowableRange as well')
            while ~stopProcessing
                % Naively identify centroids in image
                [CubeSats_TOF,pc] = obj.naiveFindCentroids(ls(ii).name,SensorData,CubeSats_TOF);
                % Associate with known cubesats within Deployer
                warning('unimplemented')
                % Determine if cubesats have passed out of range (sets
                % outOfRange)
                warning('unimplemented')
                % Present Results
                if presentResults
                    obj.plotResults(CubeSats_TOF,pc,obj.Truth_VCF.t(ii));
                end
                % Iterate counter
                ii = ii + 1;
                % Stop processing?
                if (ii > fileLims(2)) || outOfRange
                    stopProcessing = 1;
                end
            end
            
            % Update Deployer CubesatArray
            Deployer.CubesatArray = CubeSats;
            
        end
        
        %% Calculate centroids from file
        %
        % Produces centroids of all CubeSats naively identified in a single
        % point cloud
        %
        % @param    filename        filename for a single point cloud, char
        % @param    SensorData      struct of information about sensors
        % @param    CubeSats_TOF    initialized array of CubeSat_TOF
        %                           objects ordered [first-out to
        %                           last-out] and containing expectedU and
        %                           actualDims
        %
        % @return	length-n array of Cubesat_TOF's containing identified
        %           TCF centroids
        % @return   raw point cloud from file
        %
        % @author   Joshua Kirby
        % @date     04-Mar-2019
        function [CubeSats_TOF,pc] = naiveFindCentroids(obj,filename,SensorData,CubeSats_TOF)
            % Extract point cloud locations
            filename = strcat(SensorData.TOFData,filename);
            
            % Load point cloud from file
            warning('Load script is currently only designed to work with simulated .pcd files')
            pc = obj.loadSimFile(filename);
            
            % Only proceed if the point cloud is valid
            if ~isempty(pc.XLimits)
                % Separate point cloud into identified cubesats
                [obj,CubeSats_TOF] = obj.cubesatPointsFromPC(pc,CubeSats_TOF);

                for i = 1:length(CubeSats_TOF)
                    % If a point cloud was found for this cubesat
                    if ~isempty(CubeSats_TOF(i).pc)
                        % Identify visible planes for each cubesat
                        CubeSats_TOF(i) = obj.fitPlanesToCubesat(CubeSats_TOF(i));

                        % Calculate centroids by projecting from identified planes
                        CubeSats_TOF(i) = obj.findCentroidFromFaces(CubeSats_TOF(i));
                    end
                end
            else
                CubeSats_TOF = VANTAGE.PostProcessing.CubeSat_TOF.empty;
            end
        end
        
    end
    
    %% Private Methods
    methods  (Access = private)
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
        function pc = loadSimFile(~,filename)
            ptCloud = pcread(filename);
            
            if nnz(~isnan(ptCloud.Location))==0
                pc = pointCloud(nan(1,3));
            else
                % Filter extraneous points
                pts = reshape(ptCloud.Location,ptCloud.Count,3);
                I = logical(prod(~isnan(pts),2));
                pc = pointCloud(pts(I,:));
            end
            
            if 0 % useful for debugging
                figure            
                pcshow(pc)
                xlabel('x')
                ylabel('y')
                zlabel('z')
            end
            
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
        % @param    pc              raw point cloud from file
        % @param    CubeSats_TOF    initialized array of CubeSat_TOF
        %                           objects ordered [first-out to
        %                           last-out] and containing expectedU and
        %                           actualDims
        %
        % @return	array of identified cubesats 
        %           @see CubeSat_TOF class
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
        function [obj,CubeSats_TOF] = cubesatPointsFromPC(obj,pc,CubeSats_TOF)
            % Pull out z values
            z = sort(pc.Location(:,3));

            % Start the loop
            NPeaks = Inf;
            bw = 0.01;
            counter = 0;
            while NPeaks > length(CubeSats_TOF)-1
              % Calculate k-squares density
              [zDense,zBin] = ksdensity(z,'bandwidth',bw,'function','pdf');

              % Identify split locations
              c = 0.3;
              h = -1.5;
              warning('off','signal:findpeaks:largeMinPeakHeight')
              [pks,locs] = findpeaks(-zDense,zBin,'MinPeakProminence',c,'MinPeakHeight',h);
              warning('on','signal:findpeaks:largeMinPeakHeight')
              
              % Update bandwidth
              NPeaks = length(locs);
              bw = 1.05*bw;

              % give up if too many tries
              counter = counter + 1;
              if counter > 30
                error('Bandwidth for point splitting ksdensity function did not converge in 10 tries, implement a better bw update')
              end
            end
            
            if 0 % useful for debugging
                figure
                hold on
                grid on
                grid minor
                plot(zBin,-zDense);
                plot(locs,pks,'r*')
                xlabel('z (m)')
                ylabel('Percent point density')
                hold off
            end

            locs = flip(locs);
            nSplit = numel(locs);

            % Separate point cloud by split planes
            if nSplit>0
                for i = 1:nSplit
                    if i==1
                        I = pc.Location(:,3)>=locs(i);
                        CubeSats_TOF(i).pc = pointCloud(pc.Location(I,:));
                    else
                        I = (pc.Location(:,3)>=locs(i)) & (pc.Location(:,3)<locs(i-1));
                        CubeSats_TOF(i).pc = pointCloud(pc.Location(I,:));
                    end
                end
                I = pc.Location(:,3)<locs(nSplit);
                CubeSats_TOF(nSplit+1).pc = pointCloud(pc.Location(I,:));
            else
                CubeSats_TOF(1).pc = pc;
            end
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
                [planes(numPlanes).n,planes(numPlanes).V,planes(numPlanes).o] = obj.affine_fit(double(planes(numPlanes).planeCloud.Location));
                
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
                            numPlanes = numPlanes-1;
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
        function CubeSat_TOF = centroid1(obj,CubeSat_TOF)
            %%% Data extract
            face = CubeSat_TOF.faces;
            
            %%% Find plane boundary points and centroid
            [face.faceCentr,face.corners] = obj.getFaceCentroidAndCorners(face);
            
            %%% Identify face corners using bounding box of face
            [fullFace,trustedLen,~,faceIndex] = obj.diagnoseFace(face,CubeSat_TOF);
            
            %%% Find centroid of face in 3D
            if fullFace
                % mean corners to find centroid
                centr = mean(face.corners,2);
            else
                % project to centroid from two most distant (downrange) corners
                % Obtain distant two corners and their midPt                
                [~,I] = sort(face.corners(3,:),'descend');
                farCorners = face.corners(:,I(1:2));
                farMidPt = mean(farCorners,2);
                
                % Determine which trustedLen corresponds to the side that
                % is orthogonal to corners
                [~,I] = min(abs(trustedLen-norm(farCorners(:,1)-farCorners(:,2))));
                projLength = trustedLen([1:I-1,I+1:end]);
                
                % Extract face normal vector
                nhat = face(1).n;
                
                % Obtain vector from midPt to a corner
                relCorner = farCorners(:,2) - farMidPt;
                
                % Obtain projection direction from farMidPt orthogonal to corners and
                % towards the face interior
                midPtToInterior = face.faceCentr - farMidPt;
                projHat = obj.unitvec( cross(nhat,relCorner) );
                projHat = sign( dot(projHat,midPtToInterior) ) * projHat;
                
                % Project from farMidPt to the desired face centroid
                centr = farMidPt + projLength/2 * projHat;
            end
            
            %%% Project inward to volumetric centroid
            d = obj.distInFromFace(CubeSat_TOF,faceIndex);
            CubeSat_TOF.centroid_TCF = centr + d*sign(dot(centr,face.n))*face.n;
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
        function CubeSat_TOF = centroid2(obj,CubeSat_TOF)
            %%% Extract data
            faces = CubeSat_TOF.faces;
            
            %%% Check that planes intersect as expected
            if rank([faces.n]) ~= 2
                error('The two planes identified do not intersect as a line')
            end
            
            %%% Find faces' centroids and corners
            %%%     and determine if each face qualifies as a fullFace
            for i = 1:length(faces)
                [faces(i).faceCentr,faces(i).corners] = ...
                    obj.getFaceCentroidAndCorners(faces(i));
                [~,~,faces(i).trustedCorners,faces(i).faceIndex] = ...
                    obj.diagnoseFace(faces(i),CubeSat_TOF);
            end
            
            %%% Determine unit vector parallel to intersection
            v_intersect = cross(faces(1).n,faces(2).n);
            
            %%% Get mean point along intersect
            % Project trusted corners onto intersect
            allTrustedCorners = [faces(1).trustedCorners,faces(2).trustedCorners];
            corners_alongIntersect = allTrustedCorners' * v_intersect;
            % Find distance from min to mean along intersect
            [min_alongIntersect,minI]  = min(corners_alongIntersect);
            tmp_mean = mean([max(corners_alongIntersect),min_alongIntersect]);
            mean_alongIntersect = tmp_mean-min_alongIntersect;
            % Identify min_alongIntersect in 3D space by projecting min
            %   point onto intersect three dimensionally
            if minI <= 4 % if point is in face 1
                otherFace = 2;
            else % if point is in face 2
                otherFace = 1;
            end
            orthDist = (dot(faces(otherFace).n,faces(otherFace).o)-...
                dot(faces(otherFace).n,allTrustedCorners(:,minI)))/norm(faces(otherFace).n)^2;
            minpt = allTrustedCorners(:,minI) + orthDist*faces(otherFace).n;
            % Locate mean point along intersect in 3D
            meanpt = minpt + mean_alongIntersect*v_intersect;
            
            %%% Project into centroid from mean point along intersect
            n = zeros(3,2);
            for i = 1:2
                % ensure that unit normals are inward facing
                n(:,i) = sign(dot(meanpt,faces(i).n))*faces(i).n;
                % calculate orthogonal distance from each face to the centroid
                d(i) = obj.distInFromFace(CubeSat_TOF,faces(i).faceIndex);
            end
            % Project to centroid
            CubeSat_TOF.centroid_TCF = meanpt + sum(d.*n,2);
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
        function CubeSat_TOF = centroid3(obj,CubeSat_TOF)
            %%% Extract data
            faces = CubeSat_TOF.faces;
            
            %%% Find faces' centroids and corners
            %%%     and determine if each face qualifies as a fullFace
            for i = 1:length(faces)
                [faces(i).faceCentr,faces(i).corners] = ...
                    obj.getFaceCentroidAndCorners(faces(i));
                [~,~,faces(i).trustedCorners,faces(i).faceIndex] = ...
                    obj.diagnoseFace(faces(i),CubeSat_TOF);
            end
            
            %%% Find plane intersection
            if rank(A) ~= 3
                error('Planes do not intersect in the expected way')
            end
            for i = 1:3
                A(i,1:3) = faces(i).n;
                b(i,1)   = faces(i).n(1)*faces(i).o(1) + faces(i).n(2)*faces(i).o(2) + faces(i).n(3)*faces(i).o(3);
            end
            ptIntersect = (A\b);
            
            %%% Project into centroid from intersection point
            n = zeros(3,3);
            for i = 1:3
                % ensure that unit normals are inward facing
                n(:,i) = sign(dot(ptIntersect,faces(i).n))*faces(i).n;
                % calculate orthogonal distance from each face to the centroid
                d(i) = obj.distInFromFace(CubeSat_TOF,faces(i).faceIndex);
            end
            % Project to centroid
            CubeSat_TOF.centroid_TCF = ptIntersect + sum(d.*n,2);
            
            if 0
            %%% Find Best-Guess Face
            % ensure that unitvectors are inward facing
            for i = 1:3
                n(:,i) = sign(dot(ptIntersect,faces(i).n))*faces(i).n;
            end
            % Make best guess for which face is which
            As = CubeSat_TOF.Avec;
            for i = 1:3
                inPlane = double( (faces(i).planeCloud.Location-faces(i).o')*faces(i).V );
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
                V = cross(faces(bestI).n,faces(i).n);
                % project point cloud onto this unit vector direction
                pc_alongIntersect = [faces(bestI).planeCloud.Location;faces(i).planeCloud.Location]*V;
                % Determine length of intersection
                lengthIntersect   = max(pc_alongIntersect)-min(pc_alongIntersect);
                
                % Make best guess at which face this is
                switch bestSize
                    case {1,7}
                        offI(c)    = i;
                        offSize(c) = u;
                    case u
                        offI(c)        = i;
                        [~,offSize(c)] = min(abs(CubeSat_TOF.Lvec-lengthIntersect));
                    otherwise
                        error('Best-guess for face size does not match an allowable size (1 or U)')
                end
                c = c + 1;
            end
            
            %%% Find volumetric centroid
            % volumetric diagonal
            switch bestSize
                case {1,7}
                    innerDiag = unitvec(n(:,bestI)*CubeSat_TOF.u_long*u + n(:,offI(1))*CubeSat_TOF.Lvec(bestSize) + n(:,offI(2))*CubeSat_TOF.Lvec(bestSize));
                case u
                    innerDiag = unitvec(n(:,bestI)*CubeSat_TOF.Lvec(7) + n(:,offI(1))*CubeSat_TOF.Lvec(offSize(2)) + n(:,offI(2))*CubeSat_TOF.Lvec(offSize(1)));
                otherwise
                    error('Best-guess for face size does not match an allowable size (1 or U)')
            end
            CubeSat_TOF.centroid_TCF = ptIntersect + 0.5*sqrt((u*CubeSat_TOF.u_long)^2+2*(CubeSat_TOF.u_short)^2)*innerDiag;
            end
        end
        
        
        %% Subroutines
                %
        % Smooths a face boundary and returns the smoothed boundary and
        % face centroid
        %
        % @param    face    scalar CubeSat_TOF.faces struct 
        % 
        % @return   centroid of face
        % @return   corners of face
        %
        % @author   Joshua Kirby
        % @date     07-Mar-2019
        function [faceCentr, corners] = getFaceCentroidAndCorners(obj,face)
            %%% Project points onto 2d plane
            plane = face.planeCloud;
            inPlane = double( (plane.Location-face(1).o')*face.V );
            
            %%% Find plane boundary points
            I_bound = boundary(inPlane(:,1),inPlane(:,2));
            
            %%% Find filtered boundary
            boundaryPtsRaw = [inPlane(I_bound,1)';inPlane(I_bound,2)'];
            order = 2;
            frameLen = obj.roundToNearestOdd(size(boundaryPtsRaw,2)/5);
            boundaryPts = sgolayfilt(boundaryPtsRaw,order,frameLen,[],2);
            
            %%% Find face centroid
            faceShape = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',true);
            [x,y] = centroid(faceShape);
            faceCentr = face.o + face.V * [x;y];
            
            % Find face boundary in face plane
            corners_inPlane = obj.minBoundingBox(boundaryPts);
            % Convert to 3D
            corners = zeros(3,4);
            for i = 1:4
                corners(:,i) = face.o + face.V * corners_inPlane(:,i);
            end
        end
        
        % 
        % Determines if a CubeSat.face struct is a full CubeSat face
        %
        % @params   face            CubeSat_TOF.face struct containing
        %                           corners calculated using
        %                           getFaceCentroidAndCorners
        % @params   CubeSat_TOF     CubeSat_TOF object with which the
        %                           corners are associated, used only for
        %                           obtaining actualDims
        %
        % @return   boolean isFullFacePresent? 0-no, 1-yes
        % @return   the trusted side lengths of the face, even if a full face is
        %           not present
        % @return   index of face, indices correspond to faces composed of
        %           CubeSat.actualDims([combnk(1:3,2)](index,i))
        %
        % @author   Joshua Kirby
        % @date     07-Mar-2019
        function [fullFace,trustedLen,trustedCorners,faceIndex] = ...
                diagnoseFace(obj,face,CubeSat_TOF)
            % Extract data
            corners = face.corners;
            
            % Initialize boolean
            fullFace = 1;
            
            %%% Decide if full face is present or not
            % Determine if bounding box is near cubesat size
            len = zeros(1,2);
            deltaLen = zeros(3,2);
            for i = 1:2
                len(i) = norm(corners(:,i+1) - corners(:,i));
                deltaLen(:,i) = abs(len(i) - CubeSat_TOF.actualDims);
                faceMatches{i} = find(deltaLen(:,i) < obj.faceLenMatchTol);
            end
            % For fullFace, both sides must have a length match
            fullFaceCondition = ~isempty(faceMatches{1}) & ~isempty(faceMatches{2});
            % and each side must have an independent match
            matchCombinations = combvec(faceMatches{1}',faceMatches{2}');
            diffSides = ~(matchCombinations(1,:)-matchCombinations(2,:));
            fullFaceCondition = fullFaceCondition & any(diffSides);
            
            %fullFaceCondition = fullFaceCondition & sum(ismember(faceMatches{1},faceMatches{2}))~=length(faceMatches{1});
            %fullFaceCondition = fullFaceCondition & sum(ismember(faceMatches{2},faceMatches{1}))~=length(faceMatches{2});
            if ~fullFaceCondition
                fullFace = 0;
            end
            
            %%% Determine trusted lengths
            if fullFace
                % trusted side lengths are visible side lengths
                trustedLen = len;
            else
                % NOTE: this assumes that the CubeSat has not tumbled much
                %   since launching, which is very valid for the TOF camera
                % Determine trusted lengths
                [trustedActualDimI,trustedLenI] = find(min(min(deltaLen))==deltaLen,1);
                trustedLen = zeros(1,2);
                trustedLen(1) = len(trustedLenI);
                if trustedActualDimI ~= find(CubeSat_TOF.actualDims == max(CubeSat_TOF.actualDims))
                    trustedLen(2) = max(CubeSat_TOF.actualDims);
                else
                    trustedLen(2) = min(CubeSat_TOF.actualDims);
                end
            end
            
            %%% Determine trusted corners
            % project to centroid from two most distant (downrange) corners
            % Obtain distant two corners and their midPt                
            [~,I] = sort(face.corners(3,:),'descend');
            farCorners = face.corners(:,I(1:2));
            nearBadCornersUnordered = face.corners(:,I(3:4));
            farMidPt = mean(farCorners,2);

            % Determine which trustedLen corresponds to the side that
            % is orthogonal to corners
            [~,I] = min(abs(trustedLen-norm(farCorners(:,1)-farCorners(:,2))));
            projLength = trustedLen([1:I-1,I+1:end]);
            
            % Determine near bad corners which correspond to far corners
            nearBadCorners = zeros(3,2);
            for i = 1:size(farCorners,2)
                for j = 1:size(nearBadCornersUnordered,2)
                    v1 = farCorners(:,[1:i-1,i+1:end]) - farCorners(:,i);
                    v2 = nearBadCornersUnordered(:,j) - farCorners(:,i);
                    if abs(dot(v1,v2)) < 1e-06
                        nearBadCorners(:,i) = nearBadCornersUnordered(:,j);
                    end
                end
            end
            
            % Project through near bad corners to near corners
            nearCorners = zeros(3,2);
            for i = 1:2
                u = obj.unitvec(nearBadCorners(:,i) - farCorners(:,i));
                nearCorners(:,i) = farCorners(:,i) + projLength*u;
            end
            
            % Assemble corners in circular order
            trustedCorners = [fliplr(nearCorners),farCorners];

            % Extract face normal vector
            nhat = face(1).n;

            % Obtain vector from midPt to a corner
            relCorner = farCorners(:,2) - farMidPt;

            % Obtain projection direction from farMidPt orthogonal to corners and
            % towards the face interior
            midPtToInterior = face.faceCentr - farMidPt;
            projHat = obj.unitvec( cross(nhat,relCorner) );
            projHat = sign( dot(projHat,midPtToInterior) ) * projHat;
            
            %%% Determine faceIndex
            % Dimensions to choose from, corresponding to indices of
            %   CubeSat.actualDims
            dims = 1:length(CubeSat_TOF.actualDims);
            % Calculate standard areas
            C = combnk(dims,2);
            Avec = prod(CubeSat_TOF.actualDims(C),2);
            % Calculate difference between area and standard areas
            faceArea = prod(trustedLen);
            dA = abs(faceArea-Avec);
            % Choose nearest size
            [~,faceIndex] = min(dA);
        end

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
        function [n,V,p] = affine_fit(~,X)
            %the mean of the samples belongs to the plane
            p = mean(X,1)';
            
            %The samples are reduced:
            R = bsxfun(@minus,X,p');
            %Computation of the principal directions if the samples cloud
            [V,~] = eig(R'*R);
            %Extract the output from the eigenvectors
            n = V(:,1);
            V = V(:,2:end);
        end
        
        %
        % Calculate distance to project perpendicularly inward from a face
        % to the cubesat centerline
        %
        % @param    CubeSat     instance of VANTAGE.PostProcessing.CubeSat_TOF
        %                       @see CubeSat_TOF
        % @param    faceArea    index of face, indices correspond to faces 
        %                       composed of CubeSat.actualDims([combnk(1:3,2)](index,i))
        %
        % @return  perpendicular distance from face to centroid
        %
        % @author   Joshua Kirby
        % @date     07-Feb-2019
        function [d] = distInFromFace(~,CubeSat,faceIndex)
            % Dimensions to choose from, corresponding to indices of
            % CubeSat.actualDims
            dims = 1:length(CubeSat.actualDims);
            
            % Calculate standard areas
            C = combnk(dims,2);
            
            % Set distance in for centroid
            distI = dims(~ismember(dims,C(faceIndex,:)));
            d = CubeSat.actualDims(distI)/2;
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
        function [uvec] = unitvec(~,vec)
            uvec = vec./norm(vec);
        end
        
        
        %
        % Presents results from a single array of CubeSat_TOF objects
        % (i.e. a single TOF point cloud)
        %
        % @param    CubeSats_TOF    array of CubeSat_TOF objects after
        %                           processing the associated point cloud
        % @param    pc              raw point cloud from file
        % @param    truthTime       timestamp of truth data which
        %                           corresponds to the data file whose
        %                           results are being presented
        %
        % @author   Joshua Kirby
        % @date     04-Mar-2019
        function plotResults(obj,CubeSats_TOF,pc,truthTime)
            
            % Define indexing for use in looping over cubesats
            CubesatIndexing = 1:length(CubeSats_TOF);
            
            % Extract true centroids from truth data for this file
            I = find(obj.Truth_VCF.t == truthTime,1);
            for i = 1:obj.Truth_VCF.numCubeSats
                trueCentroids_VCF(i,:) = obj.Truth_VCF.Cubesat(i).pos(I,:);
            end
            
            % Transform true centroids from VCF to TCF
            trueCentroids_TCF = [obj.Transform.tf('TCF',trueCentroids_VCF','VCF')]';
            
            % Fit line to trueCentroids to use as plotting centerline for
            % reference
            warning('off','curvefit:fit:equationBadlyConditioned')
            trueCentroidsLine_TCF = fit(trueCentroids_TCF(:,2:3),trueCentroids_TCF(:,1),'poly11');
            warning('on','curvefit:fit:equationBadlyConditioned')
            
            % Get point for plotting the centerline
            minpt = min(trueCentroids_TCF,[],1);
            maxpt = max(trueCentroids_TCF,[],1);
            yCenterline = linspace(minpt(2),maxpt(2),2);
            zCenterline = linspace(minpt(3)-0.3,maxpt(3)+0.3,2);
            xCenterline = feval(trueCentroidsLine_TCF,yCenterline,zCenterline);
            
            % Plot results
            figure('units','normalized','position',[1/16,1/8,14/16,6/8])
            c = get(gca,'colororder');
            markersize = 100;
            fontsize = 20;
            legendfontsize = 16;
            
            subplot(1,3,1)
            hold on
            grid on
            grid minor
            pcshow(pc,'markersize',markersize)
            plot3(xCenterline,yCenterline,zCenterline,'k--','linewidth',2)
            title('Raw Pointcloud','fontsize',fontsize)
            ax = gca;
            zlimits = ax.ZLim;
            ylimits = ax.YLim;
            xlabel('x')
            ylabel('y')
            zlabel('z')
            hold off
            
            
            subplot(1,3,2)
            grid on
            grid minor
            title('Identified CubeSats','fontsize',fontsize)
            hold on
            colorcounter = 1;
            legendstrings = [];
            legendcounter = 1;
            for i = CubesatIndexing
                if ~isempty(CubeSats_TOF(i).pc)
                    CubeSats_TOF(i).pc.Color = uint8(c(colorcounter,:).*255.*ones(CubeSats_TOF(i).pc.Count,3));
                    pcshow(CubeSats_TOF(i).pc,'markersize',markersize)
                    legendStrings{legendcounter} = ['CubeSat ',num2str(i)];
                    colorcounter = colorcounter+1;
                    legendcounter = legendcounter+1;
                end
            end
            plot3(xCenterline,yCenterline,zCenterline,'k--','linewidth',2)
            zlim(zlimits)
            ylim(ylimits)
            if ~isempty(legendstrings)
                l=legend(legendStrings,'location','eastoutside');
                l.FontSize = legendfontsize;
            end
            xlabel('x')
            ylabel('y')
            zlabel('z')
            hold off
            
            subplot(1,3,3)
            hold on
            grid on
            grid minor
            title('Visible Faces Per CubeSat','fontsize',fontsize)
            legendStrings = [];
            legendcounter = 1;
            colorcounter = 1;
            for i = CubesatIndexing
                if ~isempty(CubeSats_TOF(i).pc)
                    plot3(CubeSats_TOF(i).centroid_TCF(1),CubeSats_TOF(i).centroid_TCF(2),CubeSats_TOF(i).centroid_TCF(3),'r.','markersize',25)
                    plot3(trueCentroids_TCF(i,1),trueCentroids_TCF(i,2),trueCentroids_TCF(i,3),'b.','markersize',25)
                    legendStrings{legendcounter} = ['Calc.Centr',num2str(i)];
                    legendStrings{legendcounter+1} = ['True.Centr',num2str(i)];
                    legendcounter = legendcounter+2;
                    for j = 1:CubeSats_TOF(i).numVisibleFaces
                        CubeSats_TOF(i).faces(j).planeCloud.Color = uint8(c(colorcounter,:).*255.*ones(CubeSats_TOF(i).faces(j).planeCloud.Count,3));
                        pcshow(CubeSats_TOF(i).faces(j).planeCloud,'markersize',markersize)
                        legendStrings{legendcounter} = ['Sat',num2str(i),'-Face',num2str(j)];
                        colorcounter = colorcounter+1;
                        legendcounter = legendcounter+1;
                    end
                end
            end
            plot3(xCenterline,yCenterline,zCenterline,'k--','linewidth',2)
            zlim(zlimits)
            ylim(ylimits)
            if ~isempty(legendstrings)
                l=legend(legendStrings,'location','eastoutside');
                l.FontSize = legendfontsize;
            end
            xlabel('x')
            ylabel('y')
            zlabel('z')
            hold off
        end
        
        
        % @author   Julien Diener
        % @date     05-Mar-2019
        function bb = minBoundingBox(~,X)
            % compute the minimum bounding box of a set of 2D points
            %   Use:   boundingBox = minBoundingBox(point_matrix)
            %
            % Input:  2xn matrix containing the [x,y] coordinates of n points
            %         *** there must be at least 3 points which are not collinear
            % output: 2x4 matrix containing the coordinates of the bounding box corners
            %
            % Example : generate a random set of point in a randomly rotated rectangle
            %     n = 50000;
            %     t = pi*rand(1);
            %     X = [cos(t) -sin(t) ; sin(t) cos(t)]*[7 0; 0 2]*rand(2,n);
            %     X = [X  20*(rand(2,1)-0.5)];  % add an outlier
            %
            %     tic
            %     c = minBoundingBox(X);
            %     toc
            %
            %     figure(42);
            %     hold off,  plot(X(1,:),X(2,:),'.')
            %     hold on,   plot(c(1,[1:end 1]),c(2,[1:end 1]),'r')
            %     axis equal
            
            % compute the convex hull (CH is a 2*k matrix subset of X)
            k = convhull(X(1,:),X(2,:));
            CH = X(:,k);
            
            % compute the angle to test, which are the angle of the CH edges as:
            %   "one side of the bounding box contains an edge of the convex hull"
            E = diff(CH,1,2);           % CH edges
            T = atan2(E(2,:),E(1,:));   % angle of CH edges (used for rotation)
            T = unique(mod(T,pi/2));    % reduced to the unique set of first quadrant angles
            
            % create rotation matrix which contains
            % the 2x2 rotation matrices for *all* angles in T
            % R is a 2n*2 matrix
            R = cos( reshape(repmat(T,2,2),2*length(T),2) ... % duplicate angles in T
                + repmat([0 -pi ; pi 0]/2,length(T),1));   % shift angle to convert sine in cosine
            
            % rotate CH by all angles
            RCH = R*CH;
            
            % compute border size  [w1;h1;w2;h2;....;wn;hn]
            % and area of bounding box for all possible edges
            bsize = max(RCH,[],2) - min(RCH,[],2);
            area  = prod(reshape(bsize,2,length(bsize)/2));
            
            % find minimal area, thus the index of the angle in T
            [~,i] = min(area);
            
            % compute the bound (min and max) on the rotated frame
            Rf    = R(2*i+[-1 0],:);   % rotated frame
            bound = Rf * CH;           % project CH on the rotated frame
            bmin  = min(bound,[],2);
            bmax  = max(bound,[],2);
            
            % compute the corner of the bounding box
            Rf = Rf';
            bb(:,4) = bmax(1)*Rf(:,1) + bmin(2)*Rf(:,2);
            bb(:,1) = bmin(1)*Rf(:,1) + bmin(2)*Rf(:,2);
            bb(:,2) = bmin(1)*Rf(:,1) + bmax(2)*Rf(:,2);
            bb(:,3) = bmax(1)*Rf(:,1) + bmax(2)*Rf(:,2);
        end
        
        % 
        % Round to nearest odd integer
        %
        % @param    x   number to be rounded
        % 
        % @return   nearest odd integer
        %
        % @author   Joshua Kirby
        % @date     06-Mar-2019
        function y = roundToNearestOdd(obj,x)
            y = 2*round(x/2) + obj.binarySign(x-round(x));
        end
        
        % 
        % Return sign without zero, for zero return 1
        %  
        % @param    num     number whose sign is to be determined
        %
        % @return   binary sign
        %
        % @author   Joshua Kirby
        % @date     06-Mar-2019
        function bSign = binarySign(~,num)
            bSign = sign(num);
            if ~bSign
                bSign = 1;
            end
        end
        
    end
    
    
    
end % classdef