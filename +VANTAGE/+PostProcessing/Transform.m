classdef Transform
    %% Classdef
    % A class used to save transform information from the
    % cartesian 3-dimensional frame srcFrame to the cartesian 3-dimensional
    % frame targFrame.  It is then used to transform a set of data in srcFrame
    % to a set of data in targFrame.
    %
    %% Properties
    properties(Access = private)
        %
        % struct containing indexed rotation matrices and translation vectors
        % between various frames
        TDATA
        
        % cell mapping frame names to indices in TDATA
        nomen
        
        % maximum allowable number of nomen entries (number of defined frames)
        maxNomen = 10
    end
    
    
    
    %% Methods
    methods
        %% Execute transform
        % Execute transformations between frames
        %
        % @param	targFrame   identifier for the target frame, char
        % @param    data        a 3xn set of vectors in the srcFrame to be transformed
        %                       to the targFrame, matrix
        % @param	srcFrame    identifier for the source frame, char
        %
        % @return	a 3xn set of vectors in the targFrame,
        %           matrix
        %
        % @author 	Joshua Kirby
        % @date 	24-Jan-2019
        function [output] = tf(obj,targFrame,data,srcFrame)
            % Error catching
            if size(obj.nomen,1) > obj.maxNomen
                error('''obj.nomen'' has more entries than the maximum specified by maxNomen.')
            end
            
            if ~ischar(srcFrame)
                error('srcFrame must be a single element string')
            end
            
            if isstring(srcFrame)
                if length(srcFrame) > 1
                    error('srcFrame must be a single element string')
                end
            end
            
            if ~ischar(targFrame)
                error('targFrame must be a single element string')
            end
            
            if isstring(targFrame)
                if length(targFrame) > 1
                    error('targFrame must be a single element string')
                end
            end
            
            % Perform transformation and translation
            % Identify src-to-targ indices in TDATA struct using obj.nomen
            src  = findNomen(obj,srcFrame);
            targ = findNomen(obj,targFrame);
            
            % Check that this particular src-to-targ is populated
            if ~src || ~targ
                error(['The requested transformation does not have a '...
                    'transform matrix or translation vector set'])
            end
            
            % Produced transformed/translated output data
            output = obj.TDATA(src,targ).C*(data-obj.TDATA(src,targ).V);
        end
        
        
        
        
        %% Save transform matrices and translation vectors to TDATA
        %
        % Records transform matrices and translation vectors between specific
        % frames in TDATA
        %
        % @param	targFrame   identifier for the target frame, char
        % @param	C           3x3 transformation matrix from source frame to target
        %                       frame
        % @param	V           3x1 translation vector from the origin of srcFrame to
        %                       the origin of targFrame, cell
        % @param    srcFrame    identifier for the source frame, char
        %
        % @author	Joshua Kirby
        % @date  	24-Jan-2019
        function obj = setTdata(obj,targFrame,C,V,srcFrame)
            % Error catching
            if size(C,1)~=3 || size(C,2)~=3
                error('Rotation matrix must be a 3x3 transform matrix')
            elseif sum(sum(C'*C-eye(3))) > 1000*eps
                error('Transform matrix C does not satisfy C''*C=I')
            end
            
            if size(V,1) ~= 3 || size(V,2)~= 1
                error('V must be a 3x1 translation vector')
            end
            
            if ~ischar(srcFrame)
                error('srcFrame must be a single element string')
            end
            
            if isstring(srcFrame)
                if length(srcFrame) > 1
                    error('srcFrame must be a single element string')
                end
            end
            
            if ~ischar(targFrame)
                error('targFrame must be a single element string')
            end
            
            if isstring(targFrame)
                if length(targFrame) > 1
                    error('targFrame must be a single element string')
                end
            end
            
            % Make new entry for srcFrame if not already in obj.nomen
            if ~findNomen(obj,srcFrame)
                obj.nomen{size(obj.nomen,1)+1,1} = srcFrame;
            end
            
            % Make new entry for targFrame if not already in obj.nomen
            if ~findNomen(obj,targFrame)
                obj.nomen{size(obj.nomen,1)+1,1} = targFrame;
            end
            
            % Set/overwrite transform matrix and translation vector (src-to-targ)
            obj.TDATA(findNomen(obj,srcFrame),findNomen(obj,targFrame)).C = C;
            obj.TDATA(findNomen(obj,srcFrame),findNomen(obj,targFrame)).V = V;
            
            % Set/overwrite transform matrix and translation vector (targ-to-src)
            obj.TDATA(findNomen(obj,targFrame),findNomen(obj,srcFrame)).C = C';
            obj.TDATA(findNomen(obj,targFrame),findNomen(obj,srcFrame)).V = C*(-V);
        end
        
    end
    
    
    
    
    
    %% Private methods
    methods (Access = private)
        %% Index into nomen
        %
        % Return row number of string location in obj.nomen
        %
        % @param	str     string to be found in obj.nomen
        %
        % @return           index of string in obj.nomen, set to 0 if string not
        %                   found
        %
        % @author   Joshua Kirby
        % @date 	24-Jan-2019
        function [row] = findNomen(obj,str)
            
            [row,~] = ind2sub(size(obj.nomen),find(strcmp(str,obj.nomen)));
            
            if isempty(row)
                row = 0;
            end
            
        end
    end
    
    
    
    
    
    
end