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

figure;

subplot(1,2,1)
boxplot( err_full( g <= 17 ) / m_to_ft * 100, g( g <= 17 ) )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Error [cm]' )

subplot(1,2,2)
boxplot( err_full( g > 17 ) / m_to_ft * 100, g( g > 17 ) )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Error [cm]' )

figure;

subplot(1,2,1)
plot( err_str.pos, err_str.sis_err / m_to_ft * 100, '*k' )
xlabel( 'Distance from Origin [ft]' )
ylabel( 'Systematic Error [cm]' )

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