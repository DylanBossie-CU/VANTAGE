classdef TOF
    %% Classdef
    % A class representing the Time of Flight Camera used by VANTAGE
    %
    %% Properties
    properties
        %
        % Maximum range within which TOF data is considered valid, m
        maxAllowableRange = 8
        
        % The transform class used for frame changes
        % @see Transform
        Transform
        
    end
    
    
    %% Methods
    methods
        
        
        
        
        
    end
    
    %% Private Methods
    methods (Access = private)
        %% Load data from a simulation file
        %
        % Loads data from a simulation file
        %
        %
        function ptNew = loadSimToF(filename)
            % LEGACY CODE
            %{
          
          ptCloud = pcread(filename);
          
          if sum(sum(sum(~isnan(ptCloud.Location))))==0
              ptNew = struct('Count',0);
          else
              % Filter extraneous points
              pts = reshape(ptCloud.Location,ptCloud.Count,3);
              I = logical(prod(~isnan(pts),2));
              ptNew = pointCloud([pts(I,1:2),-pts(I,3)]);
          end
          
          
            figure
            pcshow(ptNew)
            hold on
            plot3(0,0,0,'k*')
            grid on
            axis equal
            %}
            
        end
    end
    
    
    
end % classdef