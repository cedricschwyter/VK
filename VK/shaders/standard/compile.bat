::
::      Implements batch-commands to compile GLSL-files to SPIR-V format
::
::      @author        D3PSI
::      @version        0.0.1 02.12.2019
::
::      @file            compile.bat
::      @brief        Implementation of batch-commands to compile GLSL-files to SPIR-V format
::

C:/VulkanSDK/1.1.85.0/Bin32/glslangValidator.exe -V shader.vert
C:/VulkanSDK/1.1.85.0/Bin32/glslangValidator.exe -V shader.frag
pause