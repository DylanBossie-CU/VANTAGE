clear
TF = VANTAGE.Transform.Transform;

% Frame 1
F1 = eye(3);

% Rotation
xang = deg2rad(30); % deg
yang = deg2rad(-10);
zang = deg2rad(45);
C = angle2dcm(zang,yang,xang,'ZYX');

% Frame 2
F2 = C*F1;

% frame 1 to 2 vector
V = [2 2 2]';

% Frame 1 vector
v1 = [pi,-pi,2*pi]';

% Frame 2 vector
v2 = C*(v1-V);
TF = TF.setTdata('F2',C,V,'F1');
v2 = TF.tf('F2',v1,'F1');