@echo off

:: Configuration
set VERSION_URL=https://raw.githubusercontent.com/horta/almosthere/master/VERSION
powershell -Command "(New-Object Net.WebClient).DownloadFile('%VERSION_URL%', 'VERSION.tmp')"
set /p VERSION=<VERSION.tmp && del VERSION.tmp
set FILE=athr-%VERSION%.zip
set DIR=almosthere-%VERSION%
set URL=https://github.com/horta/almosthere/archive/%VERSION%.zip

echo [0/5] Library(athr==%VERSION%)

:: Ancient Windows don't support TLS 1.1 and 1.2, so we fall back to insecure download.
set Version=
for /f "skip=1" %%v in ('wmic os get version') do if not defined Version set Version=%%v
for /f "delims=. tokens=1-3" %%a in ("%Version%") do (
  set Version.Major=%%a
  set Version.Minor=%%b
  set Version.Build=%%c
)

SET ORIGIN=%cd%
call :joinpath "%ORIGIN%" "install.log"
SET LOG_FILE=%Result%

:: Cleaning up previous mess
del /Q %FILE% ! >nul 2>&1
rd /S /Q %DIR% >nul 2>&1
del /Q %LOG_FILE% ! >nul 2>&1
copy /y nul %LOG_FILE% >nul 2>&1

echo|set /p="[1/5] Downloading... "
echo Fetching %URL% >>%LOG_FILE% 2>&1
powershell -Command "(New-Object Net.WebClient).DownloadFile('%URL%', '%FILE%')" >>%LOG_FILE% 2>&1
if %ERRORLEVEL% NEQ 0 (echo FAILED. && echo Log can be found at %LOG_FILE%. && exit /B 1) else (echo done.)

echo|set /p="[2/5] Extracting... "
powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('%FILE%', '.'); }"
if %ERRORLEVEL% NEQ 0 (echo FAILED. && echo Log can be found at %LOG_FILE%. && exit /B 1) else (echo done.)

cd %DIR% && mkdir build && cd build

echo|set /p="[3/5] Configuring... "
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%programfiles%\athr" >>%LOG_FILE% 2>&1
if %ERRORLEVEL% NEQ 0 (echo FAILED. && echo Log can be found at %LOG_FILE%. && exit /B 1) else (echo done.)

echo|set /p="[4/5] Compiling... "
nmake >>%LOG_FILE% 2>&1
if %ERRORLEVEL% NEQ 0 (echo FAILED. && echo Log can be found at %LOG_FILE%. && exit /B 1) else (echo done.)

echo|set /p="[5/5] Installing... "
nmake install >>%LOG_FILE% 2>&1
set PATH=%PATH%;%programfiles%\athr\bin
if %ERRORLEVEL% NEQ 0 (echo FAILED. && echo Log can be found at %LOG_FILE%. && exit /B 1) else (echo done.)

cd %ORIGIN% >nul 2>&1
del /Q %FILE% >nul 2>&1
rd /S /Q %DIR% >nul 2>&1

echo Details can be found at %LOG_FILE%.

@echo on
@goto :eof

:joinpath
set Path1=%~1
set Path2=%~2
if {%Path1:~-1,1%}=={\} (set Result=%Path1%%Path2%) else (set Result=%Path1%\%Path2%)
goto :eof

