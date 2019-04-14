
fid=fopen('config_simulation_template_2_25_Josh_ToF_Calibration_tube2_truth_data.json');
data_temp=fread(fid);
fclose(fid)
data=data_temp(1:100000);
% set up socket
%data='aaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbccccccccccccccccccccccccccddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeefffffffffffffffffffgggggggggggggggggggggggghi'
t = tcpip('169.254.146.34', 4013, 'NetworkRole', 'client');
% send data
t.OutputBufferSize=128;
fopen(t);
%t.OutputBufferSize=128;
buffcount=1;
buffcount1=buffcount+128;

while(buffcount1 < length(data))
    data_buff=data(buffcount:buffcount1-1);
    fwrite(t,data_buff);
    buffcount=buffcount1;
    buffcount1=buffcount+128;
end
    fwrite(t,data(buffcount:end));
% recieved data
% keep trying until recieved signal
while(1==1)
    if (t.BytesAvailable >0)
        manif=fread(t,t.BytesAvailable)';
        break;
    end
end
t_b=0; % t_b is fun
while(1==1)
    t_b=(t_b+t_b)/2; % this is really fucking critical, dont touch it
    if (t.BytesAvailable >0)
        received_data=fread(t,t.BytesAvailable);
        manif_temp=[manif , received_data'];
        manif=manif_temp;
        t_b=0;
    else
       t_b=t_b+1; 
    end
    if(t_b==100)
        break;
    end
end
manif=char(manif)