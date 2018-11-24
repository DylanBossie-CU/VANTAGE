function [cubesats,numCubesats] = ptSplit(ptCloud,prevNumCubesats)

    % Pull out z values
    z = sort(ptCloud.Location(:,3));
    
    % Calculate k-squares density
    [zDense,zBin] = ksdensity(z);
    
    %{
    figure
    plot(zBin,-zDense);
    xlabel('z (m)')
    ylabel('Percent point density')
    %}
    
    % Identify split locations
    c = 0.05;
    %{
    findpeaks(-zDense,zBin,'MinPeakProminence',c,'NPeaks',3);
    xlim([0 3])
    pause(1)
    %}
    [~,locs] = findpeaks(-zDense,zBin,'MinPeakProminence',c,'NPeaks',2);
    nSplit = numel(locs);
    
    % Separate point cloud by split planes
    cubesats = struct('ptCloud',[]);
    if nSplit>0
        for i = 1:nSplit
            if i==1
                I = ptCloud.Location(:,3)<=locs(i);
                cubesats(i).ptCloud = pointCloud(ptCloud.Location(I,:));
            else
                I = ptCloud.Location(:,3)<=locs(i);
                cubesats(i).ptCloud = pointCloud(ptCloud.Location(I,:));
                I = cubesats(i).ptCloud.Location(:,3)>locs(i-1);
                cubesats(i).ptCloud = pointCloud(cubesats(i).ptCloud.Location(I,:));
            end
        end
        I = ptCloud.Location(:,3)>locs(nSplit);
        cubesats(nSplit+1).ptCloud = pointCloud(ptCloud.Location(I,:));
    else
        cubesats.ptCloud = ptCloud; 
    end
    
    % Adjust for losing sight of cubesats
    numCubesats = numel(cubesats);
    if numCubesats<prevNumCubesats
        cubesats (numCubesats:prevNumCubesats) = struct('ptCloud',[]);
        numCubesats = prevNumCubesats;
    end
    cubesats = flipud(cubesats);
    
end