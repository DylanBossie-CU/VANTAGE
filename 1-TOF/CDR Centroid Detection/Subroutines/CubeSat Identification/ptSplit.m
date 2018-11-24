function [cubesats] = ptSplit(ptCloud)

    % Pull out z values
    z = sort(ptCloud.Location(:,3));
    
    %% Start the loop
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
    
  %     figure
  %     plot(zBin,-zDense);
  %     xlabel('z (m)')
  %     ylabel('Percent point density')
  
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
end