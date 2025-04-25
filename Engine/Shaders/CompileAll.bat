@ECHO OFF

PUSHD Geometry
CALL Compile.bat
POPD

PUSHD Light
CALL Compile.bat
POPD

PUSHD PostProcess
CALL Compile.bat
POPD

@REM PUSHD UI
@REM CALL Compile.bat
@REM POPD
