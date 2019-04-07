function murder(input)

if nargin==0
    input='all';
end

if any(strcmpi(input,{'all','everything'}))
    close all;
    close hidden;
    evalin('base','clear');
    clear variables;
    clear global;
    clc;
    home;
    return;
elseif any(strcmpi(input,{'plots','figures','pictures'}))
    close all;
    close hidden;
    return;
elseif any(strcmpi(input,{'plot','figure','picture'}))
    a=gcf;
    close(a);
    return;
elseif any(strcmpi(input,{'variables','vars'}))
    clear variables;
    return;
elseif any(strcmpi(input,{'word','words','command window','command','this'}))
    clc;
    home;
    return;
else
    try
        clear(input);
    catch
        try
            close(input);
        catch
            error('Murder:NoOneMurdered','There was no one to murder...');
        end
    end
end

end