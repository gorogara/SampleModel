@echo off
:: script is supposed to be executed in /build directory

set modelName="MNav"

if exist x64\Release\%modelName%.dll goto DLL_EXISTS
echo "ERROR: File %modelName%.dll expected in directory x64\Release\%modelName%.dll, but does not exist.
exit /b 1
:DLL_EXISTS

:: remove target directory if it exists
if not exist package goto DIRECTORY_CLEAN
echo Removing existing directory 'package'
rd /S /Q "package"
:DIRECTORY_CLEAN

:: remove target FMU if it exists
if not exist %modelName%.fmu goto FMU_REMOVED
echo Removing existing FMU file '%modelName%.fmu'
del /F /S /Q "%modelName%.fmu"
:FMU_REMOVED

::create subdir and change into it
mkdir package

cd package

:: create binary dir for Windows
mkdir binaries\win64

:: copy shared library, we expect it to be already renamed correctly
xcopy ..\x64\Release\%modelName%.dll binaries\win64\
xcopy ..\modelDescription_MNav.xml .
rename modelDescription_MNav.xml modelDescription.xml
echo Created FMU directory structure

::change working directory back to original dir
cd ..

::create zip archive
echo Creating archive '%modelName%.zip'
cd package
7z a ../%modelName%.zip .
cd ..

echo Renaming archive to '%modelName%.fmu'
rename %modelName%.zip %modelName%.fmu

:: all ok
exit /b 0
