function ptNew = loadToF(filename)

    ptCloud = pcread(filename);

    % Filter extraneous points
    tmp = sum(abs(ptCloud.Location),2)~=0;
    ptCloud.Count - prod(tmp)
    ptNew = pointCloud(ptCloud.Location(tmp,:));
    
    %
    figure
    pcshow(ptNew)
    hold on
    plot3(0,0,0,'k*')
    grid on
    %}

end