function pos = centroid2(ptCloud,planes,u)

    % Calculate position for each plane
    pos = zeros(2,3);
    for i = 1:2
        pos(i,:) = centroid1(ptCloud,planes(i),u);
    end
    
    % Average position estimates
    pos = sum(pos,1)./2;

end