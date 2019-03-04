clearvars;close all;clc

truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube1/config_simulation_template_2_25_Josh_ToF_Calibration_tube1_truth_data.json';
manifestFilename = 'config/Manifest.json';
Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename);
Deployer = Model.Deployer;
SensorData = jsondecode(fileread('config/Testing/TOF/Sensors_TOFdev.json'));
TOF = VANTAGE.PostProcessing.TOF('config/TOF.json',Model);
Deployer = TOF.TOFProcessing(SensorData,Deployer,'fileLims',[40,40]);

%%
if 0
Deployer = VANTAGE.PostProcessing.Deployer('config/Manifest.json');
CubeSats = Deployer.CubesatArray;
TOF = VANTAGE.PostProcessing.TOF;
pc = TOF.loadSimFile(['Data/Prototyping Data/Simulation Data/Tube 2/',...
                 '30FPS_Tube_2_3U_2U_1U_VX_0_VY_0_VZ_001_WX_0_WY_0_WZ_0_SEP_0P03_F00050.pcd']);
            
[TOF,CubeSats] = TOF.cubesatPointsFromPC(pc,CubeSats);

CubesatIndexing = 1:TOF.currNumSatsVisible;
for i = CubesatIndexing
    CubeSats(i) = TOF.fitPlanesToCubesat(CubeSats(i));
    CubeSats(i) = TOF.findCentroidFromFaces(CubeSats(i));
end

figure
c = get(gca,'colororder');
markersize = 100;
fontsize = 20;
legendfontsize = 16;

subplot(1,3,1)
pcshow(pc,'markersize',markersize)
title('Raw Pointcloud','fontsize',fontsize)

subplot(1,3,2)
grid on
grid minor
title('Identified CubeSats','fontsize',fontsize)
hold on
colorcounter = 1;
legendcounter = 1;
for i = CubesatIndexing
    CubeSats(i).pc.Color = uint8(c(colorcounter,:).*255.*ones(CubeSats(i).pc.Count,3));
    pcshow(CubeSats(i).pc,'markersize',markersize)
    legendStrings{legendcounter} = ['CubeSat ',num2str(i)];
    colorcounter = colorcounter+1;
    legendcounter = legendcounter+1;
end
l=legend(legendStrings,'location','eastoutside');
l.FontSize = legendfontsize;
hold off

subplot(1,3,3)
hold on
grid on
grid minor
title({'Visible Faces','Per CubeSat'},'fontsize',fontsize)
legendStrings = [];
legendcounter = 1;
colorcounter = 1;
for i = CubesatIndexing
    plot3(CubeSats(i).centroid_TCF(1),CubeSats(i).centroid_TCF(2),CubeSats(i).centroid_TCF(3),'r.','markersize',25)
    for j = 1:CubeSats(i).numVisibleFaces
        CubeSats(i).faces(j).planeCloud.Color = uint8(c(colorcounter,:).*255.*ones(CubeSats(i).faces(j).planeCloud.Count,3));
        pcshow(CubeSats(i).faces(j).planeCloud,'markersize',markersize)
        legendStrings{legendcounter} = ['Sat',num2str(i),'Centroid'];
        legendStrings{legendcounter+1} = ['Sat',num2str(i),'-Face',num2str(j)];
        colorcounter = colorcounter+1;
        legendcounter = legendcounter+2;
    end
end
l=legend(legendStrings,'location','eastoutside');
l.FontSize = legendfontsize;
hold off


end