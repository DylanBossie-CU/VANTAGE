function [ r, err ] = AnalyzeTest( Truth, Test )
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Author: Marshall Herr
%%%
%%% Purpose: To analyse the performace of the VANTAGE system in a test
%%%
%%% Inputs:
%%%     - Truth: A struct containing the following truth data:
%%%         > t: A time vector corresponding to the x, y, z data
%%%         > x: A cell containing vectors corresponding to the x component
%%%             of the position vector for each CubeSat: x{i} = x position
%%%             of CubeSat i
%%%         > y: A cell containing vectors corresponding to the y component
%%%             of the position vector for each CubeSat: y{i} = y position
%%%             of CubeSat i
%%%         > z: A cell containing vectors corresponding to the z component
%%%             of the position vector for each CubeSat: z{i} = z position
%%%             of CubeSat i
%%%
%%%     - Test: A struct containing the following VANTAGE data:
%%%         > t: A time vector corresponding to the x, y, z data
%%%         > x: A cell containing vectors corresponding to the x component
%%%             of the position vector for each CubeSat: x{i} = x position
%%%             of CubeSat i
%%%         > y: A cell containing vectors corresponding to the y component
%%%             of the position vector for each CubeSat: y{i} = y position
%%%             of CubeSat i
%%%         > z: A cell containing vectors corresponding to the z component
%%%             of the position vector for each CubeSat: z{i} = z position
%%%             of CubeSat i
%%%
%%% Output:
%%%     - r: A range vector
%%%     - err: The error at each range data point in r
%%%     - Graphs detailing the performance of the system
%%%
%%% Date Created: 24 March 2019
%%% Last Editted: 24 March 2019
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



end