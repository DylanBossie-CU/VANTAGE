
% Variable cleanup
clearvars

% Load data
filename = './Test Data/ZGap_Near/Ctube_05.ply';
%filename = './Test Data/Separation/DRSep_2_in.ply';
ptCloud = pcread(filename);

% Filter extraneous points
tmp1 = sum(abs(ptCloud.Location),2)~=0;
tmp2 = ptCloud.Location(:,1)<=-0.08;
tmp3 = ptCloud.Location(:,2)<0.18;
tmp = logical(tmp1.*tmp2.*tmp3);
ptNew = pointCloud(ptCloud.Location(tmp,:));
clearvars tmp1 tmp2 tmp3 tmp

%[ptNew] = ptRot(ptNew,[0 0 0 pi/5 pi/8 0],1);

%% z plane
% Identify plane normal to z
maxDistance = 0.008;

[model1,inlierIndices,outlierIndices] = pcfitplane(ptNew,...
            maxDistance);
plane1 = select(ptNew,inlierIndices);
plane1 = pcdenoise(plane1);
remainPtCloud = select(ptNew,outlierIndices);

% Identify plane normal
p1 = fit( double([plane1.Location(:,1), plane1.Location(:,2)]), double(plane1.Location(:,3)), 'poly11');


%% y plane
% Identify plane normal to y
[model2,inlierIndices,outlierIndices] = pcfitplane(remainPtCloud,...
            maxDistance);
plane2 = select(remainPtCloud,inlierIndices);
plane2 = pcdenoise(plane2);
remainPtCloud = select(remainPtCloud,outlierIndices);

% Identify plane normal
p2 = fit( double([plane2.Location(:,1), plane2.Location(:,2)]), double(plane2.Location(:,3)), 'poly11');

%% x plane
% Identify plane normal to x
maxDistance = 0.008;
referenceVector = [1,0,0];
maxAngularDistance = 5;

[model3,inlierIndices,outlierIndices] = pcfitplane(remainPtCloud,...
            maxDistance);
plane3 = select(remainPtCloud,inlierIndices);
plane3 = pcdenoise(plane3);
remainPtCloud = select(remainPtCloud,outlierIndices);

% Find plane boundary and center
p3 = fit( double([plane3.Location(:,1), plane3.Location(:,2)]), double(plane3.Location(:,3)), 'poly11');

%% Plane intersection
C = [coeffvalues(p1);coeffvalues(p2);coeffvalues(p3)];
p00 = C(:,1);
p10 = C(:,2);
p01 = C(:,3);
A = [p10,p01,-ones(3,1)];
b = -p00;

ptIntersect = (A\b)';

%% Plotting

figure
h(1) = plot(p1);
h(1).LineStyle = 'none';
h(1).CData = ones(size(h(1).CData)).*0.0;
hold on
h(2) = plot(p2);
h(2).LineStyle = 'none';
h(2).CData = ones(size(h(1).CData)).*1.0;
h(3) = plot(p3);
h(3).LineStyle = 'none';
h(3).CData = ones(size(h(3).CData)).*0.65;

plot3(0,0,0,'k*')
plot3(ptNew.Location(:,1),ptNew.Location(:,2),ptNew.Location(:,3),'r.','Markersize',2)
plot3(ptIntersect(:,1),ptIntersect(:,2),ptIntersect(:,3),'k.','Markersize',50)

axis equal
xlim([-0.3 0])
ylim([0 0.25])
zlim([0.1 0.8])
xlabel('x')
ylabel('y')
zlabel('z')