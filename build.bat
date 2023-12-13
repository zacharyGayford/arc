@echo off
if not exist bin mkdir bin
pushd engine

echo engine:
call build.bat
if not %ERRORLEVEL%==0 exit /b 1

popd
pushd test

echo test:
call build.bat
if not %ERRORLEVEL%==0 exit /b 1

popd
