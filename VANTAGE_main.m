%% The Signals
wokeSig='NUC is Woke';
manifestSig='Manifest is Sent';
standbySig='NUC is Standby';
initiateSig='CubeSat is Deploying';
resultsSig='Results is Done';


%% Set up sockets (sig=signals, data=data, PI is server)
t_sig = tcpip('169.254.146.34', 4013, 'NetworkRole', 'client');
t_data = tcpip('169.254.146.34', 4014, 'NetworkRole', 'client');

%% Pi is written to 32768 size bytes (32768 was chosen arbitrarily(favorite number), max size unknown)
%Set Output Buffer Size
t_sig.OutputBufferSize=32768;
t_data.OutputBufferSize=32768;

% Open Sockets
fopen(t_sig);
fopen(t_data);

%% Tell PI that NUC is awake
send(t_sig,wokeSig);
pause(0.2)

%% Wait for PI to send the manifest
standby(t_sig,manifestSig);

%% Recieve Manifest, write to ManifestID file
ManifestID=fopen('Manifesto.json','w');
manifest=receive(t_data);
fwrite(ManifestID,manifest);
fclose(ManifestID);

%% Tell PI that NUC is ready to take Data
send(t_sig,standbySig);

%% Wait for PI to tell NUC to start
standby(t_sig,initiateSig);


%% Start sensor automation
% File Definitions
% High-level directory of all TOF information. This should contain the
% executable data capture script and the desired location to save point
% cloud outputs.
tofDirName = '/home/vantage/Documents/githere/VANTAGE/Automation/TOF_Automation/examples/';
tofDirData = '/home/vantage/Documents/githere/VANTAGE/Data/TOF_Automated/';
tofFlush = strcat(tofDirData,'pointcloud*');
tofScript = strcat(tofDirName,'TOF_Data_Capture.py');

opticalDirName = '/home/vantage/Documents/githere/VANTAGE/Automation/OpticalCam_Automation/';
opticalDirImages = '/home/vantage/Documents/githere/VANTAGE/Data/AutomatedImageCapture/';
opticalDirData = '/home/vantage/Documents/githere/VANTAGE/Data/Optical_Automated/';
opticalFlush = strcat(opticalDirData,'VANTAGEOp*');
opticalScript = strcat(opticalDirName,'Optical_Data_Capture.py');

% Flush all TOF point clouds detected in output directory
unix(sprintf('rm %s',tofFlush));

% Flush all optical images detected in output directory
unix(sprintf('rm %s',opticalFlush));


% TOF Data Collection
% This command executes a bash command, which will begin data capture using
% the TOF sensor, and save point clouds to an output directory.
unix(sprintf('python %s &',tofScript));

% Optical Camera Data Collection
% This command executes a bash command, which will begin data capture using
% the optical camera, and save images to an output directory.
unix(sprintf('python %s &',opticalScript));

%% Start le epic post-processing PogChamp
import VANTAGE.PostProcessing.*
import VANTAGE.PostProcessing.Validate

%%%%%% Modular config direcs
configDirecNameModular = 'config/ModularTest_4_9/Test*';
%%%%%% 100m config direcs
configDirecName100m = 'config/3_25_100m/Test*';
%%%%%% Simulation config direcs
configDirecNameSim085 = 'config/Simulation/_085/Sample*';
configDirecNameSim030 = 'config/Simulation/_030/Sample*';
configDirecNameSim140 = 'config/Simulation/_140/Sample*';
configDirecNameSim195 = 'config/Simulation/_195/Sample*';
configDirecNameSim250 = 'config/Simulation/_250/Sample*';

%%%%%% Define desired test type here
testType = 'Modular';

switch testType
    case 'Modular'
        configDirecName = configDirecNameModular;
        testType = 'ModularTest_4_9';
    case '100m'
        configDirecName = configDirecName100m;
        testType = '3_25_100m';
    case 'Simulation_085'
        configDirecName = configDirecNameSim085;
        testType = 'Simulation/_085';
    case 'Simulation_030'
        configDirecName = configDirecNameSim030;
        testType = 'Simulation/_030';
    case 'Simulation_140'
        configDirecName = configDirecNameSim140;
        testType = 'Simulation/_140';
    case 'Simulation_195'
        configDirecName = configDirecNameSim195;
        testType = 'Simulation/_195';
    case 'Simulation_250'
        configDirecName = configDirecNameSim250;
        testType = 'Simulation/_250';
end

configfiles = dir(configDirecName);

for iter = 1:numel(configfiles)
    %%% Housekeeping and Allocation
    close all;
    rng(99);

    configfile = [pwd '/config/' testType '/' configfiles(iter).name];

    %%% Filenames and configurables
    manifestFilename = strcat(configfile,'/Manifest.json');
    SensorData = jsondecode(fileread(strcat(configfile,'/Sensors.json')));

    Model = VANTAGE.PostProcessing.Model(manifestFilename,configfile,true);

    fileLims = [1 inf];
    Model.Deployer = Model.TOF.TOFProcessing(SensorData,...
        Model.Deployer,'presentResults',0,'fileLims',fileLims,'showDebugPlots',0);

    % Process truth data
    Truth = Model.Truth_VCF;

    % Compute results
    Model.ComputeStateOutput();

    Validator = Validate(configfile,Model,true);

    CubeSatFitted = cell(Model.Deployer.numCubesats,1);
    TruthFitted = cell(Model.Deployer.numCubesats,1);
    AbsoluteError = cell(Model.Deployer.numCubesats,1);
    XError = cell(Model.Deployer.numCubesats,1);
    YError = cell(Model.Deployer.numCubesats,1);
    ZError = cell(Model.Deployer.numCubesats,1);

    data_t = Model.Deployer.CubesatArray(1).time(end);
    t_fit = linspace(0,data_t);
    for i=1:Model.Deployer.numCubesats
       CubeSat = Model.Deployer.CubesatArray(i);
       CubeSatFitted{i} = Validator.fitCubeSatTraj(CubeSat.centroids_VCF,CubeSat.time,'CS',t_fit,Model);
       TruthFitted{i} = interp1(Model.Truth_VCF.t,Model.Truth_VCF.Cubesat(i).pos,t_fit,'linear');
       [AbsoluteError{i},XError{i},YError{i},ZError{i}] = ...
           Validator.ProcessError(CubeSatFitted{i},TruthFitted{i});
    end

    % Save fitted results for error analysis later
    if strcmpi(testType,'ModularTest_4_9')
        dataFolder = 'Data/Results/matFiles/ModularTest_4_9/';
        folderString = Model.Deployer.TruthFileName;
        tmp = split(folderString,'/');
        testNumber = tmp{3};
    elseif strcmpi(testType,'100m')
        dataFolder = 'Data/Results/matFiles/100m/';
        folderString = Model.Deployer.TruthFileName;
        tmp = split(folderString,'/');
        testNumber = tmp{3};
    elseif strcmpi(testType,'Simulation_030')
        dataFolder = 'Data/Results/matFiles/Simulation_4_15_030/';
        tmp = split(SensorData.TOFData,'/');
        testNumber = tmp{5};
    elseif strcmpi(testType,'Simulation_085')
        dataFolder = 'Data/Results/matFiles/Simulation_4_15_085/';
        tmp = split(SensorData.TOFData,'/');
        testNumber = tmp{5};
    elseif strcmpi(testType,'Simulation_140')
        dataFolder = 'Data/Results/matFiles/Simulation_4_15_140/';
        tmp = split(SensorData.TOFData,'/');
        testNumber = tmp{5};
    elseif strcmpi(testType,'Simulation_195')
        dataFolder = 'Data/Results/matFiles/Simulation_4_15_195/';
        tmp = split(SensorData.TOFData,'/');
        testNumber = tmp{5};
    elseif strcmpi(testType,'Simulation_250')
        dataFolder = 'Data/Results/matFiles/Simulation_4_15_250/';
        tmp = split(SensorData.TOFData,'/');
        testNumber = tmp{5};
    else
        error('invalid test type in Deployer.TruthFileName')
    end

    mkdir(dataFolder)
    mkdir([dataFolder 'data/']);
    save([pwd '/' dataFolder 'data/CSData' testNumber '.mat'],'CubeSatFitted');
    save([pwd '/' dataFolder 'data/TruthData' testNumber '.mat'],'TruthFitted');
    save([pwd '/' dataFolder 'data/AbsErrorData' testNumber '.mat'],'AbsoluteError');
    save([pwd '/' dataFolder 'data/XErrorData' testNumber '.mat'],'XError');
    save([pwd '/' dataFolder 'data/YErrorData' testNumber '.mat'],'YError');
    save([pwd '/' dataFolder 'data/ZErrorData' testNumber '.mat'],'ZError');
    save([pwd '/' dataFolder 'data/CSTime' testNumber '.mat'],'t_fit');
end
%%%% Results processing
switch testType
    case 'ModularTest_4_9'
        configDirecName = configDirecNameModular;
        testType = 'ModularTest_4_9';
        testDef = 'Modular';
    case '100m'
        configDirecName = configDirecName100m;
        testType = '3_25_100m';
        testDef = '100m';
    case 'Simulation_085'
        configDirecName = configDirecNameSim085;
        testType = 'Simulation/_085';
        testDef = 'Sim085';
    case 'Simulation_030'
        configDirecName = configDirecNameSim030;
        testType = 'Simulation/_030';
        testDef = 'Sim030';
    case 'Simulation_140'
        configDirecName = configDirecNameSim140;
        testType = 'Simulation/_140';
        testDef = 'Sim140';
    case 'Simulation_195'
        configDirecName = configDirecNameSim195;
        testType = 'Simulation/_195';
        testDef = 'Sim195';
    case 'Simulation_250'
        configDirecName = configDirecNameSim195;
        testType = 'Simulation/_250';
        testDef = 'Sim250';
end
            
configfiles = dir(configDirecName);

Validator.ErrorAnalysis(Model,SensorData,testDef);

matFileDirectory = [pwd '/Data/Results/matFiles'];
Validator.GenerateOutputFiles(matFileDirectory);
%Validator.masterPlotter(matFileDirectory);

%{ 
Directories for Richard
MANIFEST LOCATION:
/home/vantage/Documents/githere/VANTAGE/config/ModularTest_4_9/Test5/Manifest.json

OUTPUT FILE LOCATION:
/home/vantage/Documents/githere/VANTAGE/Data/Results/matFiles/Modular/jsonOut/Test5_Modular.json

IMAGE DIRECTORY LOCATION:
/home/vantage/Documents/githere/VANTAGE/Data/ModularTest_4_9/Test5/Optical/VANTAGEOp20194_8_23_52_48_933.png
/home/vantage/Documents/githere/VANTAGE/Data/ModularTest_4_9/Test5/Optical/VANTAGEOp20194_8_23_52_49_293.png
%}


%% write results and input results filename and format in this code 
% the file here is the results file that the post processsing writes to
fid=fopen('config_simulation_template_2_25_Josh_ToF_Calibration_tube2_truth_data.json','r');
resultsfile=fread(fid);
fclose(fid);

%% Tell PI NUC is sending results
send(t_sig,resultsSig);
pause(0.2); % helps me visualize whats going on

%% Send Pi the results
send(t_data,resultsfile);

%% close sockets
fclose(t_sig)
fclose(t_data)

%% housekeeping
clear all
clc

% Turn off code here
% PepegaHands

%% functions used (If you can find premade functions, use them. I couldn't find any)
% function standby. NUC is on "standby" until the "signal" is received 
function output = standby(t,signal)
    while(1==1)
        if (t.BytesAvailable >0)
           received_data=fread(t,t.BytesAvailable);
           received_data=char(received_data');
           if(received_data(1:length(signal))==signal)
               break;
           end
        end
    end
end

% function receive. NUC "receives" data and returns the data
function output = receive(t)
data=[];
t_b=0; % t_b is fun

    while(1==1)
        t_b=(t_b+t_b)/2; % this is really fucking critical, DONT touch it
        if (t.BytesAvailable >0)
            received_data=fread(t,t.BytesAvailable);
            data_temp=[data , received_data'];
            data=data_temp;
            t_b=0;
        else
           t_b=t_b+1; 
        end
        if(t_b==100)
            break;
        end
    end
output=data;
end

% function send. NUC "sends" the "data" to the PI as a string 
function output = send(t,data)
buffcount=1;
buffsize=t.OutputBufferSize;
buffcount1=buffcount+buffsize;

while(buffcount1 < length(data))
    data_buff=data(buffcount:buffcount1-1);
    fwrite(t,data_buff);
    buffcount=buffcount1;
    buffcount1=buffcount+buffsize;
end
fwrite(t,data(buffcount:end));
pause(0.2)
end