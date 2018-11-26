%%  Author: Joshua Kirby
%  Created: 11/11/2018
% Modified: 11/11/2018
%
% Purpose: Produces a unitvector from an input vector.  Handy no?
%
% Inputs:
%   vec  - vector
%   
% Outputs:
%   uvec - unit vector
function [uvec] = unitvec(vec)
%% Produce unit vector
uvec = vec./norm(vec);
end