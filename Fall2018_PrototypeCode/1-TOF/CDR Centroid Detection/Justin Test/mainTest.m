
% Open data directory
direc = './Simulation Data/ToF/';
data = dir(direc);
data = data(3:end);
numFrame = numel(data);

% Initialize centroid data
centroids(6) = struct('pos',nan(numFrame,3),'u',zeros(numFrame,1),'uExpect',NaN);
for i = 1:5
    centroids(i) = struct('pos',nan(numFrame,3),'u',zeros(numFrame,1),'uExpect',i);
end
centroids(5).uExpect = NaN;
centroids(4).uExpect = NaN;
prevNumCubesats = 0;

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
        [cubesats,prevNumCubesats] = ptSplit(ptCloud,prevNumCubesats);
        fprintf('found %i cubesats\n',prevNumCubesats)
        
        % Loop through cubesats
        for j = 1:prevNumCubesats
            fprintf('\tCubesat %i ',j)
            
            if ~isempty(cubesats(j).ptCloud)
                
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
                    centroids(j).pos(i,:) = centroid1(cubesats(j).ptCloud,planes(1:numPlanes),centroids(j).uExpect);
                elseif numPlanes==2
                    centroids(j).pos(i,:) = centroid2(cubesats(j).ptCloud,planes(1:numPlanes),centroids(j).uExpect);
                elseif numPlanes==3
                    centroids(j).pos(i,:) = centroid3(cubesats(j).ptCloud,planes(1:numPlanes),centroids(j).uExpect);
                end
            end
        end
    end
    fprintf('\t')
    toc
end

