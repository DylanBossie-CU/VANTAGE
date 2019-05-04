classdef Transform
    %% Classdef
    % A class used to save transform information from the
    % cartesian 3-dimensional frame srcFrame to the cartesian 3-dimensional
    % frame targFrame.  It is then used to transform a set of data in srcFrame
    % to a set of data in targFrame.
    %
    %% Properties
    properties(Access = public)
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
        %% Class Constructor
        %
        % Constructs class using Transform.json config file
        %
        % @param    configFilename  relative path to JSON config file
        %
        % @return   Class instance
        % 
        % @author   Joshua Kirby
        % @date     17-Feb-2019
        function this = Transform(configFilename)
            if nargin == 1
                % Obtain data from json file
                config = jsondecode(fileread(configFilename));

                % Set transforms between TCF and VCF
                this = this.setTdata('VCF',config.VT_DCM,config.V_TCF2VCF,'TCF');

                % Set transforms between CCF and VCF
                this = this.setTdata('VCF',config.VC_DCM,config.V_CCF2VCF,'CCF');
            else
                error('Incorrect number of input arguments for initialization of Transform Class instance')
            end
        end
        
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
        function [output] = tf(this,targFrame,data,srcFrame)
            % Error catching
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
            
            if size(data,1)~=3
                error('''data'' must be a 3xn set of vectors to be transformed from srcFrame to targFrame')
            end
            
            % Perform transformation and translation
            % Identify src-to-targ indices in TDATA struct using this.nomen
            src  = findNomen(this,srcFrame);
            targ = findNomen(this,targFrame);
            
            % Check that this particular src-to-targ is populated
            if ~src || ~targ
                error(['The requested transformation does not have a '...
                    'transform matrix or translation vector set'])
            elseif isempty(this.TDATA(src,targ).C) || isempty(this.TDATA(src,targ).V)
                error('The requested transformation frames exist separately but their relation is not populated explicitly')
            end
            
            % Produced transformed/translated output data
            output = this.TDATA(src,targ).C*(data-this.TDATA(src,targ).V);
        end
        
    end
    
    
    
    %% Private methods
    methods (Access = private)
        %% Save transform matrices and translation vectors to TDATA
        %
        % Records transform matrices and translation vectors between specific
        % frames in TDATA
        %
        % @param	targFrame   identifier for the target frame, char
        % @param	C           3x3 transformation matrix from source frame to target
        %                       frame
        % @param	V           3x1 translation vector from the origin of srcFrame to
        %                       the origin of targFrame (expressed in srcFrame), cell
        % @param    srcFrame    identifier for the source frame, char
        %
        % @author	Joshua Kirby
        % @date  	24-Jan-2019
        function this = setTdata(this,targFrame,C,V,srcFrame)
            % Error catching
            if size(this.nomen,1) == this.maxNomen
                error('''this.nomen'' already has the maximum number of entries as specified by maxNomen.')
            end
            
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
            
            if strcmp(srcFrame,targFrame)
                warning('specification of a transformation from a frame to itself is not necessary, returning...')
                return
            end
            
            % Make new entry for srcFrame if not already in this.nomen
            if ~findNomen(this,srcFrame)
                this.nomen{size(this.nomen,1)+1,1} = srcFrame;
            end
            
            % Make new entry for targFrame if not already in this.nomen
            if ~findNomen(this,targFrame)
                this.nomen{size(this.nomen,1)+1,1} = targFrame;
            end
            
            % Set/overwrite transform matrix and translation vector (src-to-targ)
            this.TDATA(findNomen(this,srcFrame),findNomen(this,targFrame)).C = C;
            this.TDATA(findNomen(this,srcFrame),findNomen(this,targFrame)).V = V;
            
            % Set/overwrite transform matrix and translation vector (targ-to-src)
            this.TDATA(findNomen(this,targFrame),findNomen(this,srcFrame)).C = C';
            this.TDATA(findNomen(this,targFrame),findNomen(this,srcFrame)).V = C*(-V);
        end
        %% Index into nomen
        %
        % Return row number of string location in this.nomen
        %
        % @param	str     string to be found in this.nomen
        %
        % @return           index of string in this.nomen, set to 0 if string not
        %                   found
        %
        % @author   Joshua Kirby
        % @date 	24-Jan-2019
        function [row] = findNomen(this,str)
            
            [row,~] = ind2sub(size(this.nomen),find(strcmp(str,this.nomen)));
            
            if isempty(row)
                row = 0;
            end
            
        end
    end
    
    
    
    
    
    
end