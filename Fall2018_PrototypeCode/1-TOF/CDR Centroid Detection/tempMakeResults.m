% direc = 'Simulation Data/Tube 6/';
% ls = dir(direc);
% for i = 1:length(ls)
%   if contains(ls(i).name,'FPS_Tube')
%     str = ls(i).name;
%     j = strfind(ls(i).name,'VZ_0P1');
%     str(j:j+5) = 'VZ_001';
%     movefile([direc ls(i).name],[direc str]);
%   end
% end


figure
hold on
grid on 
grid minor
[p1,inlier,outlier] = pcfitplane(ptCloud,0.005);
rem = select(ptCloud,outlier);
[p2,inlier,outlier] = pcfitplane(rem,0.005);
%xlim([0.323612987995148   0.445630013942719])
%ylim([-0.194181993603706  -0.079263001680374])
%zlim([1.225940942764282   1.989722013473511])
pcshow(ptCloud,'markersize',75)
c = get(gca,'colororder');
h = plot(p1);
h.FaceColor = c(1,:);
h = plot(p2);
h.FaceColor = c(1,:);
xlabel('x')
ylabel('y')
zlabel('z')
title({'Planes Fitted to 3U CubeSat'})
hold off