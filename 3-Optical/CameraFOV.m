function CameraFOV(nx,ny)
%Optical Camera Parameters: 
%Inputs: Pixel size x, pixel size y, number pixels x, number pixels y,
%desired FOV, 

%outputs: angular resolution (deg/pixel), focal length
% Projection size of CubeSat.

clear
PSizeX=5.5e-3; %mm
PSizeY=5.5e-3;

nx=2048;
ny=2048;

FOV=19.5;%desired FOV

SSizeX=PSizeX*nx;%sensor Size
SSizeY=PSizeY*ny;

FL=SSizeX/tand(FOV/2);%calculates Focal Length for this FOV and sensor size
%ASSUMPTION: The Lens is perfectly designed for that sensor size 

%Resolution per pixel
AnglRez=FOV/nx;

%at 100 m, 10 cm takes up
angRad=deg2rad(AnglRez);
%50 meter radius
%each pixel is
pixel50=(50*angRad)*100;%cm per pixel
CubeSide=10/pixel50;%10 cm edge is this many pixels
end