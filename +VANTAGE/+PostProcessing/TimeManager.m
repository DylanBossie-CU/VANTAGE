classdef TimeManager
    %% Classdef
    % A class used to convert between various time frames.  A limitation is
    % that all truth, tof, and optical times (date strings) are expected to
    % be in the same time zone.
    %
    %% Properties
    properties(Access = public)
        %
        % TOF Date Format
        TofDateFormat = 'yyyy_mm_dd_HH_MM_SS.FFF'
        
        % Truth Date Format
        TruthDateFormat = 'dd-mmm-yyyy HH:MM:SS.FFF'
        
        % Optical Date Format
        OpticalDateFormat = 'yyyy_m_dd_HH_MM_SS_FFF'
        
        % Desired Output Date Format for VANTAGE
        VantageDateFormat = 'dd-mmm-yyyy HH:MM:SS.FFF'
        
        % Datevec corresponding to t=0s
        DatevecZero
        
        ModelRef
    end
    
    %% Methods
    methods
        % Class Constructor
        %
        % Constructs class using Transform.json config file
        %
        % @return   Class instance
        % 
        % @author   Joshua Kirby
        % @date     06-Apr-2019
        function obj = TimeManager(SensorData,testType,ModelRef)
            % Get DateVecZero
            ls = dir(SensorData.TOFData);
            ls = ls([ls.bytes]~=0);
            try
            obj.DatevecZero = obj.datevecFromTofFilename(ls(1).name,testType);
            catch
                disp('go fuck urself')
            end
            obj.ModelRef = ModelRef;
        end
        
        %
        % Synchronize truth data timestamps such that their t=0s
        % corresponds to DatevecZero
        %
        % @param    ModelRef    Reference to Model class instance,
        %                       containing a populated Truth_VCF
        %
        % @author   Joshua Kirby
        % @date     06-Apr-2019
        function syncTruthData(obj,ModelRef)
            dv0_truth = ModelRef.Truth_VCF.t0_datevec;
            offset_sec = etime(obj.DatevecZero,dv0_truth);
            if offset_sec < 0
                warning('Truth data starts after TOF data')
            end
            
            if abs(offset_sec) > 10000
                offset_sec = 0;
            end
            ModelRef.Truth_VCF.t = ModelRef.Truth_VCF.t - offset_sec;
            dv0_updated = dv0_truth;
            dv0_updated(6) = dv0_truth(6) + offset_sec;
            dv0_str = datestr(dv0_updated,obj.VantageDateFormat);
            ModelRef.Truth_VCF.t0_datevec = datevec(dv0_str,obj.VantageDateFormat);
        end
        %
        % Extract datevec from TOF point cloud filename
        %
        % @param    TOF_filename    TOF point cloud filename
        %
        % @return   datevec corresponding to this point cloud
        %
        % @author   Joshua Kirby
        % @date     06-Apr-2019
        function [dv] = datevecFromTofFilename(obj,TOF_filename,testType)
            TOF_filename = char(TOF_filename);
            if strcmpi(testType,'Simulation')
                ds = '0000_00_00_00_00_00.000';
            else
                ds = TOF_filename(12:end-4);
            end
            dv = datevec(ds,obj.TofDateFormat);
        end
        
        %
        % Extract datevec from Optical Image filename
        %
        % @param    Opt_filename    Optical image filename
        %
        % @return   datevec corresponding to this image
        %
        % @author   Joshua Kirby
        % @date     06-Apr-2019
        function [dv] = datevecFromOpticalFilename(obj,Opt_filename,TestType)
            Opt_filename = char(Opt_filename);
            if strcmpi(TestType,'Simulation')
                ds = '0000_0_00_00_00_0_000';
            else
                ds = [Opt_filename(10:13),'_',Opt_filename(14:end-4)];
            end
            dv = datevec(ds,obj.OpticalDateFormat);
        end
        
        %
        % Return time [s] in the global VANTAGE time frame given data
        % filenames of a consistent type
        %
        % @param    filenames        length-n cell or string of TOF or Optical
        %                           filenames
        % @param    filetype        'TOF' or 'Optical'
        %
        % @return   global VANTAGE time vector [s]
        %
        % @author   Joshua Kirby
        % @date     06-Apr-2019
        function [t] = VantageTime(obj,filenames,filetype,TestType)
            filenames = string(filenames);
            t = zeros(length(filenames),1);
            if ~strcmpi(TestType,'simulation')
                for i = 1:length(filenames)
                    if strcmpi(filetype,'TOF')
                        dv = obj.datevecFromTofFilename(filenames(i),TestType);
                    elseif strcmpi(filetype,'Optical')
                        dv = obj.datevecFromOpticalFilename(filenames(i),TestType);
                    else
                        error('filetype must be ''TOF'' or ''Optical''')
                    end
                    t(i) = etime(dv,obj.DatevecZero);
                end
            else
                for i = 1:length(filenames)
                    if strcmpi(filetype,'TOF')
                        tmp = char(filenames(i));
                        filenum = str2num(tmp(end-8:end-4));
                        tofFps = 10;
                        try
                        t(i) = filenum/tofFps;
                        catch
                            disp('awoeinawoef')
                        end
                    elseif strcmpi(filetype,'Optical')
                        tmp = char(filenames(i));
                        filenum = str2num(tmp(end-7:end-4));
                        opticalFps = 2;
                        t(i) = filenum/opticalFps;
                    else
                        error('filetype must be ''TOF'' or ''Optical''')
                    end
                end
            end
        end
        
        %
        % Convert a vector of VANTAGE global times [s] to datestrings
        %
        % @param    t   length-n vector of VANTAGE global times [s]
        %
        % @return   length-n string array of associated date strings
        %           formatted in TimeManager.VantageDateFormat
        %
        % @author   Joshua Kirby
        % @date     07-Apr-2019
        function [ds] = VantageTime2DateStr(obj,t)
            ds = strings(size(t));
            for i = 1:length(t)
                dv    = obj.DatevecZero;
                dv(6) = dv(6) + t(i);
                ds(i) = string(datestr(dv,obj.VantageDateFormat));
            end
        end
    end
    
    
    
    
    
    
end