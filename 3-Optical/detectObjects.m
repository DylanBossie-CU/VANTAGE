function detectedObject = detectObjects(I_boundaries,s,~)
maxBoundary = max(s);
pixelThreshold = 0.1*maxBoundary;
detectedObject = [];
for i=1:length(s)
    if s(i) > pixelThreshold
        detectedObject = [detectedObject I_boundaries(i)];
    end
end

end