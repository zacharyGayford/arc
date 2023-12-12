@echo off
pushd engine
echo engine:
call build.bat
popd
pushd test
echo test:
call build.bat
popd
