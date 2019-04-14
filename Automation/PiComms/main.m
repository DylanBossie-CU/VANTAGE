


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
% Automation Code here:   %



%                         %
%% Crop out data so we can simulate realistic data (no cars near ISS)


%% Start postprocess shit
% Post Process Code here: % 



%                         %


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