function pos = centroid1(ptCloud,planes,u)

    planes = planes(1);

    % Project points onto 2d plane
    plane = planes.planeCloud;
    inPlane = double(plane.Location*planes.V);
    
    % Find plane boundary points
    I_bound = boundary(inPlane(:,1),inPlane(:,2));

    % Find centroid of boundary
    face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2));
    [x,y] = centroid(face);
    
    outPlane = [x,y]*planes.V';
    
end