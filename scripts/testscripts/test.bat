@echo off
if not '%1'=='' goto continue

echo Runs testing application
echo Parameter 1 - Path to testing application
echo Parameter 2 - Platform, i.e. x64
echo Parameter 3 - Configuration, i.e. Release
exit 1

:continue

if '%SSUSER%' == 'buildbot' goto exit

set TargetPlatform=%2
@echo Target Platform = %TargetPlatform%
if defined ProgramFiles(x86) goto OS64
@echo 32-bit OS
if '%TargetPlatform%' == 'x64' goto exit
goto EndPlatformCheck
:OS64
@echo 64-bit OS
:EndPlatformCheck

if not exist %1.xml goto nodelneed
del %1.xml
:nodelneed

if not exist preteststep.bat goto noprepareneed
echo Executing user prepare script
if '%2'=='' goto presteperror
if '%3'=='' goto presteperror
call preteststep %2 %3
if errorlevel 1 (goto presteperror) else echo OK
:noprepareneed

%1 --gtest_output=xml:%1.xml --gtest_shuffle

echo:
echo ViPNet Unit Test Utility finished with exit code: %errorlevel%

if %errorlevel% neq 0 exit /b %errorlevel%

if exist %1.xml goto exit

:error
echo Error: Test results not found. Check if the tests run.
exit /b 1

:presteperror
echo Error: Can't execute requered user script
exit /b 1

:exit
rem If running from TeamCity, then return zero exit code to let TeamCity determine whether the tests were successful.
if not 'TEAMCITY_VERSION'=='' exit /b 0

