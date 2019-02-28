function varargout = execute_TOF_Test
import matlab.unittest.TestSuite;

TOF_test = TestSuite.fromClass(?VANTAGE.Test.PostProcessing.Test_TOF);

if nargout == 1
    varargout{1} = TOF_test;
else
    clc
    run(TOF_test)
end
