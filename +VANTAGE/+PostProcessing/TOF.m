classdef TOF
    %% Classdef
    % A class representing the Time of Flight Camera used by VANTAGE
    %
    %% Properties       
    properties (Access = private)
        %
        % Maximum range within which TOF data is considered valid, m
        maxAllowableRange
        
        % Maximum allowed distance from a plane to be considered part of the plane
        ptMaxDistFromPlane
        
        % Face length matching tolerance, for use in centroid1 method
        faceLenMatchTol = 0.02; % m

        % Reference to Model class
        ModelRef
        
        % fps
        fps
        
        % fov
        HVfov_deg
        
        % showDebugPlots, default to 0
        showDebugPlots = 0
        
        % max number of consecutive outliers for a CubeSat
        maxConsOutliers = 3;
        
        % scalar by which to multiply the standard deviation of all previous
        % centroid measurements for each CubeSat, the value
        % (outlierMultiplier*stdOfAllPreviousMeas) is used as the threshold for
        % separating outliers from valid data points
        outlierMultiplier = 10;
        
    end
    
    %% Methods
    methods
        % Constructor
        %
        % @param      modelRef         Model class handle (@see Model)
        % @param      configFileneame  The configuration filename
        %
        % @return     A reference to an initialized TOF object
        %
        function obj = TOF(ModelRef,configFilename)
            % Clear persistents
            clear obj.associateCentroids
            
            % Store Model class reference
            obj.ModelRef = ModelRef;

            % Read data from configuration file
            configData = jsondecode(fileread(configFilename));

            % Initialize configuration parameters
            obj.maxAllowableRange  = configData.maxAllowableRange; % m
            obj.ptMaxDistFromPlane = configData.ptMaxDistFromPlane; % m
            obj.fps                = configData.fps;
            obj.HVfov_deg          = configData.HVfov_deg; % deg
            
            % Obtain truth data from Model
            if ~isa(obj.ModelRef,'VANTAGE.PostProcessing.Model')
                error('Model input to TOF class object constructor must be a VANTAGE.PostProcessing.Model handle')
            end
        end

        %% Getters
        %
        % Returns the maxAllowableRange
        function [maxAllowableRange] = getMaxAllowableRange(obj)
            maxAllowableRange = obj.maxAllowableRange;
        end
        
        % Returns the TOF weight given a predicted mean range of cubesats
        %
        % @param    predMeanRange   predicted mean range of CubeSats, m
        %
        % @return   current TOF weight for sensor fusion
        %
        % @author   Joshua Kirby
        % @date     07-Apr-2019
        function [sigmaTOF] = TofWeighting(~,predMeanRange)
            if predMeanRange <= 10
                sigmaTOF = 1;
            else
                sigmaTOF = interp1([10 100],[1,0.5],predMeanRange);
            end
        end
        
        %% TOF Processing
        %
        % Performs all TOF processing for VANTAGE
        %
        % @param    SensorData  struct of information about sensors
        % @param    Deployer    Deployer class instance, containing
        %                       Cubesats and manifest data
        %
        % @return   Deployer class instance, containing
        %           Cubesats populated with TOF centroid 
        %           results
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
            if length(obj.ModelRef.Truth_VCF.t) < length(ls)
                error('There are more data files than truth data points')
            end
            % Calculate timestamps
            filenames = strings(1,length(ls));
            for i = 1:length(ls)
                filenames(i) = string(ls(i).name);
            end
            time = obj.ModelRef.TimeMan.VantageTime(filenames,'TOF');
            
            % Initialize varargin mutable parameters
            fileLims = [1,length(ls)];
            presentResults = 0;
            
            % Process varargin
            if ~isempty(varargin)
                for i = 1:length(varargin)
                    % limiting which files to process from the directory
                    if strcmpi(varargin{i},'fileLims')
                        if size(varargin{i+1},1) == 1 && size(varargin{i+1},2) == 2
                            fileLims(1) = max(fileLims(1),varargin{i+1}(1));
                            fileLims(2) = min(fileLims(2),varargin{i+1}(2));
                        else
                            error('fileLims requires a following [minFileIndex maxFileIndex] argument')
                        end
                    end
                    % whether or not to plot results for every file
                    % processed
                    if strcmpi(varargin{i},'presentResults')
                        if length(varargin{i+1}) == 1
                            if varargin{i+1}
                                presentResults = varargin{i+1};
                            end
                        else
                            error('presentResults requires a following 1x1 argument that is convertible to logical')
                        end
                    end
                    % whether or not to plot debugging plots for every file
                    if strcmpi(varargin{i},'showDebugPlots')
                        if length(varargin{i+1}) == 1
                            if varargin{i+1}
                                obj.showDebugPlots = varargin{i+1};
                            end
                        else
                            error('showDebugPlots requires a following 1x1 argument that is convertible to logical')
                        end
                    end
                end
            end
            
            % Loop over files for as long as there are files or until the
            % Cubesats leave the maxAllowableRange of the TOF camera
            stopProcessing = 0;
            outOfRange = 0;
            centroidingDevolved = 0;
            numConsOutliers = zeros(1,length(CubeSats));
            ii = fileLims(1);
            while ~stopProcessing
                % Initialize (disposable) CubeSats_TOF
                for i = 1:length(CubeSats)
                    CubeSats_TOF(i) = VANTAGE.PostProcessing.CubeSat_TOF(CubeSats(i));
                end
                % Naively identify centroids in image
                [CubeSats_TOF,pc] = obj.naiveFindCentroids(ls(ii).name,time(ii),SensorData,CubeSats_TOF);
                % Determine if CubeSats are passing out of range
                if mean(pc.Location(:,3)) > obj.maxAllowableRange
                    disp(['Mean of the analyzed point cloud passed out of the ',...
                        'maxAllowableRange for the TOF, this will be the last file',...
                        ' analyzed...']);
                    outOfRange = 1;
                end
                % Associate with known cubesats within Deployer
                [CubeSats,numConsOutliers] = obj.associateCentroids(CubeSats_TOF,CubeSats,numConsOutliers);
                % Determine if all of the CubeSats have too many
                % consecutive outliers
                if ~any(numConsOutliers < obj.maxConsOutliers)
                    centroidingDevolved = 1;
                end
                % Present Results
                if presentResults
                    obj.plotResults(CubeSats,CubeSats_TOF,...
                        pc,time(ii),numConsOutliers);
                end
                % Iterate counter
                ii = ii + 1;
                % Stop processing?
                if (ii > fileLims(2)) || outOfRange || centroidingDevolved
                    for ii = 1:length(CubeSats)
                        % Save the number of centroids that came from TOF
                        % direct calculation
                        CubeSats(ii).numTOFpoints = length(CubeSats(ii).time);
                        % Determine the final 3D fit model to the TOF data,
                        % used for propagation
                        CubeSats(ii).TOFfit_VCF = obj.produceFinalTOFfit(CubeSats(ii));
                    end
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
        % @param    time            timestamp of this file, scalar
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
        function [CubeSats_TOF,pc] = naiveFindCentroids(obj,filename,time,SensorData,CubeSats_TOF)
            % Extract point cloud locations
            filename = strcat(SensorData.TOFData,filename);
            
            % Load point cloud from file
            pc = obj.loadPcFile(filename);
            for i = 1:length(CubeSats_TOF)
                CubeSats_TOF(i).time = time;
            end
            
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
                        if CubeSats_TOF(i).trustedSat
                            CubeSats_TOF(i) = obj.findCentroidFromFaces(CubeSats_TOF(i));
                        end
                    end
                end
            end
        end
        
        %% Associate centroids from CubeSats_TOF with CubeSats
        %
        % 
        %
        % @param    CubeSats_TOF    CubeSat_TOF object with naive
        %                           centroids in TCF
        % @param    CubeSats        CubeSat object with all previous
        %                           centroids in VCF
        % @param    numConsOutliers a 1xlength(CubeSats) vector of the
        %                           number of consecutive outliers for each CubeSat 
        %                           identified thus far
        %
        % @return   CubeSats object with a new set of centroids from
        %           CubeSats_TOF
        % @return   updated numConsOutliers vector
        %
        % @author   Joshua Kirby
        % @date     10-Mar-2019
        function [CubeSats,numConsOutliers] = associateCentroids(obj,CubeSats_TOF,CubeSats,numConsOutliers)
            % Define persistent variable to save whether the last file
            % contained outliers for each CubeSat
            persistent hadOutlier
            if isempty(hadOutlier)
                hadOutlier = zeros(1,length(CubeSats));
            end
            hasOutlier = false(1,length(CubeSats));
            firstNmeas = 10;
            % Determine populated indices of CubeSats_TOF
            ItofFoundCentroids = find(~cellfun(@isempty,{CubeSats_TOF.centroid_TCF}));
            % Transform naive centroids to VCF
            centroid_VCF = zeros(3,length(CubeSats_TOF));
            for ii = ItofFoundCentroids
                centroid_VCF(:,ii) = obj.ModelRef.Transform.tf('VCF',CubeSats_TOF(ii).centroid_TCF,'TCF');
            end
            % Only proceed if some centroids were found
            if any(ItofFoundCentroids)
                % Ensure range ordering (first-out to last-out) of newly identified centroids
                tmp = [CubeSats_TOF(ItofFoundCentroids).centroid_TCF];
                [~,Irngorder] = sort(tmp(3,:),'descend');
                centroid_VCF = centroid_VCF(:,Irngorder);
                % Determine which CubeSats already have firstNmeas measurements
                IsatsWithEnoughMeas = find(cellfun(@length,{CubeSats.time})>=firstNmeas);
                % Save centroids directly for CubeSats which do not yet
                % have firstNmeas measurements
                ItofUsedCentroids = [];
                if numel(IsatsWithEnoughMeas) < length(CubeSats)
                    ItofUsedCentroids = setdiff(ItofFoundCentroids,IsatsWithEnoughMeas);
                    for ii = ItofUsedCentroids
                        CubeSats(ii).centroids_VCF = [CubeSats(ii).centroids_VCF,centroid_VCF(:,ii)];
                        CubeSats(ii).time = [CubeSats(ii).time,CubeSats_TOF(ii).time];
                    end
                end
                % Obtain predicted point and outlierThresholds for each
                % CubeSat with greater than or equal to firstNmeas measurements
                outlierThreshold = zeros(3,length(CubeSats));
                predPt = ones(3,length(CubeSats)).*inf;
                pX = zeros(length(CubeSats),2);
                pY = zeros(length(CubeSats),2);
                pZ = zeros(length(CubeSats),2);
                stdXYZ = zeros(3,length(CubeSats));
                for ii = IsatsWithEnoughMeas
                    [pX(ii,:),pY(ii,:),pZ(ii,:),stdXYZ(:,ii)] = ...
                        obj.fitLineToCentroids(CubeSats(ii),firstNmeas);
                    outlierThreshold(:,ii) = obj.outlierMultiplier*stdXYZ(:,ii);
                    predPt(:,ii) = [polyval(pX(ii,:),CubeSats_TOF(1).time),...
                              polyval(pY(ii,:),CubeSats_TOF(1).time),...
                              polyval(pZ(ii,:),CubeSats_TOF(1).time)]';
                    % Remove any outliers from CubeSats which just reached
                    % firstNmeas
                    if length(CubeSats(ii).time)==firstNmeas
                        isRecursiveOutlier = false(1,firstNmeas);
                        for jj = 1:firstNmeas
                            recursivePredPt = [polyval(pX(ii,:),CubeSats(ii).time(jj)),...
                              polyval(pY(ii,:),CubeSats(ii).time(jj)),...
                              polyval(pZ(ii,:),CubeSats(ii).time(jj))]';
                            recursiveResidual = abs(recursivePredPt-CubeSats(ii).centroids_VCF(:,jj));
                            isRecursiveOutlier(jj) = ...
                                (recursiveResidual(1) > 2*stdXYZ(1,ii)) || ...
                                (recursiveResidual(2) > 2*stdXYZ(2,ii)) || ...
                                (recursiveResidual(3) > 2*stdXYZ(3,ii));
                        end
                        CubeSats(ii).centroids_VCF(:,isRecursiveOutlier) = [];
                        CubeSats(ii).time(isRecursiveOutlier) = [];
                        % Update IsatsWithEnoughMeas to not include the sat
                        % with removed points
                        if any(isRecursiveOutlier)
                            IsatsWithEnoughMeas(IsatsWithEnoughMeas==ii) = [];
                        end
                    end
                end
                % Update ItofUsedCentroids
                ItofUsedCentroids = setdiff(ItofFoundCentroids,IsatsWithEnoughMeas);
                % Find number of unused new centroids
                IunusedNewCentroids = setdiff(ItofFoundCentroids,ItofUsedCentroids);
                numUnusedNewCentroids = length(IunusedNewCentroids);
                % Get all combinations of numUnusedNewCentroids CubeSats
                % with more than firstNmeas measurements
                if any(IsatsWithEnoughMeas) && numUnusedNewCentroids~=0
                    allCombsSats = combnk(IsatsWithEnoughMeas,numUnusedNewCentroids);
                else % no centroids remain to be associated
                    % Update the consecutive number of outliers and return
                    numConsOutliers = hadOutlier.*numConsOutliers + hasOutlier;
                    hadOutlier = hasOutlier;
                    return
                end
                % Loop over all possible combinations of satellites with
                % which the new centroids could be associated, since
                % CubeSats are ordered (first-out to last-out) and combnk
                % ensures that allCombsSats rows are increasing, each set
                % of possible CubeSats (i.e. each row in allCombsSats) is
                % range ordered
                possibleCubesats(size(allCombsSats,1),size(allCombsSats,2)) = ...
                    struct('centroid_VCF',[],'resFromPredPt',[],'isOutlier',[]);
                for ii = 1:size(allCombsSats,1) % possible combination sets
                    for jj = 1:size(allCombsSats,2) % cubesats in the possible combination
                        currSat = allCombsSats(ii,jj);
                        % directly associate unused new centroids with the
                        % current combination of CubeSats in range ordered
                        % fashion
                        possibleCubesats(ii,jj).centroid_VCF = ...
                            centroid_VCF(:,jj);
                        % Obtain residual between this possible point and
                        % the predicted point for this CubeSat
                        possibleCubesats(ii,jj).resFromPredPt = ...
                            abs(...
                            possibleCubesats(ii,jj).centroid_VCF - ...
                            predPt(:,currSat)...
                            );
                        % Check if any of the coordinates of the residual
                        % are greater than the outlierThreshold for that
                        % coordinate, if so then this point is an outlier
                        possibleCubesats(ii,jj).isOutlier = ...
                            (possibleCubesats(ii,jj).resFromPredPt(1) > outlierThreshold(1,currSat)) || ...
                            (possibleCubesats(ii,jj).resFromPredPt(2) > outlierThreshold(2,currSat)) || ...
                            (possibleCubesats(ii,jj).resFromPredPt(3) > outlierThreshold(3,currSat));
                    end
                end
                % determine non-outlier combinations sets
                possCombsSats = true(size(possibleCubesats,1),1);
                for ii = 1:size(possibleCubesats,2)
                    possCombsSats = possCombsSats & ~[possibleCubesats(:,ii).isOutlier]';
                end
                % associate with cubesats
                if any(possCombsSats) % if any valid combinations were identified
                    if sum(possCombsSats)==1 % if exactly one was identified
                        % save that combination
                        for ii = 1:length(allCombsSats(possCombsSats,:))
                            CubeSats(allCombsSats(possCombsSats,ii)).centroids_VCF = ...
                                [CubeSats(allCombsSats(possCombsSats,ii)).centroids_VCF,...
                                centroid_VCF(:,ii)];
                            CubeSats(allCombsSats(possCombsSats,ii)).time = ...
                                [CubeSats(allCombsSats(possCombsSats,ii)).time,...
                                CubeSats_TOF(ii).time];
                        end
                    else % if more than one was identified
                        % save the combination with minimum residual from
                        % predicted points
                        extractedPossibleSats = possibleCubesats(possCombsSats,:);
                        extractedCombsSats    = allCombsSats(possCombsSats,:);
                        for ii = 1:size(extractedPossibleSats,1)
                            meanResNorm(ii) = mean(vecnorm([extractedPossibleSats(ii,:).resFromPredPt],2,1));
                        end
                        [~,Imin] = min(meanResNorm);
                        % save the minimum mean norm residual
                        for ii = 1:length(extractedCombsSats(Imin,:))
                            CubeSats(extractedCombsSats(Imin,ii)).centroids_VCF = ...
                                [CubeSats(extractedCombsSats(Imin,ii)).centroids_VCF,...
                                centroid_VCF(:,ii)];
                            CubeSats(extractedCombsSats(Imin,ii)).time = ...
                                [CubeSats(extractedCombsSats(Imin,ii)).time,...
                                CubeSats_TOF(Imin).time];
                        end
                    end
                else % if no valid combinations were identified
                    % save the combination of non-outliers which have minimum
                    % mean residual from predicted points
                    resNorm = nan(size(possibleCubesats));
                    meanResNorm = nan(size(possibleCubesats,1),1);
                    for ii = 1:size(possibleCubesats,1)
                        for jj = 1:size(possibleCubesats,2)
                            if ~possibleCubesats(ii,jj).isOutlier
                                resNorm(ii,jj) = norm(possibleCubesats(ii,jj).resFromPredPt);
                            end
                        end
                        meanResNorm(ii) = nanmean(resNorm(ii,:));
                    end
                    [~,Imin] = nanmin(meanResNorm);
                    satsSaved = [];
                    for ii = 1:size(possibleCubesats,2)
                        if ~possibleCubesats(Imin,ii).isOutlier
                            CubeSats(allCombsSats(Imin,ii)).centroids_VCF = ...
                                [CubeSats(allCombsSats(Imin,ii)).centroids_VCF,...
                                centroid_VCF(:,ii)];
                            CubeSats(allCombsSats(Imin,ii)).time = ...
                                [CubeSats(allCombsSats(Imin,ii)).time,...
                                CubeSats_TOF(ii).time];
                            satsSaved = [satsSaved allCombsSats(Imin,ii)];
                        end
                    end
                    hasOutlier(IsatsWithEnoughMeas) = 1;
                    hasOutlier(satsSaved) = 0;
                end
                % Update the consecutive number of outliers
                numConsOutliers = hadOutlier.*numConsOutliers + hasOutlier;
                hadOutlier = hasOutlier;
            end
        end
        
    end
    
    %% Private Methods
    methods  (Access = {?VANTAGE.Test.PostProcessing.Test_TOF})
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
        function pc = loadPcFile(obj,filename)
            ptCloud = pcread(filename);
            if nnz(~isnan(ptCloud.Location))==0
                pc = pointCloud(nan(1,3));
            else
                % Filter extraneous points
                pts = reshape(ptCloud.Location,ptCloud.Count,3);
                I = logical(prod(~isnan(pts),2));
                pc = pointCloud(pts(I,:));
            end
            if strcmpi(obj.ModelRef.Deployer.testScenario,'Modular') || strcmpi(obj.ModelRef.Deployer.testScenario,'100m')
                % Scale mm to m
                xyzPoints = pc.Location./1000;
                pc = pointCloud(xyzPoints);
            elseif strcmpi(obj.ModelRef.Deployer.testScenario,'simulation')
                % do nothing
            else
                error('testType is invalid, must be ''100m'', ''Modular'', or ''Simulation''')
            end
            if obj.showDebugPlots % useful for debugging
                figure            
                pcshow(pc)
                xlabel('x')
                ylabel('y')
                zlabel('z')
            end
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
                %warning('Bandwidth for point splitting ksdensity function did not converge in 30 tries')
                nsplit = 0;
                NPeaks = [];
                locs = [];
                break
              end
            end
            if obj.showDebugPlots % useful for debugging
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
            for i = 1:nSplit+1
                % Find indices of the current cubesat points in the point
                % cloud
                if i == 1
                    if nSplit > 0
                        I = pc.Location(:,3)>=locs(i);
                    else
                        I = ~isnan(pc.Location(:,3));
                    end
                elseif i == nSplit+1
                    I = pc.Location(:,3)<locs(nSplit);
                else
                    I = (pc.Location(:,3)>=locs(i)) & (pc.Location(:,3)<locs(i-1));
                end
                % Skip this CubeSat if it's too close to the edge of the
                % FOV
                maxZfov = max(...
                    max(abs(pc.Location(I,1)))/tand(obj.HVfov_deg(1)/2),...
                    max(abs(pc.Location(I,2)))/tand(obj.HVfov_deg(2)/2)...
                    );
                satPcZDistFromFOV = min(pc.Location(I,3)-abs(pc.Location(I,1))./tand(obj.HVfov_deg(1)/2),...
                                    pc.Location(I,3)-abs(pc.Location(I,2))./tand(obj.HVfov_deg(2)/2));
                % skip if first percentile of data is less than two cm from
                % the FOV --and-- if the 99th percentile of data is less
                % than 20 cm from the highest point of the FOV directly
                % below (-t3) the cubesat
                skipThisCubesat = (prctile(satPcZDistFromFOV,1) < 2/100) & ...
                                   (prctile(pc.Location(I,3),99)-maxZfov < 15/100);
                if skipThisCubesat
                    continue
                end
                CubeSats_TOF(i).pc = pointCloud(pc.Location(I,:));
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
        function [CubeSat_TOF] = fitPlanesToCubesat(obj,CubeSat_TOF)
            %%% Allocation
            % Initial value for minimum number of points considered to make up a plane
            minPtsInPlane   = 10;
            minPtsUnset     = true;

            % Initialize planes structure
            numPlanes = 0;
            planes = CubeSat_TOF.faces;
            
            % Tracking of points not associated with any plane
            remainPtCloud = CubeSat_TOF.pc;

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
                            numPlanes = numPlanes-1;
                            break
                            % if the previous plane is smaller than this plane
                        else
                            % replace previous plane with this plane and delete this plane
                            planes(i) = planes(numPlanes);
                            planes(numPlanes) = [];
                            numPlanes = numPlanes-1;
                            break
                        end
                    end
                end
                
                %%% Update minimum number points required to make a plane
                % Take minPoints to be one-twentieth the number of points found in the first
                % plane
                if numPlanes == 1 && minPtsUnset
                    minPtsInPlane = round(planes(numPlanes).planeCloud.Count/20);
                    minPtsUnset   = false;
                end
            end

            %%% Check that no more than three planes were identified
            if numPlanes > 3
              %error('fitPlanesToCubesat identified more than three planes for a single cubesat')
              CubeSat_TOF.trustedSat = 0;
            end

            %%% Check that planes are mutually orthogonal
            for i = 1:numPlanes
                for j = 1:numPlanes
                    if abs(asind(dot(planes(i).n,planes(j).n))) > 8 && i~=j
                        %CubeSat_TOF.trustedSat = 0;
                        %warning('fitPlanesToCubesat identified planes which are not mutually orthogonal by greater than 8 deg')
                    end
                end
            end
            
            %%% Package planes and numPlanes into CubeSat object
            CubeSat_TOF.faces = planes;
            CubeSat_TOF.numVisibleFaces = numPlanes;

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
            %%% Find plane boundary points and centroid
            [CubeSat_TOF.faces] = obj.getFaceCentroidAndCorners(CubeSat_TOF.faces);
            
            %%% Identify face corners using bounding box of face
            [CubeSat_TOF] = obj.diagnoseFace(CubeSat_TOF);
            
            %%% Extract data
            face = CubeSat_TOF.faces;
            
            %%% Find centroid of face in 3D
            if face.fullFace
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
                [~,I] = min(abs(face.trustedLen-norm(farCorners(:,1)-farCorners(:,2))));
                projLength = face.trustedLen([1:I-1,I+1:end]);
                
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
            d = obj.distInFromFace(CubeSat_TOF,face.faceIndex);
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
            %%% Check that planes intersect as expected
            if rank([CubeSat_TOF.faces.n]) ~= 2
                error('The two planes identified do not intersect as a line')
            end
            
            %%% Find faces' centroids and corners
            %%%     and determine if each face qualifies as a fullFace
            CubeSat_TOF.faces = obj.getFaceCentroidAndCorners(CubeSat_TOF.faces);
            CubeSat_TOF = obj.diagnoseFace(CubeSat_TOF);
            
            %%% Extract data
            faces = CubeSat_TOF.faces;
            
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
        % @param    CubeSat_TOF     CubeSat object to be analyzed
        %                       @see CubeSat
        %
        % @return   CubeSat object with calculated TCF centroid
        %           populated
        %
        % @author   Joshua Kirby
        % @date     03-Feb-2019
        function CubeSat_TOF = centroid3(obj,CubeSat_TOF)
            %%% Find faces' centroids and corners
            %%%     and determine if each face qualifies as a fullFace
            CubeSat_TOF.faces = obj.getFaceCentroidAndCorners(CubeSat_TOF.faces);
            CubeSat_TOF = obj.diagnoseFace(CubeSat_TOF);
            
            %%% Extract data
            faces = CubeSat_TOF.faces;
            
            %%% Find plane intersection
            A = zeros(3);
            b = zeros(3,1);
            for i = 1:3
                A(i,1:3) = faces(i).n;
                b(i,1)   = faces(i).n(1)*faces(i).o(1) + faces(i).n(2)*faces(i).o(2) + faces(i).n(3)*faces(i).o(3);
            end
            if rank(A) ~= 3
                error('Planes do not intersect in the expected way')
            end
            ptIntersect = (A\b);
            
            %%% Project into centroid from intersection point
            n = zeros(3,3);
            d = zeros(1,3);
            for i = 1:3
                % ensure that unit normals are inward facing
                n(:,i) = sign(dot(ptIntersect,faces(i).n))*faces(i).n;
                % calculate orthogonal distance from each face to the centroid
                d(i) = obj.distInFromFace(CubeSat_TOF,faces(i).faceIndex);
            end
            % Orthogonalize n
            [Q,R] = qr(n);
            n_orth = sign(diag(R))' .* Q;
            % Project to centroid
            CubeSat_TOF.centroid_TCF = ptIntersect + sum(d.*n_orth,2);
        end
        
        
        %% Subroutines
        %
        % Smooths a face boundary and returns the smoothed boundary and
        % face centroid
        %
        % @param    faces    CubeSat_TOF.faces struct
        % 
        % @return   struct with identified face centroids and corners
        %
        % @author   Joshua Kirby
        % @date     07-Mar-2019
        function [faces] = getFaceCentroidAndCorners(obj,faces)
            for ii = 1:length(faces)
                face = faces(ii);
                %%% Project points onto 2d plane
                plane = face.planeCloud;
                inPlane = double( (plane.Location-face(1).o')*face.V );

                %%% Find plane boundary points
                I_bound = boundary(inPlane(:,1),inPlane(:,2));

                %%% Find filtered boundary
                boundaryPtsRaw = [inPlane(I_bound,1)';inPlane(I_bound,2)'];
                order = 2;
                frameLen = obj.roundToNearestOdd(size(boundaryPtsRaw,2)/5);
                if frameLen <= 1
                    boundaryPts = boundaryPtsRaw;
                else
                    boundaryPts = sgolayfilt(boundaryPtsRaw,order,frameLen,[],2);
                end

                %%% Find face centroid
                faceShape = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
                [x,y] = centroid(faceShape);
                face.faceCentr = face.o + face.V * [x;y];

                % Find face boundary in face plane
                corners_inPlane = obj.minBoundingBox(boundaryPts);
                % Convert to 3D
                face.corners = zeros(3,4);
                for i = 1:4
                    face.corners(:,i) = face.o + face.V * corners_inPlane(:,i);
                end
                
                %%% repackage into faces
                faces(ii) = face;
            end
        end
        
        % 
        % Determines if a CubeSat.face struct is a full CubeSat face
        %
        % @params   CubeSat_TOF     CubeSat_TOF object with faces struct
        %                           already updated using
        %                           getFaceCentroidAndCorners
        %
        % @return   CubeSat_TOF with fullFace boolean, trusted side
        %           lengths, trusted corners, and face index
        %
        % @author   Joshua Kirby
        % @date     07-Mar-2019
        function [CubeSat_TOF] = diagnoseFace(obj,CubeSat_TOF)
            %%% for all faces
            for ii = 1:length(CubeSat_TOF.faces)
                % Extract data
                face = CubeSat_TOF.faces(ii);
                corners = face.corners;
                % Initialize boolean
                face.fullFace = 1;
                %%% Decide if full face is present or not
                % Determine if bounding box is near cubesat size
                len = zeros(1,2);
                deltaLen = zeros(3,2);
                faceMatches = cell(1,2);
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
                    face.fullFace = 0;
                end

                %%% Determine trusted lengths
                if face.fullFace
                    % trusted side lengths are visible side lengths
                    face.trustedLen = len;
                    % trusted corners are identified corners
                    face.trustedCorners = corners;
                else
                    % NOTE: this assumes that the CubeSat has not tumbled much
                    %   since launching, which is very valid for the TOF camera
                    % Determine trusted lengths
                    [trustedActualDimI,trustedLenI] = find(min(min(deltaLen))==deltaLen,1);
                    face.trustedLen = zeros(1,2);
                    face.trustedLen(1) = len(trustedLenI);
                    if trustedActualDimI ~= find(CubeSat_TOF.actualDims == max(CubeSat_TOF.actualDims))
                        face.trustedLen(2) = max(CubeSat_TOF.actualDims);
                    else
                        face.trustedLen(2) = min(CubeSat_TOF.actualDims);
                    end
                    %%% Determine trusted corners
                    % project to centroid from two most distant (downrange) corners
                    % Obtain distant two corners and their midPt     
                    if (mean(corners(3,:)) > 3) || (CubeSat_TOF.rangeOrder == 1)
                        sortDirection = 'ascend';
                    else
                        sortDirection = 'descend';
                    end
                    [~,I] = sort(face.corners(3,:),'descend');
                    farCorners = face.corners(:,I(1:2));
                    nearBadCornersUnordered = face.corners(:,I(3:4));

                    % Determine which trustedLen corresponds to the side that
                    % is orthogonal to corners
                    [~,I] = min(abs(face.trustedLen-norm(farCorners(:,1)-farCorners(:,2))));
                    projLength = face.trustedLen([1:I-1,I+1:end]);

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
                    face.trustedCorners = [fliplr(nearCorners),farCorners];
                end

                %%% Determine faceIndex
                % Dimensions to choose from, corresponding to indices of
                %   CubeSat.actualDims
                dims = 1:length(CubeSat_TOF.actualDims);
                % Calculate standard areas
                C = combnk(dims,2);
                Avec = prod(CubeSat_TOF.actualDims(C),2);
                % Calculate difference between area and standard areas
                faceArea = prod(face.trustedLen);
                dA = abs(faceArea-Avec);
                % Choose nearest size
                [~,face.faceIndex] = min(dA);
                
                %%% Repackage into CubeSat
                CubeSat_TOF.faces(ii) = face;
            end
            
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
        % @param    time            timestamp of this point cloud in global
        %                           VANTAGE time [s]
        %
        % @author   Joshua Kirby
        % @date     04-Mar-2019
        function plotResults(obj,CubeSats,CubeSats_TOF,pc,time,numConsOutliers)
            
            % Define indexing for use in looping over cubesats
            CubesatIndexing = 1:length(CubeSats);
            
            % Get truth data at the current time
            trueCentroids_VCF = zeros(obj.ModelRef.Truth_VCF.numCubeSats,3);
            for i = 1:obj.ModelRef.Truth_VCF.numCubeSats
                trueCentroids_VCF(i,:) = interp1(obj.ModelRef.Truth_VCF.t,...
                                                 obj.ModelRef.Truth_VCF.Cubesat(i).pos,...
                                                 time,[],'extrap');
            end
            
            % Transform true centroids from VCF to TCF
            trueCentroids_TCF = [obj.ModelRef.Transform.tf('TCF',trueCentroids_VCF','VCF')]';
            calcCentroids_TCF = nan(length(CubeSats),3);
            for i = 1:length(CubeSats) 
                if ~numConsOutliers(i) && ~isempty(CubeSats(i).centroids_VCF)
                    calcCentroids_TCF(i,:) = [obj.ModelRef.Transform.tf('TCF',CubeSats(i).centroids_VCF(:,end),'VCF')]';
                end
            end
            
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
            c = [c;c];
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
            xlabel('x [m]','fontsize',legendfontsize)
            ylabel('y [m]','fontsize',legendfontsize)
            zlabel('z [m]','fontsize',legendfontsize)
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
                    legendstrings{legendcounter} = ['CubeSat ',num2str(i)];
                    colorcounter = colorcounter+1;
                    legendcounter = legendcounter+1;
                end
            end
            plot3(xCenterline,yCenterline,zCenterline,'k--','linewidth',2)
            zlim(zlimits)
            ylim(ylimits)
            if ~isempty(legendstrings)
                l=legend(legendstrings,'location','eastoutside');
                l.FontSize = legendfontsize;
            end
            xlabel('x [m]','fontsize',legendfontsize)
            ylabel('y [m]','fontsize',legendfontsize)
            zlabel('z [m]','fontsize',legendfontsize)
            hold off
            
            subplot(1,3,3)
            hold on
            grid on
            grid minor
            title('Visible Faces Per CubeSat','fontsize',fontsize)
            legendstrings = [];
            legendcounter = 1;
            colorcounter = 1;
            centrColCounter = 1;
            for i = CubesatIndexing
                if ~any(isnan(calcCentroids_TCF(i,:)))
                    plot3(calcCentroids_TCF(i,1),calcCentroids_TCF(i,2),calcCentroids_TCF(i,3),'.','color',c(centrColCounter,:),'markersize',25)
                    legendstrings{legendcounter} = ['Calc.Centr',num2str(i)];
                    legendcounter = legendcounter+1;
                end
                plot3(trueCentroids_TCF(i,1),trueCentroids_TCF(i,2),trueCentroids_TCF(i,3),'b.','markersize',25)
                legendstrings{legendcounter} = ['True.Centr',num2str(i)];
                legendcounter = legendcounter+1;
                centrColCounter=centrColCounter + 1;
                for j = 1:CubeSats_TOF(i).numVisibleFaces
                    CubeSats_TOF(i).faces(j).planeCloud.Color = uint8(c(colorcounter,:).*255.*ones(CubeSats_TOF(i).faces(j).planeCloud.Count,3));
                    pcshow(CubeSats_TOF(i).faces(j).planeCloud,'markersize',markersize)
                    legendstrings{legendcounter} = ['Sat',num2str(i),'-Face',num2str(j)];
                    colorcounter = colorcounter+1;
                    legendcounter = legendcounter+1;
                end
            end
            plot3(xCenterline,yCenterline,zCenterline,'k--','linewidth',2)
            zlim(zlimits)
            ylim(ylimits)
            if ~isempty(legendstrings)
                l=legend(legendstrings,'location','eastoutside');
                l.FontSize = legendfontsize;
            end
            xlabel('x [m]','fontsize',legendfontsize)
            ylabel('y [m]','fontsize',legendfontsize)
            zlabel('z [m]','fontsize',legendfontsize)
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
        
        %
        % Calculate polynomial fit coefficients and outlierThresholds along
        % each axis for a given CubeSat object for the first N measurements
        % in the CubeSat
        %
        % @param    CubeSat     CubeSat class instance containing
        %                       centroids_VCF
        % @param    N           number of measurements to use
        %                       (centroids_VCF(:,1:N), inf is allowed
        %
        % @return   polynomial fit coefficients along x, size 1x2
        % @return   polynomial fit coefficients along y, size 1x2
        % @return   polynomial fit coefficients along z, size 1x2
        % @return   standard deviations along [x,y,z]'
        %
        % @author   Joshua Kirby
        % @date     21-Mar-2019
        function [pX,pY,pZ,stdXYZ] = fitLineToCentroids(~,CubeSat,N)
            % Evaluate N
            if N < inf
                if N > length(CubeSat.time)
                    error('More data points requested than are present')
                end
            elseif N <=0
                error('Invalid N requested')
            elseif isinf(N)
                N = length(CubeSat.time);
            else
                error('unhandled value for N received')
            end
            % Extract first N datapoints
            prevCentroids_VCF = CubeSat.centroids_VCF(:,1:N);
            prevTimes = CubeSat.time(1:N);
            % Fit to x, y, z data and determine standard deviations
            [pX,SX] = polyfit(prevTimes,prevCentroids_VCF(1,:),1);
            [~,stdX] = polyval(pX,prevTimes,SX);
            stdX = mean(stdX);
            [pY,SY] = polyfit(prevTimes,prevCentroids_VCF(2,:),1);
            [~,stdY] = polyval(pY,prevTimes,SY);
            stdY = mean(stdY);
            [pZ,SZ] = polyfit(prevTimes,prevCentroids_VCF(3,:),1);
            [~,stdZ] = polyval(pZ,prevTimes,SZ);
            stdZ = mean(stdZ);
            % Determine if current point is near a prediction or is
            % an outlier
            stdXYZ = [stdX stdY stdZ]';
        end
        
        %
        % Produces final 3D fit model to all TOF centroids calculated for a
        % given CubeSats
        %
        % @param    CubeSat     CubeSat class instance containing
        %                       time and centroids_VCF
        %
        % @return   1x3 (x y z) cell array of fit objects, each of which is a
        %           function of time
        %
        % @author   Joshua Kirby
        % @date     24-Mar-2019
        function TOFfit_VCF = produceFinalTOFfit(~,CubeSat)
            TOFfit_VCF{1} = fit(CubeSat.time',CubeSat.centroids_VCF(1,:)','poly1');
            TOFfit_VCF{2} = fit(CubeSat.time',CubeSat.centroids_VCF(2,:)','poly1');
            TOFfit_VCF{3} = fit(CubeSat.time',CubeSat.centroids_VCF(3,:)','poly1');
        end
        
        
        
    end
    
    
    
end % classdef