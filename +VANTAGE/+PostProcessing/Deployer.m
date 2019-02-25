classdef Deployer
    % Deployer is a class that stores the expected conditions of the deployment.
    % This includes geometric expectations as well as timing expectations.
    
    properties (SetAccess = public)

    end
    properties (SetAccess = protected)
        % Number corresponding to the deployment tube housing VANTAGE
        VantageTube

        % Number corresponding to the deployment tube expected to launch
        DeploymentTube

        % Array of cubesat objects
        CubesatArray = VANTAGE.PostProcessing.CubeSat.empty

        % Array of expected release time for each cubesat
        ExpectedRelease
    end
    properties (SetAccess = private)
        % Structure of deployer geometry
        DeployerGeometry

        % Cell array of tube geometry
        TubeGeometry
    end

    methods
        % Class Constructor:
        %
        % @param      manifestFilename  The manifest filename
        % @param      configFilename    The configuration filename
        %
        % @return     A reference to an initialized Deployer object
        %
        function obj = Deployer(manifestFilename, configFilename)
            % This constructor will need to read the deployment manifest file
            % and initialize an instance of the Deployer class. This constructor
            % should also create instances of the CubeSat class for every
            % expected cubesat that will be deployed.
            
            % Import required classes
            import VANTAGE.PostProcessing.CubeSat
            
            % Read data from manifest
            manifestData = jsondecode(fileread(manifestFilename));

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

            % Calculate cubesat initial positions
            numCubesats = numel(manifestData.CubesatArray);
            pos_init = zeros(numCubesats,3);

            % Initialize cubesat array
            obj.CubesatArray(1,numCubesats) = CubeSat(...
                manifestData.CubesatArray(numCubesats).name,...
                manifestData.CubesatArray(numCubesats).rangeOrder,...
                pos_init(numCubesats,:),...
                manifestData.CubesatArray(numCubesats).expectedU,...
                manifestData.CubesatArray(numCubesats).actualDims);
            for i = 1:numCubesats-1
                obj.CubesatArray(1,i) = CubeSat(...
                    manifestData.CubesatArray(i).name,...
                    manifestData.CubesatArray(i).rangeOrder,...
                    pos_init(i,:),...
                    manifestData.CubesatArray(i).expectedU,...
                    manifestData.CubesatArray(i).actualDims);
            end

            % Read data from deployer configuration file
            obj.DeployerGeometry = jsondecode(fileread(configFilename));
            
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
    end
end

