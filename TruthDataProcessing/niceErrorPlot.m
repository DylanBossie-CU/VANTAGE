function niceErrorPlot( r, mu, sigma )
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Author: Marshall Herr
%%%
%%% Purpose: To make nice plots of the error vs range of VANTAGE
%%%
%%% Inputs:
%%%     - r: A vector of the range of each datapoint
%%%     - mu: The mean error at each range given in r
%%%     - sigma: The standard deviation of the error at each range given in
%%%         r
%%%
%%% Outputs:
%%%     - A cool plot
%%%
%%% Date Created: 22 March 2019
%%%
%%% Last Editted: 22 March 2019
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% % Testing
% r = linspace(0,100,100);
% mu = r.^2 ./ 100;
% sigma = r.^(1.5) ./ 50 + 1;

n = 1000;
s = 10;
[ x, y1, y2, c1 ] = deal( zeros( n, length(r) ) );

for i = 1 : length(r)
    
    idx = ( (i-1)*n + 1 ) : ( i*n );
    x(idx) = r(i) .* ones(1,n)';
    y1(idx) = mu(i)+linspace(0,s*sigma(i),n)';
    y2(idx) = mu(i)-linspace(0,s*sigma(i),n)';
    c1(idx) = exp( -linspace(0,s*sigma(i),n) ./ sigma(i) );
    
end

hold on
p1 = surf( x, y1, 0.*x, 'FaceAlpha', 'flat', ...
    'AlphaDataMapping', 'scaled', 'AlphaData', c1, 'LineStyle', 'None' );
surf( x, y2, 0.*x, 'FaceAlpha', 'flat', 'AlphaDataMapping', 'scaled', ...
    'AlphaData', c1, 'LineStyle', 'None' )
p2 = plot( r, mu, '-k', 'LineWidth', 2 );
p3 = plot( r, mu+sigma, '-r', 'LineWidth', 2 );
ylim( [ min(mu-3.*sigma), max(mu+3.*sigma) ] )
legend( [ p1, p2, p3 ], ...
    { 'Likelyhood of Error', 'Mean Error', '1\sigma Upper Error Bound' }...
    , 'location', 'best' )

end