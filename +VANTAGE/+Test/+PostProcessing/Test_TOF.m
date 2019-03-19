classdef Test_TOF < matlab.unittest.TestCase
    %% Setup
    properties
        validationData
        Model
    end
    
    methods (TestClassSetup)
        %
        %
        function setupValidationData(testcase)
            load('+VANTAGE/+Test/+PostProcessing/validationData_TOF.mat');
            if ~exist('validationData')
                error('Loading validationData from validationData_TOF.mat failed')
            end
            if ~exist('Model')
                error('Loading Model from validationData_TOF.mat failed')
            end
            testcase.validationData = validationData;
            testcase.Model = Model;
        end
    end
    
    methods (TestMethodTeardown)
    end
    %% Test methods
    methods (Test)
        
        %
        % Test binarySign
        function testBinarySign(testcase)
            if ~isfield(testcase.validationData,'binarySignData')
                error('validation data does not exist for this test')
            end
            if isempty(testcase.validationData.binarySignData)
                error('validation data is empty for this test')
            end
            testVals       = testcase.validationData.binarySignData.testVals;
            validationVals = testcase.validationData.binarySignData.validationVals;
            for i = 1:length(testcase.validationData.binarySignData.testVals)
                testcase.verifyEqual(validationVals(i),testcase.Model.TOF.binarySign(testVals(i)));
            end
        end
        
        %
        % Test roundToNearestOdd
        function testRoundToNearestOdd(testcase)
            if ~isfield(testcase.validationData,'roundToNearestOddData')
                error('validation data does not exist for this test')
            end
            if isempty(testcase.validationData.roundToNearestOddData)
                error('validation data is empty for this test')
            end
            testVals       = testcase.validationData.roundToNearestOddData.testVals;
            validationVals = testcase.validationData.roundToNearestOddData.validationVals;
            for i = 1:length(testcase.validationData.roundToNearestOddData.testVals)
                testcase.verifyEqual(validationVals(i),testcase.Model.TOF.roundToNearestOdd(testVals(i)));
            end
        end
        
        %
        % Test minBoundingBox
        function testMinBoundingBox(testcase)
            if ~isfield(testcase.validationData,'minBoundingBoxData')
                error('validation data does not exist for this test')
            end
            if isempty(testcase.validationData.minBoundingBoxData)
                error('validation data is empty for this test')
            end
            testVals       = testcase.validationData.minBoundingBoxData.testVals;
            validationVals = testcase.validationData.minBoundingBoxData.validationVals;
            for i = 1:length(testcase.validationData.minBoundingBoxData.testVals)
                testcase.verifyEqual(validationVals{i},testcase.Model.TOF.minBoundingBox(testVals{i}));
            end
        end
            
        %
        % Test unitvec
        function testUnitvec(testcase)
            if ~isfield(testcase.validationData,'unitvecData')
                error('validation data does not exist for this test')
            end
            if isempty(testcase.validationData.unitvecData)
                error('validation data is empty for this test')
            end
            testVals       = testcase.validationData.unitvecData.testVals;
            validationVals = testcase.validationData.unitvecData.validationVals;
            for i = 1:length(testcase.validationData.unitvecData.testVals)
                testcase.verifyEqual(validationVals{i},testcase.Model.TOF.unitvec(testVals{i}));
            end
        end
        
        %
        % Test distInFromFace
        
        
         
        % For development
        function testTOFProcessing(testcase)
            return
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
