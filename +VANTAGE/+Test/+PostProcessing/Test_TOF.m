classdef Test_TOF < matlab.unittest.TestCase
    %% Setup
    properties
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    %% Test methods
    methods (Test)
         
        function testTOFProcessing(testcase)
            tube = 6;
            switch tube
                case 1
                    truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube1/config_simulation_template_2_25_Josh_ToF_Calibration_tube1_truth_data.json';
                    manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Manifest_TOFdev.json';
                    SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube1/Sensors_TOFdev.json'));
                case 6
                    truthFilename = 'Data/Simulation_TOF-Truth_3-3-19_Tube6/config_simulation_template_2_25_Josh_ToF_Calibration_tube6_truth_data.json';
                    manifestFilename = 'Config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Manifest_TOFdev.json';
                    SensorData = jsondecode(fileread('config/Testing/TOF/Simulation_TOF-Truth_3-3-19_Tube6/Sensors_TOFdev.json'));
                otherwise
                    error('unimplemented tube requested')
            end
            Model = VANTAGE.PostProcessing.Model(manifestFilename,truthFilename);
            Deployer = Model.Deployer;
            TOF = VANTAGE.PostProcessing.TOF(Model,'Config/TOF.json');
            [Deployer,naiveCentroidsForDylan] = TOF.TOFProcessing(SensorData,Deployer,'fileLims',[30,35],'presentResults',1,'showDebugPlots',0);
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
