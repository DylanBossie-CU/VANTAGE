parallelPool = parpool(2);
parfor i = 1:2
    if i == 1
        !python "/home/vantage/Documents/githere/VANTAGE/Automation/TOFExamples/o3d3xx-python/examples/image_viewer_messing.py" 169.254.145.24
    elseif i == 2
        !python test2.py
    end
end
delete(parallelPool);
    
clear