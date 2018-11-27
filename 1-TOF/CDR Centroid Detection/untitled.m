direc = 'Simulation Data/Tube 6/';
ls = dir(direc);
for i = 1:length(ls)
  if contains(ls(i).name,'FPS_Tube')
    str = ls(i).name;
    j = strfind(ls(i).name,'VZ_0P1');
    str(j:j+5) = 'VZ_001';
    movefile([direc ls(i).name],[direc str]);
  end
end