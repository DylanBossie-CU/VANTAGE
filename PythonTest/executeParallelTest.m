parallelPool = parpool(2);
parfor i = 1:2
    if i == 1
        !"C:\Users\bobbl\AppData\Local\Programs\Python\Python37-32\python.exe" test1.py
    elseif i == 2
        !"C:\Users\bobbl\AppData\Local\Programs\Python\Python37-32\python.exe" test2.py
    end
end
delete(parallelPool);
    
clear