function [ptNew] = loadTestData(filepath)

    % Load data
    ptCloud = pcread(filepath);

    if sum(sum(ptCloud.Location==0))==0
        ptNew = struct('Count',0);
    else
        % Filter extraneous points
        pts = ptCloud.Location;
        I = logical(prod(~isnan(pts),2));
        ptNew = pointCloud([pts(I,1:2),-pts(I,3)]);
    end


end