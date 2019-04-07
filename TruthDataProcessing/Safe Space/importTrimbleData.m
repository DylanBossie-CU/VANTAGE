function [ t, x, y, z, u, v, w ] = ...
    importTrimbleData( filepath, delimiter, headerRowNumber )
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Author: Marshall Herr
%%%
%%% Purpose: Takes in a path to a Trimble GPS text file and outputs the
%%% position and velocity data along with the time vector.
%%%
%%% Inputs:
%%%     - filepath: The pagth to the *.txt file from the current directory
%%%     - delimiter: Optional, specify the delimiter, defualt is ' '
%%%     - headerRowNumber: Optional, specify what line of the text file
%%%         contains the header, default is 1
%%%
%%% Outputs:
%%%     - t: Time vector in seconds
%%%     - x: East position from origin in meters
%%%     - y: North position from origin in meters
%%%     - z: Zenith position from origin in meters
%%%     - u: Eastward velocity from origin in meters per second
%%%     - v: Northward velocity from origin in meters per second
%%%     - w: Zenith-ward velocity from origin in meters per second
%%%
%%% Date Created: 21 Feb 2019
%%% Last Editted: 21 Feb 2019
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Note: datafiles are created in 10 minute blocks (E.G. from 1:00 to 1:10),
% thus the day cannot change during a dataset, nor the hour, nor the month,
% year, etc. If this happens in your dataset, this code will be bugged!!!

%% Setup

minutesToSeconds = 60 / 1; % s min^-1

if nargin < 3
    headerRowNumber = 1;
end
if nargin < 2
    delimiter = ' ';
end

% This will exclude data points more that 'sigmaExclude' standard
% deviations away from the median of x, y, and z
sigmaExclude = 10;

%% Data Extraction

% Export data from text file
dataStruct = importdata( filepath, delimiter, headerRowNumber );

% Export position/velocity data from struct
data = dataStruct.data;

% Export timetamps as chars
chars = dataStruct.textdata;

% Chop 1st data point because it is always wrong
t = chars( headerRowNumber + 2 : end, 4 );
x = data(2:end,1);
y = data(2:end,2);
z = data(2:end,3);
u = data(2:end,4);
v = data(2:end,5);
w = data(2:end,6);

%% Data Cleanup
while isinf( std(x) )
    
    badData = abs( x - median(x) ) == max( abs( x - median(x) ) );
    
    t = t( ~badData );
    x = x( ~badData );
    y = y( ~badData );
    z = z( ~badData );
    u = u( ~badData );
    v = v( ~badData );
    w = w( ~badData );
    
end

while isinf( std(y) )
    
    badData = abs( y - median(y) ) == max( abs( y - median(y) ) );
    
    t = t( ~badData );
    x = x( ~badData );
    y = y( ~badData );
    z = z( ~badData );
    u = u( ~badData );
    v = v( ~badData );
    w = w( ~badData );
    
end

while isinf( std(z) )
    
    badData = abs( z - median(z) ) == max( abs( z - median(z) ) );
    
    t = t( ~badData );
    x = x( ~badData );
    y = y( ~badData );
    z = z( ~badData );
    u = u( ~badData );
    v = v( ~badData );
    w = w( ~badData );
    
end

badData = ( abs( x - median(x) ) > sigmaExclude * std(x) ) | ...
          ( abs( y - median(y) ) > sigmaExclude * std(y) ) | ...
          ( abs( z - median(z) ) > sigmaExclude * std(z) );

while ~isempty( find( badData, 1 ) )
    
    t = t( ~badData );
    x = x( ~badData );
    y = y( ~badData );
    z = z( ~badData );
    u = u( ~badData );
    v = v( ~badData );
    w = w( ~badData );
    
    badData = ( abs( x - median(x) ) > sigmaExclude * std(x) ) | ...
              ( abs( y - median(y) ) > sigmaExclude * std(y) ) | ...
              ( abs( z - median(z) ) > sigmaExclude * std(z) );
      
end

% Transform hr:min:sec cells into seconds
t = cell2mat(t);
[ ~, ~, ~, ~, Minute, Second ] = datevec( t );
t = Second + minutesToSeconds * Minute;

t = t - t(1);
x = x - x(1);
y = y - y(1);
z = z - z(1);

end