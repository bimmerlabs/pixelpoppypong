@ECHO Off
SET COMPILER_DIR=..\..\Compiler
SET PATH=%COMPILER_DIR%\WINDOWS\Other Utilities;%COMPILER_DIR%\WINDOWS\bin;%PATH%
REM uncomment and update path to auto-build your palettes etc:
REM perl "M:\Saturn\Development\_Tools\TGA_Tools\tga_tools.pl" --p "M:\Saturn\Development\03_joengine-master\Projects\demo - HSL color calc - Pixel Poppy" --in_image HEXAGON
make re