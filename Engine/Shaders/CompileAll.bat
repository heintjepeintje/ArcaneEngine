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

PUSHD UI
CALL Compile.bat
POPD
