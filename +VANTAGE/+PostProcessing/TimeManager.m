classdef TimeManager
    %% Classdef
    % A class used to convert between various time frames.  A limitation is
    % that all truth, tof, and optical times (date strings) are expected to
    % be in the same time zone.
    %
    %% Properties
    properties(Access = public)
        %
        % struct containing data on converting between various time frames
        TDATA
        
        % cell mapping frame names to indices in TDATA
        nomen
        
        % maximum allowable number of nomen entries (number of defined frames)
        maxNomen = 10
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
        function obj = TimeManager(SensorData)
            % Get DateVecZero
            ls = dir(SensorData.TOFData);
            ls = ls([ls.bytes]~=0);
            obj.DatevecZero = obj.datevecFromTofFilename(ls(1).name);
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
        function [dv] = datevecFromTofFilename(obj,TOF_filename)
            TOF_filename = char(TOF_filename);
            ds = TOF_filename(12:end-4);
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
        function [dv] = datevecFromOpticalFilename(obj,Opt_filename)
            Opt_filename = char(Opt_filename);
            ds = [Opt_filename(10:13),'_',Opt_filename(14:end-4)];
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
        function [t] = VantageTime(obj,filenames,filetype)
            filenames = string(filenames);
            t = zeros(length(filenames),1);
            for i = 1:length(filenames)
                if strcmpi(filetype,'TOF')
                    dv = obj.datevecFromTofFilename(filenames(i));
                elseif strcmpi(filetype,'Optical')
                    dv = obj.datevecFromOpticalFilename(filenames(i));
                else
                    error('filetype must be ''TOF'' or ''Optical''')
                end
                t(i) = etime(dv,obj.DatevecZero);
            end
        end
    end
    
    
    
    
    
    
end