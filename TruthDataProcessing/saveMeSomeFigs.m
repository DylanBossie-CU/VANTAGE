function saveMeSomeFigs(shouldSaveFigures, saveTitle, plotHandles, ...
                        increaseQuality)
    
    %%% saveMeSomeFigs(shouldSaveFigures, saveTitle, plotHandles, ...
    %%%                increaseQuality)
    %%%
    %%% Takes a boolean toggle (shouldSaveFigures) and a string with the
    %%% save name (saveTitle), including the path, that you want to save
    %%% the current figure as. Example saveTitle looks like the following:
    %%%
    %%%     saveTitle = '../Figures/Velocity vs Time.pdf' - this would save
    %%%     a figure named 'Velocity vs Time.pdf' (as a .pdf) to a folder
    %%%     called 'Figures' up one directoy from the code's working
    %%%     directory.
    %%% 
    %%% Uses the gca function to pull the current figure, normalize
    %%% and scale it to the deafult paper size, and save it as a .pdf.
    %%%
    %%% Examples function call:
    %%%
    %%% x = linspace(0, 2*pi, 100);
    %%% y = sin(x);
    %%% plot(x, y)
    %%% title('saveMeSomeFigs Test')
    %%%
    %%% saveTitle = 'saveMeSomeFigs Test Plot.pdf';
    %%% shouldSaveFigures = true;
    %%% saveMeSomeFigs(shouldSaveFigures, saveTitle)
    %%%
    %%% Last Modified: 12/7/2017
    %%% Date Created: 2/10/2017
    %%% Author: Nicholas Renninger

    
    %% Input Sanitization
    
    if ~any(nargin)
        error('Gotta use some args there m8.')
    elseif nargin < 2
        error('Need more than 2 args');   
    elseif nargin < 3
        increaseQuality = false;
        plotHandles = NaN;
    elseif nargin < 4
        if isa(plotHandles, 'cell')
            increaseQuality = false;
        else
            if isa(plotHandles, 'logical')
                increaseQuality = plotHandles;
            elseif isa(plotHandles, 'matlab.ui.Figure')
                increaseQuality = false;
            else
                error('increaseQuality must be of type: logical')
            end
        end
    end
    
    if isa(plotHandles, 'cell') || isa(saveTitle, 'cell')
        if isa(plotHandles, 'cell') && isa(saveTitle, 'cell')
            lenHandles = length(plotHandles);
            lenTitles = length(saveTitle);
            if lenTitles ~= lenHandles
                error('plotHandles and saveTitle must be the same length')
            end
        else
            error(['if either plotHandles and saveTitle are cell', ...
                   ' objects, both must be']);
        end
    else
        if isa(saveTitle, 'char')
            saveTitle = {saveTitle};
        else 
            error('saveTitle must be of type: char')
        end
        
        if isa(plotHandles, 'matlab.ui.Figure')
            plotHandles = {plotHandles};
        else
            plotHandles = {gcf};
        end
    end
    
    
    %% Actual function
    
    % setup and save figure as .pdf
    if shouldSaveFigures
         
        for i = 1:length(saveTitle)
                
            curr_fig = plotHandles{i};
            set(curr_fig, 'PaperOrientation', 'landscape');
            set(curr_fig, 'PaperUnits', 'normalized');
            set(curr_fig, 'PaperPosition', [0 0 1 1]);
            [fid, errmsg] = fopen(saveTitle{i}, 'a');

            if fid ~= -1
                fclose(fid);
            else
                while (fid == -1)
                    error(['Error Opening File in fopen: \n%s.', ...
                       ' It''s probably already open somewhere.'],...
                      errmsg); 
                end
            end

            if increaseQuality
                print(curr_fig, '-dpdf', saveTitle{i}, '-opengl', '-r400');
            else
                print(curr_fig, '-dpdf', saveTitle{i});
            end
        end
    end

end