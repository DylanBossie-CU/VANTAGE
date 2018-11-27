%%  Author: Justin Fay
%  Created: 11/26/2018
% Modified: 11/26/2018
%
% fusionCentroid
%
% Assumptions: 
%   We're assuming that the centroid lies on the plane normal to the camera
%   line. This could potentially be updated in the future if it makes more
%   sense to do something else
% 
% Purpose:
%   This function calculates an error-weighted centroid based on the
%   vector output from the VANTAGE camera processing and the position
%   output from the VANTAGE time of flight processing.
%
% Inputs:
%   camOrigin - origin point of camera
%   camVec    - vector from camera to visual cetroid of cubesat
%   pos_TOF   - position output from time of flight processing
%   sig_cam   - uncertainty in camera centroid
%   sig_TOF   - uncertainty in TOF centroid
% Outputs:
%   pos - centroid position of cubesat after sensor fusion
% Notes:
%   sig_cam and sig_TOF should use the same units or be a weighted score
%   for the uncertainties.

function [pos] = fusionCentroid(camOrigin, camVec, pos_TOF, sig_cam, sig_TOF, pos_truth)

    % Normalize camera vector
    camVec = camVec./norm(camVec);

    % Define line vector
    a = camOrigin - camVec;
    
    % Define vector from line to TOF point
    b = pos_TOF - camVec;
    
    % Calculate vector cross product
    vec = cross(a,b);
    
    % Calculate distance along line for weigthed center point
    d = norm(vec) / norm(a);
    if sig_TOF~=0
        q = sig_TOF./(sig_cam+sig_TOF);
    else
        q = 0;
    end
    d_q = d.*q;
    
    % Calculate position for camera
    mag_TOF = norm(pos_TOF);
    mag_cam = sqrt(mag_TOF.^2 - d.^2);
    pos_cam = camOrigin + mag_cam.*camVec;
    
    % Calculate error-weighted centroid position
    posDir = pos_cam - pos_TOF; posDir = posDir./norm(posDir);
    pos = pos_TOF + posDir.*d_q;
    
    % Plotting
    %{
    figure
    aVec = [camOrigin;camVec];
    bVec = [pos_TOF;camVec];
    cam = [camOrigin;camVec];
    posVec = [pos_TOF;pos_cam];
    plot3(camOrigin(1),camOrigin(2),camOrigin(3),'r*')
    hold on
    plot3(cam(:,1),cam(:,2),cam(:,3),'r-')
    plot3(posVec(1,1),posVec(1,2),posVec(1,3),'bo')
    plot3(posVec(2,1),posVec(2,2),posVec(2,3),'ro')
    plot3(posVec(:,1),posVec(:,2),posVec(:,3),'m--')
    plot3(pos(1),pos(2),pos(3),'k*')
    plot3(pos_truth(1),pos_truth(2),pos_truth(3),'g*')
    axis equal
    grid on
    title('Sensor Fusion Visualization')
    xlabel('x (m)'); ylabel('y (m)'); zlabel('z (m)')
    legend('Camera origin','Camera vector','Position from TOF','Position from camera','Shortest distance line','Error-weighted centroid','Truth data')
    %}   
    
end