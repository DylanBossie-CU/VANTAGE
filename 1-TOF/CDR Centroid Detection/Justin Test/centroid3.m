function pos = centroid3(ptCloud,planes,u)

    % Find plane intersection
    C = [planes(1).n; planes(2).n; planes(3).p];
    px = C(:,1);
    py = C(:,2);
    pz = C(:,3);
    A = [px,py,pz];
    b = [0;0;0];
    ptIntersect = (A\b)';
    
    % Find volumetric centroid
    pos = ptIntersect + 0.5.*[0.1 -0.1 u.*0.1];
    
end