function detectedObjects = detectObjects(I_boundaries,s,~)
maxBoundary = max(s);
pixelThreshold = 0.2*maxBoundary;
j = 1;
for i=1:length(s)
    if s(i) > pixelThreshold
        detectedObjects{j} = I_boundaries{i};
        j = j + 1;
    end
end

end