function pos = centroid1(ptCloud,planes,u)

    % Project points onto 2d plane
    plane = planes.planeCloud;
    inPlane = double( (plane.Location-planes.o)*planes.V );
    
    % Find plane boundary points
    I_bound = boundary(inPlane(:,1),inPlane(:,2));
    
    %{
    m = (max(dr)-min(dr)).*0.5;
    [~,locs] = findpeaks(dr,'MinPeakHeight',m);
    findpeaks(dr,'MinPeakHeight',m)
    %}

    % Find area centroid of boundary
    face = polyshape(inPlane(I_bound,1),inPlane(I_bound,2),'simplify',false);
    
    % Find corners of face
    %{
    th = zeros(size(face.Vertices,1),1);
    for i = 3:size(face.Vertices,1)
        P0 = face.Vertices(i-2,:);
        P1 = face.Vertices(i-1,:);
        P2 = face.Vertices(i,:);
        
        n1 = (P2 - P0) / norm(P2 - P0);
        n2 = (P1 - P0) / norm(P1 - P0);
        
        th(i) = atan2(norm(det([n2; n1])), dot(n1, n2));
    end
    [~,locs] = findpeaks(th,'MinPeakHeight',0.15);
    locs = locs-1;
    
    face = polyshape(face.Vertices(locs,1),face.Vertices(locs,2));
    %}
    % Find face area centroid
    [x,y] = centroid(face);
    
    % Convert area centroid to 3d point
    outPlane = planes.o + (x*planes.V(:,1))' + (y*planes.V(:,2))';
    
    % Project inward to centroid
    d = distInFromFace(face,u);
    pos = outPlane + d.*(planes.n)';
    
    %{
    close all
    figure
    pcshow(ptCloud)
    hold on
    plot3(pos(1),pos(2),pos(3),'ko')
    plot3(0,0,0,'k*')
    pause(1)
    %}
end