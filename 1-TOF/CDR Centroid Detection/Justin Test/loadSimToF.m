function ptNew = loadSimToF(filename)

    ptCloud = pcread(filename);

    if sum(sum(sum(~isnan(ptCloud.Location))))==0
        ptNew = struct('Count',0);
    else     
        % Filter extraneous points
        pts = reshape(ptCloud.Location,ptCloud.Count,3);
        I = logical(prod(~isnan(pts),2));
        ptNew = pointCloud([pts(I,1:2),-pts(I,3)]);
    end
    
    %{
    figure
    pcshow(ptNew)
    hold on
    plot3(0,0,0,'k*')
    grid on
    axis equal
    %}

end