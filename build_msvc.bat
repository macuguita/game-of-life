@echo off

set PROGRAM_NAME=game_of_life
set CFLAGS=/Wall /std:c17 /MT
set INCLUDES=/I.\thirdparty /I.\thirdparty\msvc-raylib\include
set ARCH=/MACHINE:X64

REM Set up Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
if errorlevel 1 (
    exit /b 1
)

cl 2>&1 | findstr x64 >nul
if errorlevel 1 (
    exit /b 1
)

REM Clean and create build directory
if exist build rmdir /s /q build
mkdir build

REM Compile resources (icon)
rc /fo build\%PROGRAM_NAME%.res %PROGRAM_NAME%.rc
if errorlevel 1 (
    exit /b 1
)

REM Compile and link main.c directly with resources
cl src\main.c %CFLAGS% %INCLUDES% /link %ARCH% /OUT:build\%PROGRAM_NAME%.exe ^
    .\thirdparty\msvc-raylib\lib\raylib.lib ^
    build\%PROGRAM_NAME%.res ^
    /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup ^
    windowsapp.lib opengl32.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib
if errorlevel 1 (
    exit /b 1
)

del main.obj
