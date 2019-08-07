::
::      Implements batch-commands to compile GLSL-files to SPIR-V format
::
::      @author       D3PSI
::      @version      0.0.1 02.12.2019
::
::      @file         compile.bat
::      @brief        Implementation of batch-commands to compile GLSL-files to SPIR-V format
::

../../../ext/vulkan/Linux/binglslangValidator -V shader.vert
../../../ext/vulkan/Linux/binglslangValidator -V shader.frag
pause