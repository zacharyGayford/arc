@echo off
rem build script
SetLocal EnableDelayedExpansion

if not exist ../bin mkdir ../bin

rem find all .c files
set sources=
for /R %%f in (*.c) do (
	set sources=!sources! %%f
)

set output=arc-test
set compilerFlags=-g
set includeFlags=-Isrc -I../engine/src/
set linkerFlags=-L../bin/ -larc.lib
set defines=-DDEBUG -DIMPORT

echo building...
clang %sources% %compilerFlags% -o ../bin/%output%.exe %defines% %includeFlags% %linkerFlags%
if not %ERRORLEVEL%==1 exit /b 1

pushd ..\bin
if exist %output%.exe %output%.exe
popd
