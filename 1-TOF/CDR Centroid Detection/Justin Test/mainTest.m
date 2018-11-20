
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
for i = 16:numFrame

    % Load point cloud
    ptCloud = loadSimToF(strcat(direc,data(i).name));

    % If good frame
    if ptCloud.Count>200 && min(ptCloud.Location(:,3))<=3
        
        % Split into seperate cubesat point clouds
        [cubesats] = ptSplit(ptCloud);
        
        % Loop through cubesats
        for j = 1:numel(cubesats)
            
            % Find size of cubesat
            centroids(j).u(i) = findCubesatSize(cubesats(j).ptCloud);
            
            % Fit planes to point cloud
            [planes,numPlanes] = fitPlanes(cubesats(j).ptCloud);
            
            if numPlanes==0
                break; 
            end
            
            % Choose centroid determination method
            u = mode( centroids(j).u(centroids(j).u~=0) );
            if numPlanes==1
                centroids(j).pos(i,:) = centroid1(cubesats(j).ptCloud,planes,u);
            elseif numPlanes==2
                
            elseif numPlanes==3
                centroids(j).pos(i,:) = centroid3(cubesats(j).ptCloud,planes,u);
            end
        end
    end
    
end

