close all

% Frame 1
F1 = eye(3);

% Rotation
xang = deg2rad(30); % deg
yang = deg2rad(-10);
zang = deg2rad(45);
C = angle2dcm(zang,yang,xang,'ZYX');

% Frame 2
F2 = C*F1;

% frame 1 to 2 vector
V = [2 2 2]';

% Frame 1 vector
v1 = [-3 4 1]';

% Frame 2 vector
v2 = C*(v1-V);

% plot
lims = [-5 5];
figure
hold on
grid on
plot3(0,0,0,'k.','markersize',10)
% Frame 1
plot3([0 F1(1,1)],[0 F1(1,2)],[0 F1(1,3)],'color','red')
plot3([0 F1(2,1)],[0 F1(2,2)],[0 F1(2,3)],'color','blue')
plot3([0 F1(3,1)],[0 F1(3,2)],[0 F1(3,3)],'color','blue')
plot3([0 v1(1)],[0 v1(2)],[0 v1(3)],'color','black')
% Frame 2
plot3([0 F2(1,1)]+V(1),[0 F2(1,2)]+V(2),[0 F2(1,3)]+V(3),'color','red')
plot3([0 F2(2,1)]+V(1),[0 F2(2,2)]+V(2),[0 F2(2,3)]+V(3),'color','green')
plot3([0 F2(3,1)]+V(1),[0 F2(3,2)]+V(2),[0 F2(3,3)]+V(3),'color','green')
v2 = C'*v2;
plot3([0 v2(1)] + V(1),[0 v2(2)]+V(2),[0 v2(3)]+V(3),'color','magenta')
xlabel('x')
ylabel('y')
zlabel('z')
axis equal
xlim(lims)
ylim(lims)
zlim(lims)
hold off

figure 
grid on
plot3([0 v1(1)],[0 v1(2)],[0 v1(3)])
xlabel('x')
ylabel('y')
zlabel('z')
axis equal

figure
grid on
plot3([0 v2(1)],[0 v2(2)],[0 v2(3)])
xlabel('x')
ylabel('y')
zlabel('z')
axis equal
