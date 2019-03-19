classdef Test_TOF < matlab.unittest.TestCase
    %% Setup
    properties
        validationData = load('+VANTAGE/+Test/+PostProcessing/validationData_TOF.mat');
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    %% Test methods
    methods (Test)
        
        function testBinarySign(testcase)
            % Initialize Model using a simulated test case
            configDirecName = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1';
            truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube1/config_simulation_template_2_25_Josh_ToF_Calibration_tube1_truth_data.json';
            manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Manifest_TOFdev.json';
            Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename,configDirecName);
            TOF = Model.TOF;
            % Test binarySign
            testVals       = testcase.validationData.binarySignData.testVals;
            validationVals = testcase.validationData.binarySignData.validationVals;
            for i = 1:length(testcase.validationData.binarySignData.testVals)
                testcase.verifyEqual(validationVals(i),TOF.binarySign(testVals(i)));
            end
        end
        
        function testRoundToNearestOdd(testcase)
            
        end
            
        
         
        % For development
        function testTOFProcessing(testcase)
            % housekeeping
            close all;clearvars
            rng(99);
            tube = 6;
            fileLims = [1 inf];
            switch tube
                case 1
                    configDirecName = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1';
                    truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube1/config_simulation_template_2_25_Josh_ToF_Calibration_tube1_truth_data.json';
                    manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Manifest_TOFdev.json';
                    SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Sensors.json'));
                case 6
                    configDirecName = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6';
                    truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube6/config_simulation_template_2_25_Josh_ToF_Calibration_tube6_truth_data.json';
                    manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Manifest_TOFdev.json';
                    SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Sensors.json'));
                otherwise
                    error('unimplemented tube requested')
            end
            Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename,configDirecName);
            Model.Deployer = Model.TOF.TOFProcessing(SensorData,Model.Deployer,'presentResults',0,'fileLims',fileLims,'showDebugPlots',0);
        end

    end
    
    %% Private methods
    methods (Access = private)
        
        function checkStruct(testCase, oldStruct, newStruct)
            
            desiredFieldNames = fieldnames(oldStruct);
            numDesiredFieldNames = length(desiredFieldNames);
            
            for jj = 1:numDesiredFieldNames

                % this test will fail if the new struct is missing a field
                hasFieldName = isfield(oldStruct, desiredFieldNames{jj});
                testCase.verifyTrue(hasFieldName);

                % once you know that the new stuct has the field, check the
                % values
                if hasFieldName
                    truthData = oldStruct.(desiredFieldNames{jj});
                    testData = newStruct.(desiredFieldNames{jj});

                    testCase.verifyEqual(testData, truthData, ...
                                         desiredFieldNames{jj})
                end
            end 
        end
        
    end

end
