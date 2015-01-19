rem script for copy lib-files from contrib to project\lib directory

setlocal enableextensions enabledelayedexpansion

@echo off

pushd..
set projects_dir=%cd%
popd

set project_lib_dir=%projects_dir%\lib
set contrib_lib=%projects_dir%\contrib\lib

rem create project lib directory if it does not exist
if not exist "%project_lib_dir%" (
    echo lib dir does not exist, create it
    mkdir %project_lib_dir%
)

rem extract library files from archive contrib\lib 
rem and copy them to projects/lib
pushd..
cd %contrib_lib%

for %%i in (%contrib_lib%\*.tar.gz) do (
    set lib_achive=%%~nxi
    set tar_archive=%%~ni
    rem archive name without extentions (without .tar.gz)
    set lib_dir_name=!tar_archive:~0,-4!
   
    call gzip.exe -d -c !lib_achive! > !tar_archive! & tar.exe -xvf !tar_archive!
    xcopy !lib_dir_name! %project_lib_dir%\!lib_dir_name! /e /y /i
    
    rmdir !lib_dir_name! /S /Q
    del !tar_archive! /Q
) 
popd

echo development libraries have been copied successfully