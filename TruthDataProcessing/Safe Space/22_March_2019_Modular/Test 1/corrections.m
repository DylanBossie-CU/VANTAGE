inch_to_m = 0.0254; % m in^-1
% number of CubeSats
correction.nSats = 3;
% global time correction
correction.t = 0; % s

% Ball (b#) positions from end of kabob (e)
Y_e_b1 = 0; % m
Y_e_b2 = 0; % m
Y_e_b3 = 0; % m
Y_e_b4 = 0; % m
Y_e_b5 = 0; % m
X_e_b1 = (7+3/8) * inch_to_m; % m
X_e_b2 = (15+7/8) * inch_to_m; % m
X_e_b3 = (22.75+1+1/8) * inch_to_m; % m
X_e_b4 = (22.75+6.5) * inch_to_m; % m
X_e_b5 = (22.75+13) * inch_to_m; % m
Z_e_b1 = (-0.75) * inch_to_m; % m
Z_e_b2 = (-0.75) * inch_to_m; % m
Z_e_b3 = (-0.75) * inch_to_m; % m
Z_e_b4 = (-0.75) * inch_to_m; % m
Z_e_b5 = (-0.75) * inch_to_m; % m

% VICON kabob object location (b) from end of kabob(e)
Y_e_b = (Y_e_b1+Y_e_b2+Y_e_b3+Y_e_b4+Y_e_b5)/5; % m
X_e_b = (X_e_b1+X_e_b2+X_e_b3+X_e_b4+X_e_b5)/5; % m
Z_e_b = (Z_e_b1+Z_e_b2+Z_e_b3+Z_e_b4+Z_e_b5)/5; % m

% CubeSat (c#) positions from end of kabob (e)
Y_e_c1 = 0; % m
Y_e_c2 = 0; % m
Y_e_c3 = 0; % m
X_e_c1 = (4.25/2) * inch_to_m; % m
X_e_c2 = (9.5+4.25/2) * inch_to_m; % m
X_e_c3 = (18.75+4.25/2) * inch_to_m; % m
Z_e_c1 = 0; % m
Z_e_c2 = 0; % m
Z_e_c3 = 0; % m

% CubeSat (c#) positions from VICON kabob object location (b)
Y_b_c1 = Y_e_c1 - Y_e_b; % m
Y_b_c2 = Y_e_c2 - Y_e_b; % m
Y_b_c3 = Y_e_c3 - Y_e_b; % m
X_b_c1 = X_e_c1 - X_e_b; % m
X_b_c2 = X_e_c2 - X_e_b; % m
X_b_c3 = X_e_c3 - X_e_b; % m
Z_b_c1 = Z_e_c1 - Z_e_b; % m
Z_b_c2 = Z_e_c2 - Z_e_b; % m
Z_b_c3 = Z_e_c3 - Z_e_b; % m

% VICON kabob object location (b) from Origin (o)
% by not normalizing the position data this is auto-encorporated
Y_o_b = 0; % m
X_o_b = 0; % m
Z_o_b = 0; % m

% VANTAGE (v) position from Origin (o)
data = dir( './*Camera_Pos.csv' );
data = xlsread( [ './', data.name ] );
% crop out header
data = data(5:end,:);
Y_o_v = mean( data(:,7) / 1000 ) + 69/1000; % m
X_o_v = mean( data(:,6) / 1000 ); % m
Z_o_v = mean( data(:,8) / 1000 ) - 72/1000; % m

% VICON kabob object location (b) from VANTAGE (v)
Y_v_b = Y_o_b - Y_o_v; % m
X_v_b = X_o_b - X_o_v; % m
Z_v_b = Z_o_b - Z_o_v; % m

% CubeSat (c#) location from VANTAGE (v)
Y_v_c1 = Y_b_c1 + Y_v_b; % m
Y_v_c2 = Y_b_c1 + Y_v_b; % m
Y_v_c3 = Y_b_c1 + Y_v_b; % m
X_v_c1 = X_b_c1 + X_v_b; % m
X_v_c2 = X_b_c2 + X_v_b; % m
X_v_c3 = X_b_c3 + X_v_b; % m
Z_v_c1 = Z_b_c1 + Z_v_b; % m
Z_v_c2 = Z_b_c2 + Z_v_b; % m
Z_v_c3 = Z_b_c3 + Z_v_b; % m

% CubeSat 1 corrections
correction.x(1) = X_v_c1; % m
correction.y(1) = Y_v_c1; % m
correction.z(1) = Z_v_c1; % m
% CubeSat 2 corrections
correction.x(2) = X_v_c2; % m
correction.y(2) = Y_v_c2; % m
correction.z(2) = Z_v_c2; % m
% CubeSat 3 corrections
correction.x(3) = X_v_c3; % m
correction.y(3) = Y_v_c3; % m
correction.z(3) = Z_v_c3; % m