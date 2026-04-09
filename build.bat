@echo off

REM === No-CRT Windows Development Template ===
set PROJECT_NAME=noWinCRT

:: 1. Compiler-only flags
set COMPILER_FLAGS=/Oi- /Zl /TC /GS- /D "UNICODE" /D "_UNICODE" /I"..\src"

set SDL_DIR=..\external\SDL3-3.4.4
set SDL_INC=/I"%SDL_DIR%\include"
set SDL_LIB_PATH=/LIBPATH:"%SDL_DIR%\lib\x64"

:: 2. Linker-only flags
set LINKER_FLAGS=/NODEFAULTLIB /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /STACK:0x100000,0x100000

:: 3. Libraries
set LIBS=kernel32.lib user32.lib gdi32.lib shell32.lib SDL3.lib

REM Create build directory if it doesn't exist
if not exist build mkdir build

pushd build
del /q *.*

echo Building %PROJECT_NAME%...

if "%1" == "debug" (
  echo Building Debug...
  cl /nologo /WX /Od /Zi %COMPILER_FLAGS% /Fe:"%PROJECT_NAME%.exe" ..\src\*.c /link %LIBS% %LINKER_FLAGS%
) else (
  echo Building Release...
  cl /nologo /O2 %COMPILER_FLAGS% /Fe:"%PROJECT_NAME%.exe" ..\src\*.c /link %LIBS% %LINKER_FLAGS%
)

if %errorlevel% neq 0 (
  echo Build failed!
  exit /b %errorlevel%
)

echo Build successful
popd

