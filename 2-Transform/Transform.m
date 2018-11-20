%%  Author: Joshua Kirby
%  Created: 11/19/2019
% Modified: 11/19/2019
%
% Transform
% [VT] = Transform(TARGFRAME,VS,SRCFRAME) with VS a 3xn vector in the
% SRCFRAME, VT a 3xn vector in the TARGFRAME
%
% [VT] = Transform(TARGFRAME,VS,SRCFRAME,'transform') with VS a 3xn 
% vector in the SRCFRAME, VT a 3xn vector in the TARGFRAME, this is the
% default behavior
%
% Transform(TARGFRAME,celldata,SRCFRAME,'set') with celldata{1}
% a 3x3 transform matrix and celldata{2} a 3x1 translation vector, will
% save celldata{1} as the transform between SRCFRAME and TARGFRAME and 
% celldata{2} as the vector (in SRCFRAME) from the origin of SRCFRAME to
% the origin of TARGFRAME
%
% Purpose:
%
% Inputs:
%
% Outputs:
%
function [output] = Transform(targFrame,data,srcFrame,varargin)
%% Allocation 
% Define persistents
persistent TF nomen

% Define maximum allowable length for nomen (number of known frames)
maxNomen = 10;

% Initialize mode to 0, mode is a flag defining whether Transform is in
% transform operation (mode=0) or 'set' operation (mode=1)
mode = 0;

% Initialize nomen with the Camera Angular Frame (CAF) and VANTAGE
% Cartesian Frame (VCF), because it is a unique transformation and will
% therefore be baked into the function
if isempty(nomen)
  nomen{1,1} = 'VCF';
  nomen{2,1} = 'CAF';
end


%% Process varargin
minArgs = 3;
maxArgs = 4;
narginchk(minArgs,maxArgs);

if nargin > minArgs
  for i = 1:length(varargin)
    if strcmpi('transform',varargin{i})
      mode = 0;
    elseif strcmpi('set',varargin{i})
      mode = 1;
    else
      warning('Unrecognized varargin, proceeding in default (''transform'') mode')
    end
  end
end

%% Error catching
if size(nomen,1) > maxNomen
  error('''nomen'' has more entries than the maximum specified by maxNomen.')
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

if mode == 1
  if ~iscell(data)
    error('data must be a 2-element cell array when in ''set'' mode')
  end
  
  if size(data{1},1)~=3 || size(data{1},2)~=3
    error('When in ''set'' mode, data{1} must be a 3x3 transform matrix')
  elseif sum(sum(data{1}'*data{1}-eye(3))) > 1000*eps
    error('Transform matrix data{1} does not satisfy data{1}''*data{1}=I')
  end
  
  if size(data{2},1) ~= 3 || size(data{2},2)~= 1
    error('When in ''set'' mode, data{2} must be a 3x1 translation vector')
  end
end


%% Save transform matrices and translation vectors if in 'set' mode
if mode == 1
  % Make new entry for srcFrame if not already in nomen
  if ~findNomen(srcFrame,nomen)
    nomen{size(nomen,1)+1,1} = srcFrame;
  end
  
  % Make new entry for targFrame if not already in nomen
  if ~findNomen(targFrame,nomen)
    nomen{size(nomen,1)+1,1} = targFrame;
  end
  
  % Set/overwrite transform matrix and translation vector (src-to-targ)
  TF(findNomen(srcFrame,nomen),findNomen(targFrame,nomen)).C = data{1};
  TF(findNomen(srcFrame,nomen),findNomen(targFrame,nomen)).V = data{2};
  
  % Set/overwrite transform matrix and translation vector (targ-to-src)
  TF(findNomen(targFrame,nomen),findNomen(srcFrame,nomen)).C = data{1}';
  TF(findNomen(targFrame,nomen),findNomen(srcFrame,nomen)).V = data{1}*(-data{2});
end

%% Perform transformation and translation if in 'transform' or default mode
if mode == 0
  % Identify src-to-targ indices in TF struct using nomen
  src  = findNomen(srcFrame,nomen);
  targ = findNomen(targFrame,nomen);
  
  % Check that this particular src-to-targ is populated
  if ~src || ~targ
    error(['The requested transformation does not have a '...
      'transform matrix or translation vector set'])
  end
  
  % Produced transformed/translated output data
  output = TF(src,targ).C*(data-TF(src,targ).V);
end

end % Transform

%% Return row number of string location in nomen
function [row] = findNomen(str,nomen)

[row,~] = ind2sub(size(nomen),find(strcmp(str,nomen)));

if isempty(row)
  row = 0;
end

end % findNomen