function [d] = distInFromFace(face,u)
    
    % Calculate standard areas
    Avec = (1:3).*0.1.^2;
    
    % Calculate face area
    A = area(face);
    
    % Calculate difference between area and standard areas
    dA = abs(A-Avec);
    
    % Choose nearest size
    [~,I] = min(dA);
    
    % Set distance in for centroid
    dVec = 0.5.*(1:3).*0.1;
    if u==1
        d = dVec(1);
    else
        if I==1
            d = dVec(I);
        else
            d = dVec(1);
        end        
    end
    
end