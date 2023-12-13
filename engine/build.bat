@echo off
rem build script
SetLocal EnableDelayedExpansion

if not exist ../bin mkdir ../bin

rem find all .c files
set sources=
for /R %%f in (*.c) do (
	set sources=!sources! %%f
)

set output=arc
set compilerFlags=-g -shared -Wvarargs -Wall -Werror
set includeFlags=-Isrc -I%VULKAN_SDK%/Include
set linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
set defines=-DDEBUG -DEXPORT -D_CRT_SECURE_NO_WARNINGS

echo building...
set outputPath=../bin/%output%.dll
clang %sources% %compilerFlags% -o %outputPath% %defines% %includeFlags% %linkerFlags%
if not %ERRORLEVEL%==1 exit /b 1
