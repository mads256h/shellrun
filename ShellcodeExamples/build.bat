@echo off
set back=%cd%
for /R %%i in (*.asm) do (
    cd "%%~pi"
    nasm -o "%%~ni.bin" -f bin "%%i"
    cd %back%
)
pause
