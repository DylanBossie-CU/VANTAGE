%%  Author: Joshua Kirby
%  Created: 11/24/2018
% Modified: 11/24/2018
%
% Purpose: 
%
% Inputs:
%
% Outputs:
%
function u = findCubesatSize(fileNum,cubeSatNum,SET)
%% Determine u from truth data
u = SET.TRUTH(fileNum).u(cubeSatNum);
    
    
    
end


% THIS CODE ASSUMES THAT CUBESATS ARE NOT TUMBLING, AND THEREFORE IS NOT
% USED

% % Initialize standard cubesat lengths
% uz = [0.1 0.2 0.3]; % m
% 
% %dx = max(ptCloud.Location(:,1)) - min(ptCloud.Location(:,1));
% %dy = max(ptCloud.Location(:,2)) - min(ptCloud.Location(:,2));
% dz = max(ptCloud.Location(:,3)) - min(ptCloud.Location(:,3));
% 
% % Calculate maximum diagonal
% %maxDiag = sqrt(dx.^2 + dy.^2 + dz.^2);
% 
% % Estimate cubesat size
% [~,u] = min(abs(uz-dz));