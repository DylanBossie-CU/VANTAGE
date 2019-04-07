function livePlot( t, x, y, z )
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Author: Marshall Herr
%%%
%%% Purpose: Takes in x,y,z data and a time vector and plots the x,y,z
%%% points in real time
%%%
%%% Inputs:
%%%     - t: Time vector
%%%     - x: X position vector
%%%     - y: Y position vector
%%%     - z: Z position vector
%%%
%%% Date Created: 21 Feb 2019
%%% Last Editted: 21 Feb 2019
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

global pace;

pace = 1;
buttonWidth = 0.1;
buttonHeight = 0.05;

f = figure();
a = axes( 'Parent', f );
colormap(fliplr(jet))
% Add pushbutton to view data
stopButton = uicontrol( 'Parent', f, 'Style', 'pushbutton', ...
    'String', 'Pause', 'Units', 'normalized', ...
    'Position', [0.99 - buttonWidth, 0.01, buttonWidth, buttonHeight], ...
    'Callback', @stopButtonPressed, 'Visible', 'on' );

colors = jet(length(t));
angles = [ 45, 45 ];

i = 1;

tic;
pauseTime = 0;

while ishandle(f) && ( i <= length(t) )
    
    scatter3( x(1:i), y(1:i), z(1:i), 1, colors(end:-1:end-i+1,:) )
    title( [ 'Current Time: ', num2str( floor( t(i)/60 ) ), ':', ...
        num2str( t(i) - 60*floor( t(i)/60 ) ) ] )
    xlabel('East [m]')
    ylabel('North [m]')
    zlabel('Zenith [m]')
    c = colorbar( 'EastOutside' );
    c.Label.String = 'Time [s]';
    caxis( [ min(t), max(t) ] );
    view( angles )
    daspect([1,1,1]);
    pbaspect([1,1,1]);
    drawnow;
    
    while ~pace && ishandle(f)
        
        angles = get( a, 'View' );
        pause(0.01);
        pauseTime = toc - t(i);
        
    end
    
    [ ~, i ] = min( abs( t - toc + pauseTime ) );
    
end

end

function stopButtonPressed(scr,event)

global pace;

if strcmpi( scr.String, 'pause' )
    scr.String = 'Continue';
    pace = 0;
else
    scr.String = 'Pause';
    pace = 1;
end

end