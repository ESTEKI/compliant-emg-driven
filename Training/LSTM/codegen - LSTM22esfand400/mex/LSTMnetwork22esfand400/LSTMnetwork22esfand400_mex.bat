@echo off
call setEnv.bat
"D:\Programs\MATLAB21\Polyspace\toolbox\shared\coder\ninja\win64\ninja.exe" -t compdb cc cxx cudac > compile_commands.json
"D:\Programs\MATLAB21\Polyspace\toolbox\shared\coder\ninja\win64\ninja.exe" -v %*
