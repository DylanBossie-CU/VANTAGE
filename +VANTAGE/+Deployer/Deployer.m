classdef Deployer
    % Deployer is a class that stores the expected conditions of the deployment.
    % This includes geometric expectations as well as timing expectations.
    
    properties (SetAccess = public)

    end
    properties (SetAccess = protected)
        % Number corresponding to the deployment tube expected to launch
        DeploymentTube

        % Array of cubesat objects
        CubesatArray

        % Array of expected release time for each cubesat
        ExpectedRelease
    end
    properties (SetAccess = private)
        % List of deployer geometry
        DeployerGeometry

        % Cell array of tube geometry
        TubeGeometry
    end
    
    % A method for getting the initial states of all cubesats.
    %
    % @param      obj   The object
    %
    % @return     pos_init an Nx3 matrix of cubesat positions in VCF
    % @return     V_init an Nx3 matrix of cubesat velocities in VCF
    %
    methods
        function [pos_init, V_init] = GetInitalStates(obj)
            


        end
    end
end

