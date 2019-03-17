function TOF_Results = execute_TOF_Test
import matlab.unittest.TestSuite;

TOF_test = TestSuite.fromClass(?VANTAGE.Test.PostProcessing.Test_TOF);

TOF_Results = run(TOF_test);
