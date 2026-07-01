@setlocal
@echo off

@REM Make sure the current working directory is where the script is located
for %%I in ("%~dp0.") do (
    set "SCRIPT_DIR=%%~fI"
)

cd /d "%SCRIPT_DIR%"
@REM @echo on

@REM Clean-up previous run
echo del /q /f or1300SingleCore.*
del /q /f or1300SingleCore.*

@REM Synthesize
echo yosys -D GECKO5Education -s yosysOr1300.script
call yosys -D GECKO5Education -s yosysOr1300.script

if %errorlevel% neq 0 (
    if /I %0 equ "%~f0" pause
    exit /b 1
)

@REM Implement (place & route)
echo nextpnr-ecp5 --threads %NUMBER_OF_PROCESSORS% --timing-allow-fail --85k --package CABGA381 --json or1300SingleCore.json --lpf gecko5.lpf --textcfg or1300SingleCore.config
call nextpnr-ecp5 --threads %NUMBER_OF_PROCESSORS% --timing-allow-fail --85k --package CABGA381 --json or1300SingleCore.json --lpf gecko5.lpf --textcfg or1300SingleCore.config

if %errorlevel% neq 0 (
    if /I %0 equ "%~f0" pause
    exit /b 1
)

@REM Generate bitstream
echo ecppack --compress --freq 62.0 --input or1300SingleCore.config --bit ..\..\bit\or1300SingleCore.bit
call ecppack --compress --freq 62.0 --input or1300SingleCore.config --bit ..\..\bit\or1300SingleCore.bit

if %errorlevel% neq 0 (
    if /I %0 equ "%~f0" pause
    exit /b 1
)

@REM Load bitstream
echo openFPGALoader ..\..\bit\or1300SingleCore.bit
call openFPGALoader ..\..\bit\or1300SingleCore.bit

if %errorlevel% neq 0 (
    if /I %0 equ "%~f0" pause
    exit /b 1
)

if /I %0 equ "%~f0" pause
