%% VICON Error Characterization
try load('VICON_err.mat')
    
catch me
    
    filename = './Data/VICON Error_2.csv';

    m_to_ft = 3.28084; % ft m^-1

    pos = 0:25; % ft

    n = 26;

    err = cell(n,1);
    [ sis_err, sig_err ] = deal( zeros(n,1) );

    for i = 1 : n

    fprintf( [ num2str(pos(i)/m_to_ft), ' m\n' ] );

    [ ~, p, ~, ~ ] = importTruthData( filename, 1 );

    err{i} = pos(i) - p .* m_to_ft;

    sis_err(i) = mean( err{i} );

    sig_err(i) = std( err{i} );

    end
    
    err_str.err = err;
    err_str.sis_err = sis_err;
    err_str.sig_err = sig_err;
    err_str.pos = pos;
    
    save( 'VICON_err.mat', err_str )
    
end

clearvars;
close all;
clc;

m_to_ft = 3.28084; % ft m^-1
load( 'VICON_err.mat' )

err_full = [];
g = [];

for i = 1 : length( err_str.err )
    
    err_full = [ err_full; err_str.err{i} ];
    g = [ g; err_str.pos(i) .* ones( length( err_str.err{i} ), 1 ) ];
    
end

figure;
boxplot( err_full / m_to_ft * 100, g )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Error [cm]' )
grid on
grid minor

saveMeSomeFigs(0,'./Figures/VICON_Error_characterization_1.pdf',gcf)

figure;

subplot(1,2,1)
boxplot( err_full( g <= 17 ) / m_to_ft * 100, g( g <= 17 ) )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Error [cm]' )
grid on
grid minor

subplot(1,2,2)
boxplot( err_full( g > 17 ) / m_to_ft * 100, g( g > 17 ) )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Error [cm]' )
grid on
grid minor

saveMeSomeFigs(0,'./Figures/VICON_Error_characterization_2.pdf',gcf)

figure;

subplot(1,2,1)
plot( err_str.pos, err_str.sis_err / m_to_ft * 100, '*k' )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Systematic Error [cm]' )
grid on
grid minor

subplot(1,2,2)
plot( err_str.pos, err_str.sig_err / m_to_ft * 100, '*k' )
hold on
fill( [ min( err_str.pos ), max( err_str.pos ), max( err_str.pos ), ...
    min( err_str.pos ) ], [ 0, 0, std( err_str.sig_err )*100, ...
    std( err_str.sig_err )*100 ], 'b', 'FaceAlpha', 0.25, ...
    'EdgeAlpha', 0.5 )
fill( [ min( err_str.pos ), max( err_str.pos ), max( err_str.pos ), ...
    min( err_str.pos ) ], [ 0, 0, ...
    std(err_str.sig_err([1:23,25:end]))*100, ...
    std(err_str.sig_err([1:23,25:end]))*100 ], 'g', 'FaceAlpha', 0.25, ...
    'EdgeAlpha', 0.5 )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Random Error Standard Deviation [cm]' )
grid on
grid minor

saveMeSomeFigs(0,'./Figures/VICON_Error_characterization_3.pdf',gcf)

%% GPS Error Characterization
[ ~, x, y, z ] = importTruthData( './Data/120C.txt' );

x = x - mean(x);
y = y - mean(y);
z = z - mean(z);

x = x.*1000;
y = y.*1000;
z = z.*1000;

figure;

subplot(1,2,1)
hold on
plot(x,y,'.k')

% Shoutout to my homie Tobin!
% Calculate covariance matrix
P = cov( x, y );
mean_x = mean(x);
mean_y = mean(y);
% Calculate the define the error ellipses
n = 100; % Number of points around ellipse
p = 0 : pi / n : 2 * pi; % angles around a circle
[ eigvec, eigval ] = eig(P); % Compute eigen-stuff
xy_vect = [ cos(p'), sin(p') ] * sqrt(eigval) * eigvec'; % Transformation
x_vect = xy_vect(:,1);
y_vect = xy_vect(:,2);
% Plot the error ellipses overlaid on the same figure
plot( x_vect+mean_x, y_vect+mean_y, 'b', 'LineWidth', 2 )
plot( 2 * x_vect+mean_x, 2 * y_vect+mean_y, 'g', 'LineWidth', 2 )
plot( 3 * x_vect+mean_x, 3 * y_vect+mean_y, 'r', 'LineWidth', 2 )

xlabel( 'East Distance [mm]' )
ylabel( 'North Distance [mm]' )
grid on
grid minor

subplot(1,2,2)
hold on
histfit(z,15)
xlabel( 'Zenith Distance [mm]' )
ylabel( 'Counts' )
grid on
grid minor

saveMeSomeFigs(0,'./Figures/GPS_Error_characterization_1.pdf',gcf)

%% VICON Test Expected Data
[ t, x, y, z ] = importTruthData( './Data/24Feb_Rack2_Test1.csv' );

% Rotation onto principal, horizontal, vertical axes
% Principal axis is the one which minimizes mean(H) and mean(Z),
% I.E. it is the axis that points along the average position
% Horizontal is the axis perpendicular to Principal with no z
% component
% Vertical is the remaining axis perpendiculat to both Principal
% and Horizontal
p = [x-x(1),y-y(1),0.*z];
p = mean(p);
p = p / norm(p);
h = cross( [0,0,1], p );
k = cross( p, h );

p = dot( [x,y,z], p.*ones( size( [x,y,z] ) ), 2 );
h = dot( [x,y,z], h.*ones( size( [x,y,z] ) ), 2 );
k = dot( [x,y,z], k.*ones( size( [x,y,z] ) ), 2 );

figure;

subplot(1,2,1)
plot(t,p,'-k','Linewidth',2)
xlabel( 'Time [s]' )
ylabel( 'Principal Direction [m]' )
grid on
grid minor

subplot(1,2,2)
scatter(100.*h,100.*k,1,t-t(1))
xlabel( 'Horizontal Direction [cm]' )
ylabel( 'Vertical Direction [cm]' )
c = colorbar('southoutside');
caxis( [ 0, t(end)-t(1) ] )
c.Label.String = 'Time [s]';
colormap(fliplr(jet));
grid on
grid minor

saveMeSomeFigs(0,'./Figures/VICON_Expected_Data_1.pdf',gcf)

figure;

subplot(1,3,1)
plot( t, gradient(p,t), '-k' )
xlabel( 'Time [s]' )
ylabel( 'Principal Velocity [m/s]' )

subplot(1,3,2)
histfit( 100.*gradient(h,t), 15 )
xlabel( 'Horizontal Velocity [cm/s]' )
ylabel( 'Counts' )

subplot(1,3,3)
histfit( 100.*gradient(k,t), 15 )
xlabel( 'Vertical Velocity [cm/s]' )
ylabel( 'Counts' )

saveMeSomeFigs(0,'./Figures/VICON_Expected_Data_2.pdf',gcf)

%% GPS Test Expected Data
try load( 'GPSExpected.mat' )
catch me
    [ t, x, y, z ] = importTruthData( './Data/600.txt', 1 );
    save( 'GPSExpected.mat', 't', 'x', 'y', 'z' )
end

% Rotation onto principal, horizontal, vertical axes
% Principal axis is the one which minimizes mean(H) and mean(Z),
% I.E. it is the axis that points along the average position
% Horizontal is the axis perpendicular to Principal with no z
% component
% Vertical is the remaining axis perpendiculat to both Principal
% and Horizontal
p = [x,y,0.*z];
p = mean(p);
p = p / norm(p);
h = cross( [0,0,1], p );
k = cross( p, h );

p = dot( [x,y,z], p.*ones( size( [x,y,z] ) ), 2 );
h = dot( [x,y,z], h.*ones( size( [x,y,z] ) ), 2 );
k = dot( [x,y,z], k.*ones( size( [x,y,z] ) ), 2 );

load( 'GPSExpected.mat' )

f = fit(p-p(1),k-k(1),'smoothingspline','SmoothingParam',0.07);

figure;
subplot(1,2,1)
plot( p-p(1), feval( f, p-p(1) ), '-k', 'Linewidth', 2 )
hold on
scatter(p-p(1),k-k(1),1,t-t(1))
xlabel( 'Principal Direction [m]' )
ylabel( 'Vertical Direction [m]' )
c = colorbar('southoutside');
caxis( [ 0, t(end)-t(1) ] )
c.Label.String = 'Time [s]';
colormap(fliplr(jet));
grid on
grid minor

subplot(1,2,2)
histfit(100.*h,15)
hold on
plot( std(100.*h).*[1,1], [0,max(get(gca,'YLim'))], '-r', 'LineWidth', 2 )
plot( -std(100.*h).*[1,1], [0,max(get(gca,'YLim'))], '-r', 'LineWidth', 2 )
xlabel( 'Horizontal Direction [cm]' )
ylabel( 'Counts' )
grid on
grid minor

saveMeSomeFigs(0,'./Figures/GPS_Expected_Data_1.pdf',gcf)

figure;

subplot(1,3,1)
plot( t, gradient(p,t), '.k' )
hold on
plot( [ t(1), t(end) ], mean( gradient(p,t) ) .* [1,1], '-g', ...
    'LineWidth', 2 )
xlabel( 'Time [s]' )
ylabel( 'Principal Velocity [m/s]' )
grid on
grid minor

subplot(1,3,2)
histfit( 100.*gradient(h,t), 15 )
xlabel( 'Horizontal Velocity [cm/s]' )
ylabel( 'Counts' )
grid on
grid minor

subplot(1,3,3)
histfit( 100.*gradient(k,t), 15 )
xlabel( 'Vertical Velocity [cm/s]' )
ylabel( 'Counts' )
grid on
grid minor

saveMeSomeFigs(0,'./Figures/GPS_Expected_Data_2.pdf',gcf)