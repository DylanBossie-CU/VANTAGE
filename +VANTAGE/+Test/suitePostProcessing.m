function varargout = suitePostProcessing
import matlab.unittest.TestSuite;

suite = TestSuite.fromPackage('VANTAGE.Test.PostProcessing');

if nargout == 1
    varargout{1} = suite;
else
    clc
    run(suite);
end
end