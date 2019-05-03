function docGenDriver
    % Builds MATLAB documentation based on a command-line driven input
    % scheme.
    % 
    % The generated documentation is based on the mtoc++ package, found at 
    % http://www.ians.uni-stuttgart.de/MoRePaS/software/mtocpp/docs/index.html
    % . It converts the matlab files to cpp files, then uses doxygen and dot
    % to automatically build documentation and call/caller graphs.
    %
    % To access the documentation, go to the output directory you specify
    % (<tt>../DoxygenDocs/output</tt> as shown below) and open
    % any of the HTML files in a browser.
    %
    % @section dirStruct Recommended Directory Structure
    % Recommended that you have the following directory structure when
    % generating your documentation
    %
    % @code
    % ../DoxygenDocs/docGenDriver.m
    % ../DoxygenDocs/MatlabDocMaker.m
    % ../DoxygenDocs/config/developers.c
    % ../DoxygenDocs/config/Doxyfile.template
    % ../DoxygenDocs/config/latexextras.template
    % ../DoxygenDocs/config/mtocpp.conf
    % ../DoxygenDocs/output
    % ../DoxygenDocs/$(PROJECT_LOGO)
    % ../$(PROJECT_DIRECTORY)
    % @endcode
    % 
    % @section use Instructions for use (command-line walkthrough)
    %   When prompted for a 'projects source directory':
    %     on your first run, enter 'n' and then choose
    %     <tt> \verbatim ../$(PROJECT_DIRECTORY) \endverbatim </tt>
    %     in the file browser if using the recommended directory structure.
    %
    %   When prompted for a 'documentation configuration files directory':
    %     on your first run, enter 'n' and then choose
    %     <tt> ../DoxygenDocs/config </tt>
    %     in the file browser if using the recommended directory structure.
    %
    %   When prompted for a 'documentation output directory':
    %     on your first run, enter 'n' and then choose
    %     <tt> ../DoxygenDocs/output </tt>
    %     in the file browser if using the recommended directory structure.
    %
    %   - After your first new run, it should remember the directories you
    %     set, so you should be able to enter 'y' when re-running this
    %     script.Directions 
    %
    % @warning
    % You will need the following binaries accessible on your system path:
    %   - doxygen(>=1.8.1)
    %   - mtocpp
    %   - mtocpp_post
    %   - dot (graphviz tool)
    %   - latex
    %   - ghostscipt
    %
    % @attention
    % Change the name of the project in the local MatlabDocMaker.m script
    %
    % @author 
    % Nicholas Renninger
    %
    % @section Example Usage
    % @code
    % docgen()
    % @endcode
    %
    % REVISION HISTORY
    % @date 06 Apr 2019 - Nicholas Renninger
    % - Initial creation and testing    
    
    docGen = MatlabDocMaker;

    % baseDocPath is your "documentation
    % configuration files directory". View the generated documentation in a web
    % browser by looking at it in a web browser.
    docGen.setup();
    docGen.create();
    
    % This will open the documentation automatically in your default
    % browser, but it produces some pathing errors if you run it multiple
    % times. Use at your own risk
%     docGen.open();
    
end