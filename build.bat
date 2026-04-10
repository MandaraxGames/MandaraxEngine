@echo off

REM === No-CRT Windows Development Template ===
set PROJECT_NAME=MandaraxEngine

set SDL_DIR=..\external\SDL3-3.4.4
set SDL_INC=/I"%SDL_DIR%\include"
set SDL_LIB_PATH=/LIBPATH:"%SDL_DIR%\lib\x64"

REM /D "SDL_MAIN_HANDLED" is critical; it stops SDL from hijacking your Entry Point
set COMPILER_FLAGS=/Oi- /Zl /TC /GS- /D "UNICODE" /D "_UNICODE" /D "SDL_MAIN_HANDLED" %SDL_INC% /I"..\src"


set LIBS=kernel32.lib user32.lib gdi32.lib shell32.lib SDL3.lib
set LINKER_FLAGS=/NODEFAULTLIB /ENTRY:RawEntryPoint /SUBSYSTEM:CONSOLE /STACK:0x100000,0x100000 %SDL_LIB_PATH%

REM Create build directory if it doesn't exist
if not exist build mkdir build

pushd build
del /q *.*
REM Copy the DLL to the build folder so the .exe can find it at runtime
copy "%SDL_DIR%\lib\x64\SDL3.dll" .

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

