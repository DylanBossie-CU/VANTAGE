inch_to_m = 0.0254; % m in^-1
% number of CubeSats
correction.nSats = 3;
% global time correction
correction.t = 0; % s
% global boom corrections
boom_to_VANTAGE_X = -2.5 * inch_to_m; % m
boom_to_VANTAGE_Y = (28+5/8) * inch_to_m; % m
boom_to_VANTAGE_Z = -(66.75-62-7/8) * inch_to_m; % m
VANTAGE_to_VCF_X = 3 / 1000; % m
VANTAGE_to_VCF_Y = 32.498 / 1000; % m
VANTAGE_to_VCF_Z = -32.498 / 1000; % m
globalX = boom_to_VANTAGE_X + VANTAGE_to_VCF_X; % m
globalY = boom_to_VANTAGE_Y + VANTAGE_to_VCF_Y; % m
globalZ = boom_to_VANTAGE_Z + VANTAGE_to_VCF_Z; % m
% CubeSat 1 corrections
correction.x(1) = (-16.375+4.375/2) * inch_to_m + globalX; % m
correction.y(1) = 0 + globalY; % m
correction.z(1) = 0 + globalZ; % m
% CubeSat 2 corrections
correction.x(2) = (-16.375-18.25+9+4.375/2) * inch_to_m + globalX; % m
correction.y(2) = 0 + globalY; % m
correction.z(2) = 0 + globalZ; % m
% CubeSat 3 corrections
correction.x(3) = (-16.375-18.25+4.375/2) * inch_to_m + globalX; % m
correction.y(3) = 0 + globalY; % m
correction.z(3) = 0 + globalZ; % m