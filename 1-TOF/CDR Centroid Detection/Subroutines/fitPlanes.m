function [planes,numPlanes] = fitPlanes(ptCloud)

    % Initialize fit parameters
    maxDistance = 0.001;
    
    % Initialize planes structure
    numPlanes = 3;
    planes(3) = struct('o',[],'n',[],'V',[],'planeCloud',[]);

    % Fit plane to data
    [~,inlierIndices,outlierIndices] = pcfitplane(ptCloud,...
                maxDistance);
            
    if isempty(inlierIndices)
        numPlanes = 0;
        return;
    end
    plane = select(ptCloud,inlierIndices);
    
    % Remove noisy data
    plane = pcdenoise(plane);
    
    % Store point cloud of plane in output
    planes(1).planeCloud = plane;

    % Fit sfit plane to point cloud plane
    %planes(1).p = fit( double([plane.Location(:,1), plane.Location(:,2)]), double(plane.Location(:,3)), 'poly11');
    [planes(1).n,planes(1).V,planes(1).o] = affine_fit(plane.Location);
    
    for i = 2:3
        
        if isempty(outlierIndices) || numel(outlierIndices)<=250
            numPlanes = i-1;
            break;
        end
        
        % Store remaining points
        remainPtCloud = select(ptCloud,outlierIndices);
        
        dx = max(remainPtCloud.Location(:,1)) - min(remainPtCloud.Location(:,1));
        dy = max(remainPtCloud.Location(:,2)) - min(remainPtCloud.Location(:,2));
        dz = max(remainPtCloud.Location(:,3)) - min(remainPtCloud.Location(:,3));
        if sum([dx dy dz]>=0.003)<=2
            numPlanes = i-1;
            break;
        end
        
        % Fit plane to data
        [~,inlierIndices,outlierIndices] = pcfitplane(ptCloud,...
                    maxDistance);
        plane = select(ptCloud,inlierIndices);
        
        % Remove noisy data
        plane = pcdenoise(plane);
        
        % Store point cloud of plane in output
        planes(i).planeCloud = plane;

        % Store remaining points
        remainPtCloud = select(ptCloud,outlierIndices);

        % Fit sfit plane to point cloud plane
        %planes(i).p = fit( double([plane.Location(:,1), plane.Location(:,2)]), double(plane.Location(:,3)), 'poly11');
        [planes(i).n,planes(i).V,planes(i).o] = affine_fit(plane.Location);
        
    end
    
    

end