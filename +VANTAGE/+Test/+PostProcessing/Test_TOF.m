classdef Test_TOF < matlab.unittest.TestCase
    properties
    end
    
    methods (TestClassSetup)
    end
    
    methods (TestMethodTeardown)
    end
    
    methods (Test)
        
        function testInputDataFcn(testCase)
            import SWEPT.file.legacy.inputDataFcn
            import SWEPT.util.loadInComparisonData

            fileExts = '.mat';
            inDataRelDir = 'input_inputDataFcn';
            outDataRelDir = 'output_inputDataFcn';
            
            % the data dirs with the inputs and desired outputs should be
            % in the same dir as this test class
            [testDir, ~, ~] = fileparts(mfilename('fullpath'));
            inDataDir = fullfile(testDir, inDataRelDir);
            outDataDir = fullfile(testDir, outDataRelDir);

            [inp, out] = loadInComparisonData(inDataDir, ...
                                              outDataDir, ...
                                              fileExts);
            numTestCases = length(out);

            % checking that the fields are the same for each set of inputs and
            % outputs from the old tool
            for ii = 1:numTestCases
                
                currInp = inp(ii);
                currOut = out(ii);
                inputData = currOut.inputData;

                [inputData_new, ...
                 arrayPointerStructVar_new] = inputDataFcn(currInp.optionMeas);

                
                testCase.checkStruct(inputData, inputData_new)
            end
            
        end

    end
    
    
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
