function plotTruthData( t, p, h, k )
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Author: Marshall Herr
%%%
%%% Purpose: Takes in a time vector and position vectors and shows these in
%%% terms of the principal direction of travel, the horizontal direction,
%%% and the "vertical" direction
%%%
%%% Inputs:
%%%     - t: Time vector in seconds
%%%     - x: East position from origin in meters
%%%     - y: North position from origin in meters
%%%     - z: Zenith position from origin in meters
%%%
%%% Date Created: 21 Feb 2019
%%% Last Editted: 21 Feb 2019
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Formatting
numBoxes = 100;

% Standard Deviations and Means
sP = std(p);
mP = mean(p);
sH = std(h);
mH = mean(h);
sK = std(k);
mK = mean(k);

% Plotting
ax = subplot(3,2,1);
plot(t,p)
hold on
plot(t,0.*p+mP,'-g')
plot(t,0.*p+mP+sP,'-r')
plot(t,0.*p+mP-sP,'-r')
ylabel('Principal Position [m]')
title('Position VS Time')
axis tight

subplot(3,2,2);
histfit(p,numBoxes)
y_lim = get(gca,'YLim');
hold on
plot([mP,mP],[y_lim(1),y_lim(2)],'-g')
plot([mP-sP,mP-sP],[y_lim(1),y_lim(2)],'-r')
plot([mP+sP,mP+sP],[y_lim(1),y_lim(2)],'-r')
ylabel('Principal Position Counts')
title('Position Histograms')
ylim(y_lim)

ay = subplot(3,2,3);
plot(t,h)
hold on
plot(t,0.*h+mH,'-g')
plot(t,0.*h+mH+sH,'-r')
plot(t,0.*h+mH-sH,'-r')
ylabel('Horizontal Position [m]')
axis tight

subplot(3,2,4);
histfit(h,numBoxes)
y_lim = get(gca,'YLim');
hold on
plot([mH,mH],[y_lim(1),y_lim(2)],'-g')
plot([mH-sH,mH-sH],[y_lim(1),y_lim(2)],'-r')
plot([mH+sH,mH+sH],[y_lim(1),y_lim(2)],'-r')
ylabel('Horizontal Position Counts')
ylim(y_lim)

az = subplot(3,2,5);
plot(t,k)
hold on
plot(t,0.*k+mK,'-g')
plot(t,0.*k+mK+sK,'-r')
plot(t,0.*k+mK-sK,'-r')
ylabel('Vertical Position [m]')
xlabel('Time [s]')
axis tight

subplot(3,2,6);
histfit(k,numBoxes)
y_lim = get(gca,'YLim');
hold on
plot([mK,mK],[y_lim(1),y_lim(2)],'-g')
plot([mK-sK,mK-sK],[y_lim(1),y_lim(2)],'-r')
plot([mK+sK,mK+sK],[y_lim(1),y_lim(2)],'-r')
ylabel('Vertical Position Counts')
xlabel('Position [m]')
ylim(y_lim)

linkaxes([ax,ay,az],'x')

end