
% Open data directory
direc = './Simulation Data/ToF/';
data = dir(direc);
data = data(3:end);
numFrame = numel(data);

% Initialize centroid data
centroids(6) = struct('pos',nan(numFrame,3),'u',zeros(numFrame,1));
for i = 1:5
    centroids(i) = struct('pos',nan(numFrame,3),'u',zeros(numFrame,1));
end

% Loop through frames
for i = 1:numFrame
    tic
    fprintf('Frame %i ',i)
    
    % Load point cloud
    ptCloud = loadSimToF(strcat(direc,data(i).name));
    %ptCloud = loadTestData('./Test Data/Separation/DRSep_1_in.ply');
    
    % If good frame
    if ptCloud.Count>200 && min(ptCloud.Location(:,3))<=3
        fprintf('(good) ')
        
        % Split into seperate cubesat point clouds
        [cubesats] = ptSplit(ptCloud);
        fprintf('found %i cubesats\n',numel(cubesats))
        
        % Loop through cubesats
        for j = 1:numel(cubesats)
            fprintf('\tCubesat %i ',j)
            
            % Find size of cubesat
            centroids(j).u(i) = findCubesatSize(cubesats(j).ptCloud);
            
            % Fit planes to point cloud
            [planes,numPlanes] = fitPlanes(cubesats(j).ptCloud);
            fprintf('found %i planes\n',numPlanes)
            
            if numPlanes==0
                break;
            end
            
            % Choose centroid determination method
            u = mode( centroids(j).u(centroids(j).u~=0) );
            if numPlanes==1
                centroids(j).pos(i,:) = centroid1(cubesats(j).ptCloud,planes(1:numPlanes),u);
            elseif numPlanes==2
                centroids(j).pos(i,:) = centroid2(cubesats(j).ptCloud,planes(1:numPlanes),u);
            elseif numPlanes==3
                centroids(j).pos(i,:) = centroid3(cubesats(j).ptCloud,planes(1:numPlanes),u);
            end
        end
    end
    fprintf('\t')
    toc
end

