%
% Given a Model and SensorData, plots errors vs requirements over the range
% of TOF data
%
% @param
% 
% @return
%
% @author   Joshua Kirby
% @date     19-Mar-2019
function TOFpresentErrorsVsReqs(Model,SensorData,truthFileName)
%% TOF Processing
Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
    Model.Deployer,'presentResults',0,'fileLims',fileLims,'showDebugPlots',0);

%% Truth Data processing
