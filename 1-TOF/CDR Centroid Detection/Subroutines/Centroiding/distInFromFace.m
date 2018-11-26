function [d] = distInFromFace(face,u,SET)
    
  % Calculate standard areas
  Avec = SET.CSPARAMS.A;

  % Calculate face area
  A = area(face);

  % Calculate difference between area and standard areas
  dA = abs(A-Avec);

  % Choose nearest size
  [~,I] = min(dA);

  % Set distance in for centroid
  dVec = 0.5.*SET.CSPARAMS.L;
  if I==7
    d = dVec(u);
  else
    d = dVec(7);
  end        
end