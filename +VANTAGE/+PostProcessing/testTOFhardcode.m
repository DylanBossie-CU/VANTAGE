clearvars;close all;clc

TOF = VANTAGE.PostProcessing.TOF;
pc = TOF.loadSimFile(['Data/Prototyping Data/Simulation Data/Tube 6/',...
                 '30FPS_Tube_6_3U_2U_1U_VX_0_VY_0_VZ_001_WX1U_',...
                 '0p1_WY1U_0p1_WZ1U_0p1_SEP_0P03_F00040.pcd']);
             
CubeSats = TOF.cubesatPointsFromPC(pc);

figure
subplot(1,2,1)
pcshow(pc,'markersize',15)
title('Raw Pointcloud')
subplot(1,2,2)
grid on
grid minor
title('Identified CubeSats')
hold on
CubeSats(1).pc.Color = uint8([255 0 0].*ones(CubeSats(1).pc.Count,3));
CubeSats(2).pc.Color = uint8([0 0 255].*ones(CubeSats(2).pc.Count,3));
pcshow(CubeSats(1).pc,'markersize',15)
pcshow(CubeSats(2).pc,'markersize',15)
legend('CubeSat 1','CubeSat 2','location','southeast')
hold off