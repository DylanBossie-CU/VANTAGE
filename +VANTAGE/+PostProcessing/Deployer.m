classdef Deployer
    % Deployer is a class that stores the expected conditions of the deployment.
    % This includes geometric expectations as well as timing expectations.
    
    properties (SetAccess = public)
        
        % Array of cubesat objects
        CubesatArray = VANTAGE.PostProcessing.CubeSat.empty

    end
    properties (SetAccess = protected)
        % Number corresponding to the deployment tube housing VANTAGE
        VantageTube

        % Number corresponding to the deployment tube expected to launch
        DeploymentTube

        % Array of expected release time for each cubesat
        ExpectedRelease
        
        % Truth data file location
        TruthFileName
        
        % Type of test performed
        testScenario
    end
    properties (SetAccess = private)
        % Structure of deployer geometry
        DeployerGeometry

        % Cell array of tube geometry
        TubeGeometry

        % Number of expected cubesats
        numCubesats
    end

    methods
        % Class Constructor:
        %
        % @param      manifestFilename  The manifest filename
        % @param      configFilename    The configuration filename
        % @param      Model             Model class instance (@see Model)
        %
        % @return     A reference to an initialized Deployer object
        %
        function obj = Deployer(manifestFilename, configFilename, Model)
            % This constructor will need to read the deployment manifest file
            % and initialize an instance of the Deployer class. This constructor
            % should also create instances of the CubeSat class for every
            % expected cubesat that will be deployed.
            
            % Read data from manifest
            manifestData = jsondecode(fileread(manifestFilename));

            % Define test type
            obj.testScenario = manifestData.testScenario;
            % Extract truth file name
            obj.TruthFileName = manifestData.TruthDataFile;
            % Read deployer dimensions from config file

            % Initialize Deployer instance with tube positions and expected release time
            obj.DeploymentTube = manifestData.DeploymentTube;
            obj.VantageTube = manifestData.VantageTube;
            obj.ExpectedRelease = datetime(...
                manifestData.expectedRelease(1),...
                manifestData.expectedRelease(2),...
                manifestData.expectedRelease(3),...
                manifestData.expectedRelease(4),...
                manifestData.expectedRelease(5),...
                manifestData.expectedRelease(6),...
                manifestData.expectedRelease(7));

            I = [manifestData.CubesatArray.rangeOrder];
            % Initialize cubesat array
            for i = I
                obj.CubesatArray(1,i) = VANTAGE.PostProcessing.CubeSat(...
                    manifestData.CubesatArray(i).name,...
                    manifestData.CubesatArray(i).rangeOrder,...
                    manifestData.CubesatArray(i).expectedU,...
                    manifestData.CubesatArray(i).actualDims);
            end

            % Read data from deployer configuration file
            obj.DeployerGeometry = jsondecode(fileread(configFilename));
            
            % Assign CubeSat number
            obj.numCubesats = length(obj.CubesatArray);
        end

        % A method for getting the initial states of all cubesats.
        %
        % @param      obj   The object
        %
        % @return     pos_init an Nx3 matrix of cubesat positions in VCF
        % @return     V_init an Nx3 matrix of cubesat velocities in VCF
        %
        function [pos_init, V_init] = GetInitalStates(obj)
            


        end

        % A method for getting the expected number of cubesats
        % 
        % @param      obj   The object
        %
        % @return     The expected number of cubesats
        %
        function [numCubesats] = GetNumCubesats(obj)
            numCubesats = obj.numCubesats;
        end

        % A method for getting the camera origin
        % @param      obj   The object
        %
        % @return     the camera origin
        %
        function [camOrigin] = GetCamOriginVCF(obj)
            camOrigin = obj.DeployerGeometry.CAMERA_LOC;
        end

        % A method for getting the VANTAGE tube number
        % @param      obj   The object
        %
        % @return     the VANTAGE tube number
        %
        function [VantageTube] = GetVantageTube(obj)
            VantageTube = obj.VantageTube;
        end

        % A method for getting the expected deployment tube
        % @param      obj   The object
        %
        % @return     the expected deployment tube
        %
        function [DeploymentTube] = GetDeploymentTube(obj)
            DeploymentTube = obj.DeploymentTube;
        end
    end
end

