%% Main menu
clc

while 1
% Housekeeping
clearvars -except path

list = { 'Import Truth Data', 'Plot Truth Data', 'Import VANTAGE Data', ...
    'Plot VANTAGE Data', 'Plot Test Performance', ...
    'Correlate Multiple Tests' };
answer = listdlg( 'PromptString', 'Choose Action', 'ListString', ...
    list, 'SelectionMode', 'Single' );

% answer will be one of the following: 1,2,3,4,5,6,[]
% each number corresponds to the choice selected
% [] indicates closed out or cancelled

if isempty( answer ) % canceled/closed out
    disp( 'Exiting.' )
    return
end

disp( [ list{answer}, ' selected.' ] )

if answer == 6
    warning( 'Not supported yet.' )
    % probably redirect to some correlation function/wrapper
    % ehem... //TODO
    continue
end

% 1-4 has been selected
m = msgbox( 'Select Test Folder.' );
waitfor(m)

if ~exist( 'path', 'var' )
    path = '';
end

path = uigetdir( path );

if ~path
    disp( 'Cancelling.' )
    clearvars path
    continue
end

disp( [ path, ' selected as testing folder.' ] )

truth_name = [ path, '/truth.json' ];

% now is the time to separate out choices
% 1: Import Truth Data
% 2: Plot Truth Data
% 3: Import VANTAGE Data
% 4: Plot VANTAGE Data
% 5: Plot Test Performance
switch answer
    case 1
        % check for pre-expisting truth file
        if exist( truth_name, 'file' )
            warning( 'Truth file already exists.' )
            answer = questdlg( [ 'Truth file already exists. ', ...
                'Delete it and continue?' ], 'WARNING', 'Yes', 'No', ...
                'No' );
            switch answer
                case 'Yes'
                    delete(truth_name)
                    disp( [ truth_name, ' deleted.' ] )
                otherwise
                    disp( [ truth_name, ' not deleted.' ] )
                    disp( 'Import Truth Data canceled.' )
                    continue
            end
        end
        
        % import raw truth data
        [ t, x, y, z, date_str ] = importTruthData( path );
        
        % if import was cancelled
        if isempty(t)
            disp( 'Import Truth Data canceled.' )
            continue
        end
        
        % import correction
        if exist( [ path, '/corrections.m' ], 'file' )
            run( [ path, '/corrections.m' ] )
            % this loads the data in correction.m:
            % a struct called "correction" with the fields:
            % - nSats: the number of CubeSats
            % - t: The time correction to the truth data
            % - x(i): the X correction for cubesat i
            % - y(i): the Y correction for cubesat i
            % - z(i): the Z correction for cubesat i
            % correction.x/y/z are nSats long
            
        else % correction.m file does not exist
            warning( 'No corrections file found.' )
            answer = questdlg( [ 'No corrections file found. ', ...
                'Import truth data as 1 CubeSat with no corrections?' ],...
                'WARNING', 'Yes', 'No', 'Yes' );
            
            switch answer
                case 'Yes'
                    disp( [ 'Continuing with 1 CubeSat and no ', ...
                        'corrections to Truth Data.' ] )
                    correction.nSats = 1;
                    correction.t = 0;
                    correction.x = 0;
                    correction.y = 0;
                    correction.z = 0;
                    
                case 'No'
                    disp( 'Import Truth Data canceled.' )
                    continue
                    
                otherwise
                    disp( [ '¯\\_(', char(12471), ...
                        ')_/¯ guess I''ll die...' ] )
                    error( [ 'Tell Marshall his code is jank and how ', ...
                        'you got here.' ] )
                    
            end
        end
        
        % generate truth struct
        t = t + correction.t;
        
        for i = 1 : length(t)
            truth(i).t = t(i);
            for j = 1 : correction.nSats
                truth(i).pos.( [ 'CubeSat_', num2str(j) ] ) = ...
                    [ -( y(i) + correction.y(j) ); ...
                    -( z(i) + correction.z(j) ); ...
                    x(i) + correction.x(j) ];
            end
        end
        
        % generate truth file
        fID = fopen( truth_name, 'w' );
        jason = jsonencode( { date_str, truth } );
        fprintf( fID, jason );
        fclose(fID);
        disp( 'Import Truth Data complete.' )
        
    case 2
        
        % check for truth file existance
        if ~exist( truth_name, 'file' )
            warning( 'Truth file does not exist.' )
            w = warndlg( 'Truth file does not exist.' );
            waitfor(w)
            disp( 'Plot Truth Data cancelled.' )
            continue
        end
        
        % extract truth data
        truth = jsondecode( fileread( truth_name ) );
        date_str = truth{1};
        truth = truth{2};
        % number of datapoints
        for i = 1 : length( truth )
            % number of CubeSats
            t(i) = truth(i).t;
            for j = 1 : length( truth(1).pos )
                x{j}(i) = truth(i).pos.( [ 'CubeSat_', num2str(j) ] )(1);
                y{j}(i) = truth(i).pos.( [ 'CubeSat_', num2str(j) ] )(2);
                z{j}(i) = truth(i).pos.( [ 'CubeSat_', num2str(j) ] )(3);
            end
        end
        
        f = figure;
        for i = 1 : length( truth(1).pos )
            plotTruthData( t, x{i}, y{i}, z{i} )
        end
        
        waitfor(f)
        disp( 'Plot Truth Data complete.' )
        
        % It would probably behoove me to figure out a way of either
        % labelling all nSat lines in this figure, plotting nSat figures,
        % or something else to make looking at all nSat sats reasonably
        % understandable.
        
    case 3
        warning( 'Not supported yet.' )
        
    case 4
        warning( 'Not supported yet.' )
        
    case 5
        warning( 'Not supported yet.' )
        
    otherwise
        disp( [ '¯\\_(', char(12471), ')_/¯ guess I''ll die...' ] )
        error( 'Tell Marshall his code is jank and how you got here.' )
        
end

end