clearvars;close all;clc

TOF = VANTAGE.PostProcessing.TOF;
pc = TOF.loadSimFile(['Data/Prototyping Data/Simulation Data/Tube 2/',...
                 '30FPS_Tube_2_3U_2U_1U_VX_0_VY_0_VZ_001_WX_0_WY_0_WZ_0_SEP_0P03_F00010.pcd']);
             
TRUTH(i).
CubeSats = TOF.cubesatPointsFromPC(pc);

for i = 1:length(CubeSats)
    CubeSats(i) = TOF.fitPlanesToCubesat(CubeSats(i));
    CubeSats(i) = TOF.findCentroidFromFaces(CubeSats(i));
end

figure
c = get(gca,'colororder');

subplot(1,3,1)
pcshow(pc,'markersize',15)
title('Raw Pointcloud','fontsize',16)

subplot(1,3,2)
grid on
grid minor
title('Identified CubeSats','fontsize',16)
hold on
colorcounter = 1;
for i = 1:length(CubeSats)
    CubeSats(i).pc.Color = uint8(c(colorcounter,:).*255.*ones(CubeSats(i).pc.Count,3));
    pcshow(CubeSats(i).pc,'markersize',15)
    legendStrings{i} = ['CubeSat ',num2str(i)];
    colorcounter = colorcounter+1;
end
l=legend(legendStrings,'location','southeast');
l.FontSize = 14;
hold off

subplot(1,3,3)
hold on
grid on
grid minor
title('Visible Faces Per CubeSat','fontsize',16)
legendStrings = [];
legendcounter = 1;
colorcounter = 1;
for i = 1:length(CubeSats)
    for j = 1:CubeSats(i).numVisibleFaces
        CubeSats(i).faces(j).planeCloud.Color = uint8(c(colorcounter,:).*255.*ones(CubeSats(i).faces(j).planeCloud.Count,3));
        pcshow(CubeSats(i).faces(j).planeCloud,'markersize',15)
        legendStrings{legendcounter} = ['Sat',num2str(i),'-Face',num2str(j)];
        colorcounter = colorcounter+1;
        legendcounter = legendcounter+1;
    end
end
l=legend(legendStrings,'location','southeast');
l.FontSize = 14;
hold off