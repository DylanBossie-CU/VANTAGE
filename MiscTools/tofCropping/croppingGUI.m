function varargout = croppingGUI(varargin)
% CROPPINGGUI MATLAB code for croppingGUI.fig
%      CROPPINGGUI, by itself, creates a new CROPPINGGUI or raises the existing
%      singleton*.
%
%      H = CROPPINGGUI returns the handle to a new CROPPINGGUI or the handle to
%      the existing singleton*.
%
%      CROPPINGGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in CROPPINGGUI.M with the given input arguments.
%
%      CROPPINGGUI('Property','Value',...) creates a new CROPPINGGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before croppingGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to croppingGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help croppingGUI

% Last Modified by GUIDE v2.5 07-Apr-2019 12:22:03

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @croppingGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @croppingGUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
warning('off','MATLAB:MKDIR:DirectoryExists');
% End initialization code - DO NOT EDIT


% --- Executes just before croppingGUI is made visible.
function croppingGUI_OpeningFcn(hObject, eventdata, handles, varargin)

    % Choose default command line output for croppingGUI
    handles.output = hObject;
    handles.vars.TofDateFormat = 'yyyy_mm_dd_HH_MM_SS.FFF';
    handles.vars.TruthDateFormat = 'dd-mmm-yyyy HH:MM:SS.FFF';

    % Update handles structure
    guidata(hObject, handles);

    % UIWAIT makes croppingGUI wait for user response (see UIRESUME)
    % uiwait(handles.figure1);


function varargout = croppingGUI_OutputFcn(hObject, eventdata, handles) 
    varargout{1} = handles.output;

%% Edit creation stuff (ignore)

function xMinEdit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function xMaxEdit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function yMinEdit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function yMaxEdit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function zMinEdit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function zMaxEdit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

%% Edit callback functions

function xMinEdit_Callback(hObject, eventdata, handles)
    xMin = str2double(handles.xMinEdit.String);
    xMax = str2double(handles.xMaxEdit.String);
    xMin = xMin + round(hObject.Value);
    if xMax > xMin
      handles.xMinEdit.String = num2str(xMin);
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    plotTruthPoints(hObject,handles)

function xMaxEdit_Callback(hObject, eventdata, handles)
    xMin = str2double(handles.xMinEdit.String);
    xMax = str2double(handles.xMaxEdit.String);
    xMax = xMax + round(hObject.Value);
    if xMax > xMin
      handles.xMaxEdit.String = num2str(xMax);
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    plotTruthPoints(hObject,handles)

function yMinEdit_Callback(hObject, eventdata, handles)
    yMin = str2double(handles.yMinEdit.String);
    yMax = str2double(handles.yMaxEdit.String);
    yMin = yMin + round(hObject.Value);
    if yMax > yMin
      handles.yMinEdit.String = num2str(yMin);
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    plotTruthPoints(hObject,handles)

function yMaxEdit_Callback(hObject, eventdata, handles)
    yMin = str2double(handles.yMinEdit.String);
    yMax = str2double(handles.yMaxEdit.String);
    yMax = yMax + round(hObject.Value);
    if yMax > yMin
      handles.yMaxEdit.String = num2str(yMax);
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    plotTruthPoints(hObject,handles)

function zMinEdit_Callback(hObject, eventdata, handles)
    zMin = str2double(handles.zMinEdit.String);
    zMax = str2double(handles.zMaxEdit.String);
    zMin = zMin + round(hObject.Value);
    if zMax > zMin
      handles.zMinEdit.String = num2str(zMin);
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    plotTruthPoints(hObject,handles)

function zMaxEdit_Callback(hObject, eventdata, handles)
    zMin = str2double(handles.zMinEdit.String);
    zMax = str2double(handles.zMaxEdit.String);
    zMax = zMax + round(hObject.Value);
    if zMax > zMin
      handles.zMaxEdit.String = num2str(zMax);
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    plotTruthPoints(hObject,handles)

%% Directory menu functions

function dirList_Callback(hObject, eventdata, handles)
    handles.vars.workingDirNum = hObject.Value;
    handles.vars.workingDir = hObject.String{handles.vars.workingDirNum};

    workingDirFiles = dir(strcat(handles.vars.dirName,'/',handles.vars.workingDir));
    handles.vars.workingDirFiles = workingDirFiles(~vertcat(workingDirFiles(:).isdir));

    numFiles = numel(handles.vars.workingDirFiles);
    handles.fileList.String = cell(numFiles,1);
    for i = 1:numFiles
        handles.fileList.String{i} = handles.vars.workingDirFiles(i).name;
    end
    
    % Get time stamps
    handles.vars.t = NaN(numFiles,6);
    for i = 1:numFiles
        if ~handles.vars.workingDirFiles(i).isdir

            % Get time stamp
            handles.vars.t(i,:) = datevecFromTofFilename(handles,handles.vars.workingDirFiles(i).name);
        end
    end

    % Get truth data
    addpath(genpath('D:\My Stuff\School\CU Boulder\2018 Fall\ASEN 4018\VANTAGE'));
    handles.vars.T = VANTAGE.PostProcessing.Transform('D:\My Stuff\School\CU Boulder\2018 Fall\ASEN 4018\VANTAGE\Config\Transform.json');

    truthFileName = strcat('./Data/Truth/',handles.vars.workingDir,'/truth.json');
    truthData = jsondecode(fileread(truthFileName));
    handles.vars.truthData = truthData;
    numCubesats = numel(fieldnames(truthData{2,1}(1).pos));

    startString = truthData{1,1};
    if ~isempty(startString)
        if isnumeric(startString)
            truthTime_start = datevec(startString);
        else
            truthTime_start = datevec(startString,'dd-mmm-yyyy HH:MM:SS.FFF');
        end
    else
        truthTime_start = handles.vars.t(1,:);
    end

    t_tmp = truthData{2,1}(1).t;
    %truthTime_start = datevec(datetime(truthTime_start) + seconds(t_tmp));
    handles.vars.truthTime_start = truthTime_start;
    handles.vars.t = etime(handles.vars.t,truthTime_start);
    %handles.vars.t = handles.vars.t(:,6);
    handles.vars.truthTime = vertcat(truthData{2,1}(:).t) - t_tmp;

    truthVecs = cell(numCubesats,1);
    numTruthTimes = numel(handles.vars.truthTime);
    for i = 1:numCubesats
        for j = 1:numTruthTimes
            truthVecs{i}(j,:) = truthData{2,1}(j).pos.(sprintf('CubeSat_%i',i))';
            truthVecs{i}(j,3) = truthVecs{i}(j,3);
            truthVecs{i}(j,2) = truthVecs{i}(j,2);
        end    
    end

    if numCubesats==1
        for i = 2:3
            for j = 1:numTruthTimes
                truthVecs{i}(j,1) = truthVecs{1}(j,1);
                truthVecs{i}(j,2) = truthVecs{1}(j,2);
                truthVecs{i}(j,3) = truthVecs{1}(j,3) + (i-1).*.2;
            end
        end
        numCubesats = 3;
        handles.vars.numCubesats = 3;
    end
    
    for i = 1:numCubesats
        handles.vars.truthVecs{i} = handles.vars.T.tf('TCF',truthVecs{i}','VCF')'.*1000;
    end
    handles.vars.numCubesats = numCubesats;

    handles.vars.truthVecsAdj = handles.vars.truthVecs;

    % Make directory for saving cropped files
    mkdir(strcat(handles.vars.dirName,'/',handles.vars.workingDir,'/Cropped'));

    % Update handles structure
    guidata(hObject, handles);

function dirList_CreateFcn(hObject, eventdata, handles)
    handles.vars.dirName = './Data';
    currentDir = dir(handles.vars.dirName);
    numFile = numel(currentDir);
    I = false(numFile,1);
    for i = 1:numFile
        I(i) = currentDir(i).isdir && ~strcmp(currentDir(i).name(1),'.');
    end
    currentDir = currentDir(I);
    numDir = numel(currentDir);
    hObject.String = cell(numDir,1);
    for i = 1:numDir
        hObject.String{i} = currentDir(i).name;
    end

    handles.vars.workingDirNum = hObject.Value;
    handles.vars.workingDir = hObject.String{handles.vars.workingDirNum};

    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

    % Update handles structure
    guidata(hObject, handles);

function fileList_Callback(hObject, eventdata, handles)
    handles.vars.workingFileNum = hObject.Value;
    handles.vars.workingFile = hObject.String{handles.vars.workingFileNum};

    % Load point cloud
    handles.vars.ptCloud = pcread(strcat(handles.vars.dirName,'/',handles.vars.workingDir,'/',handles.vars.workingFile));

    % Find and update limits
    x = handles.vars.ptCloud.Location(:,1);
    y = handles.vars.ptCloud.Location(:,2);
    z = handles.vars.ptCloud.Location(:,3);

    handles.xMinEdit.String = num2str(min(x));
    handles.xMaxEdit.String = num2str(max(x));
    handles.yMinEdit.String = num2str(min(y));
    handles.yMaxEdit.String = num2str(max(y));
    handles.zMinEdit.String = num2str(min(z));
    handles.zMaxEdit.String = num2str(max(z));

    % Update handles structure
    guidata(hObject, handles);

    % Plot point cloud
    handles = updateCrop(hObject,handles);
    handles = updateTruthPoints(hObject, handles);
    plotTruthPoints(hObject,handles)

    % Update handles structure
    guidata(hObject, handles);

function fileList_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

%% Save button

function saveButton_Callback(hObject, eventdata, handles)
    pcwrite(handles.vars.ptCloudCrop,strcat(handles.vars.dirName,'/',handles.vars.workingDir,'/Cropped/',handles.vars.workingFile))

%% Auto crop button
function autoCropButton_Callback(hObject, eventdata, handles)
    xMin = min(handles.vars.truthVecs{1}(:,1));
    xMax = max(handles.vars.truthVecs{1}(:,1));
    yMin = min(handles.vars.truthVecs{1}(:,2));
    yMax = max(handles.vars.truthVecs{1}(:,2));
    xMin = handles.vars.truthPos{1}(1);
    xMax = handles.vars.truthPos{1}(1);
    yMin = handles.vars.truthPos{1}(2);
    yMax = handles.vars.truthPos{1}(2);
    zMin = handles.vars.truthPos{3}(3);
    zMax = handles.vars.truthPos{1}(3);

    xPad = 100;
    yPad = 100;
    zPad = 30;

    handles.xMinEdit.String = num2str(xMin-2*xPad);
    handles.xMaxEdit.String = num2str(xMax+xPad);
    handles.yMinEdit.String = num2str(yMin-yPad);
    handles.yMaxEdit.String = num2str(yMax+yPad);
    handles.zMinEdit.String = num2str(zMin-120);
    handles.zMaxEdit.String = num2str(zMax+150);

    % Update handles structure
    guidata(hObject, handles);
    pause(0.0001)

    % Update plot
    handles = updateCrop(hObject,handles);
    plotTruthPoints(hObject,handles)

%% Timestamp adjustment stuff

function timeOffsetEdit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end


function timeOffsetEdit_Callback(hObject, eventdata, handles)

    hObject.String = sprintf('%.4f',str2double(handles.timeOffsetEdit.String));

    % Update handles structure
    guidata(hObject, handles); 

    % Update plot
    handles = updateCrop(hObject,handles);
    handles = updateTruthPoints(hObject, handles);
    plotTruthPoints(hObject,handles)

function saveTimeButton_Callback(hObject, eventdata, handles)
    truthTime_start = datevec(datetime(handles.vars.truthTime_start) - seconds(str2double(handles.timeOffsetEdit.String)));

    timeTruthData = handles.vars.truthData;
    timeTruthData{1} = datestr(truthTime_start,handles.vars.TruthDateFormat);
    for i = 1:handles.vars.numCubesats
        tmpTruth = handles.vars.T.tf('VCF',handles.vars.truthVecsAdj{i}'/1000,'TCF')';
        for j = 1:size(tmpTruth,1)
            timeTruthData{2,1}(j).pos.(sprintf('CubeSat_%i',i)) = tmpTruth(j,:)';
        end
    end
    %{
    for j = 1:size(tmpTruth,1)
        timeTruthData{2,1}(j).t = handles.vars.truthTime(j);
    end
    %}

    % Make directory for saving time adjusted truth file
    mkdir(strcat(handles.vars.dirName,'/',handles.vars.workingDir,'/newTruth'));

    FID = fopen(strcat(handles.vars.dirName,'/',handles.vars.workingDir,'/newTruth/newTruth.json'),'w');
    fprintf(FID,'%s',jsonencode(timeTruthData));

    fclose(FID);

%% Truth rotation and translation

function rotYedit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function tranYedit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function tranXedit_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end

function rotYedit_Callback(hObject, eventdata, handles)

    hObject.String = sprintf('%.2f',str2double(hObject.String));

    rotY = str2num(handles.rotYedit.String);
    tranX = str2num(handles.tranXedit.String);
    tranY = str2num(handles.tranYedit.String);

    for i = 1:handles.vars.numCubesats
        truthVec = handles.vars.truthVecs{i};
        truthVec = truthVec + [tranX, tranY, 0];
        c = cosd(rotY); s = sind(rotY);
        M = [c, 0, s;...
             0, 1, 0;...
            -s, 0, c];
        truthVec = truthVec*M;

        handles.vars.truthVecsAdj{i} = truthVec;
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    handles = updateTruthPoints(hObject, handles);
    plotTruthPoints(hObject,handles)


function tranYedit_Callback(hObject, eventdata, handles)

    hObject.String = sprintf('%.0f',str2double(hObject.String));

    rotY = str2num(handles.rotYedit.String);
    tranX = str2num(handles.tranXedit.String);
    tranY = str2num(handles.tranYedit.String);

    for i = 1:handles.vars.numCubesats
        truthVec = handles.vars.truthVecs{i};
        truthVec = truthVec + [tranX, tranY, 0];
        c = cosd(rotY); s = sind(rotY);
        M = [c, 0, s;...
             0, 1, 0;...
            -s, 0, c];
        truthVec = truthVec*M;

        handles.vars.truthVecsAdj{i} = truthVec;
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    handles = updateTruthPoints(hObject, handles);
    plotTruthPoints(hObject,handles)


function tranXedit_Callback(hObject, eventdata, handles)

    hObject.String = sprintf('%.0f',str2double(hObject.String));

    rotY = str2num(handles.rotYedit.String);
    tranX = str2num(handles.tranXedit.String);
    tranY = str2num(handles.tranYedit.String);

    for i = 1:handles.vars.numCubesats
        truthVec = handles.vars.truthVecs{i};
        truthVec = truthVec + [tranX, tranY, 0];
        c = cosd(rotY); s = sind(rotY);
        M = [c, 0, s;...
             0, 1, 0;...
            -s, 0, c];
        truthVec = truthVec*M;

        handles.vars.truthVecsAdj{i} = truthVec;
    end

    % Update handles structure
    guidata(hObject, handles);

    % Update plot
    handles = updateCrop(hObject,handles);
    handles = updateTruthPoints(hObject, handles);
    plotTruthPoints(hObject,handles)

%% Additional functions

function handles = updateCrop(hObject,handles)

    % Get limits
    xMin = str2double(handles.xMinEdit.String);
    xMax = str2double(handles.xMaxEdit.String);
    yMin = str2double(handles.yMinEdit.String);
    yMax = str2double(handles.yMaxEdit.String);
    zMin = str2double(handles.zMinEdit.String);
    zMax = str2double(handles.zMaxEdit.String);

    % Update region of interest
    roi = [xMin xMax yMin yMax zMin zMax];
    indices = findPointsInROI(handles.vars.ptCloud,roi);
    handles.vars.ptCloudCrop = select(handles.vars.ptCloud,indices);

    % Plot point cloud
    hold(handles.axes1,'off')
    h(3) = pcshow(handles.vars.ptCloudCrop,'Parent',handles.axes1,'MarkerSize',10);
    hold(handles.axes1,'on')
    h(2) = plot3(handles.axes1,handles.vars.truthVecsAdj{1}(:,1),handles.vars.truthVecsAdj{1}(:,2),handles.vars.truthVecsAdj{1}(:,3),'k-');
    xlabel(handles.axes1,'X (mm)')
    ylabel(handles.axes1,'Y (mm)')
    zlabel(handles.axes1,'Z (mm)')
    xlim(handles.axes1,[xMin xMax])
    ylim(handles.axes1,[yMin yMax])
    zlim(handles.axes1,[zMin zMax])
    title(handles.axes1,strcat(handles.vars.workingDir,'/',handles.vars.workingFile),'Interpreter','none')
    legend(handles.axes1,'Raw Data','Truth Data Trajectory')

    % Update handles structure
    guidata(hObject, handles);
    pause(0.0001)

function handles = updateTruthPoints(hObject,handles)

    % Update truth time
    offsetTime = str2double(handles.timeOffsetEdit.String);

    % Interpolate positions from truth data
    truthPos = cell(handles.vars.numCubesats,1);
    for i = 1:handles.vars.numCubesats
        for j = 1:3
            truthPos{i}(j,1) = interp1(handles.vars.truthTime,handles.vars.truthVecsAdj{i}(:,j),handles.vars.t(handles.fileList.Value)+offsetTime,'linear','extrap');
        end
    end

    handles.vars.truthPos = truthPos;

    % Update position display box
    handles.cubesat1ZText.String = sprintf('%.0f',handles.vars.truthPos{1}(3,1));

    % Update handles structure
    guidata(hObject, handles);
    pause(0.0001)

function plotTruthPoints(hObject,handles)

    % Plot values
    hold(handles.axes1,'on')
    for i = 1:handles.vars.numCubesats
        plot3(handles.axes1,handles.vars.truthPos{i}(1,1),handles.vars.truthPos{i}(2,1),handles.vars.truthPos{i}(3,1),'k.','MarkerSize',15)
    end

    % Update handles structure
    guidata(hObject, handles);

function [dv] = datevecFromTofFilename(handles,TOF_filename)
    TOF_filename = char(TOF_filename);
    ds = TOF_filename(12:end-4);
    dv = datevec(ds,handles.vars.TofDateFormat);
