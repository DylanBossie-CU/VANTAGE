clearvars;close all;clc

TOF = VANTAGE.PostProcessing.TOF;
pc = TOF.loadSimFile(['Data/Prototyping Data/Simulation Data/Tube 6/',...
                 '30FPS_Tube_6_3U_2U_1U_VX_0_VY_0_VZ_001_WX1U_',...
                 '0p1_WY1U_0p1_WZ1U_0p1_SEP_0P03_F00040.pcd']);
             
CubeSats = TOF.cubesatPointsFromPC(pc);

figure
pcshow(CubeSats(1).pc)

figure
pcshow(CubeSats(2).pc)