function varargout = suite
import matlab.unittest.TestSuite;
import VANTAGE.Test.*;

testsuite = [suitePostProcessing];

if nargout == 1
    varargout{1} = testsuite;
else
    clc
    run(testsuite);
end

end