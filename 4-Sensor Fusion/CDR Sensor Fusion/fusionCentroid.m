function [pos] = fusionCentroid(camOrigin, camVec, pos_TOF, sig_cam, sig_TOF)

    % Define line vector
    a = camOrigin - camVec;
    
    % Define vector from line to TOF point
    b = pos_TOF - camVec;
    
    % Calculate vector cross product
    vec = cross(a,b);
    
    % Calculate position for camera
    pos_cam = pos_TOF-cross(vec,a);
    
    % Calculate distance along line for weigthed center point
    d = norm(vec) / norm(a);
    q = sig_TOF./(sig_cam+sig_TOF);
    d_q = d.*q;
    
    % Calculate error-weighted centroid position
    pos = pos_TOF - cross(vec,a).*d_q./d;
    
    % Plotting
    aVec = [camOrigin;camVec];
    bVec = [pos_TOF;camVec];
    cam = [camOrigin;camVec];
    posVec = [pos_TOF;pos_cam];
    figure
    plot3(camOrigin(1),camOrigin(2),camOrigin(3),'r*')
    hold on
    plot3(cam(:,1),cam(:,2),cam(:,3),'r-')
    plot3(posVec(1,1),posVec(1,2),posVec(1,3),'bo')
    plot3(posVec(2,1),posVec(2,2),posVec(2,3),'ro')
    plot3(posVec(:,1),posVec(:,2),posVec(:,3),'m--')
    plot3(pos(1),pos(2),pos(3),'k*')
    axis equal
    grid on
    title('Sensor Fusion Visualization')
    xlabel('x (m)'); ylabel('y (m)'); zlabel('z (m)')
    legend('Camera origin','Camera vector','Position from TOF','Position from camera','Shortest distance line','Error-weighted centroid')
end