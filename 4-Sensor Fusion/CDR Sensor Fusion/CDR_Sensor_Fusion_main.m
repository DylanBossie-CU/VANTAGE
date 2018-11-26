

camOrigin = [0 0 0];
camVec = [1 .5 0];
pos_TOF = [2 0.75 0];

sig_cam = 0.01;
sig_TOF = 0.05;

[pos] = fusionCentroid(camOrigin, camVec, pos_TOF, sig_cam, sig_TOF);