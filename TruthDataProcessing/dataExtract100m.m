%% Housekeeping
clearvars;
close all;
clc;

%% Data Importing
msg = msgbox( 'Select the folder holding test folders.' );
waitfor(msg)
path = uigetdir;

