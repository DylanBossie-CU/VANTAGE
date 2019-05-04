function [ t, x, y, z ] = importVICONData( filename )

framerate = 100; % Hz

data = xlsread( filename );

% crop out header
data = data(5:end,:);

t = data(:,1) / framerate;
% note x and y are switched for VICON data
y = data(:,6) / 1000;
x = data(:,7) / 1000;
z = data(:,8) / 1000;

t = t - t(1);
x = x - x(1);
y = y - y(1);
z = z - z(1);

end