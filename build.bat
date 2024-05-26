@echo off
color C

:: Create build directory and navigate to it
mkdir build
cd build

:: Generate Visual Studio solution using CMake
cmake -G "Visual Studio 17 2022" -A Win32 ..
if %ERRORLEVEL% neq 0 (
	echo Error executing CMake!
	ping 127.0.0.1 -n 3 > nul
	exit /b %ERRORLEVEL%
)

:: Build the project using the generated Visual Studio solution in Release mode
cmake --build . --config Release
if %ERRORLEVEL% neq 0 (
	echo Error building the injector!
	ping 127.0.0.1 -n 3 > nul
	exit /b %ERRORLEVEL%
)

cls
color 2

:: Get the current directory
for %%I in (.) do set "CURRENT_DIR=%%~fI"
echo The executable is located at: %CURRENT_DIR%\Release

pause
exit