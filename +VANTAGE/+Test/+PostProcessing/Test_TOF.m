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
         
        function testNaiveFindCentroids(testCase)
            
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
